#include "StdAfx.h"
#include "Arena_Locker.h"

#include "LevelData.h"

#include "RecastInterface.h"
#include "DetourInterface.h"
#include "LuaManager.h"

#include "AI\npc_character.h"

#include "CrowdManager.h"

#include "SoundManager.h"

ArenaLocker::ArenaLocker()
	: _camera(nullptr),
	  _view(nullptr),
	  _rootNode(nullptr),
	  _stateShutdown(nullptr),
	  _scene(nullptr)
{
	_returnValue = State::GAME_LOBBY;
}

void ArenaLocker::Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_scene = Graphics->getRoot()->createSceneManager(Ogre::ST_INTERIOR,"arenaLocker");

	_camera = _scene->createCamera("arenaLockerCam");
	_camera->setAspectRatio(16.0f / 9.0f);
	_camera->setNearClipDistance(0.001f);
	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));
	std::cout << "Arena Locker - scene manager and camera/viewport created " << std::endl;

	_rootNode = _scene->getRootSceneNode();

	_physics.reset(new PhysicsManager());
	_physics->Setup();
	btVector3 grav(0.0f,-9.8f,0.0f);
	_physics->setGravity(grav);
	std::cout << "Arena Locker - physics setup" << std::endl;

	//list of physics-based entities and such
	auto objList = list("resource\\xml\\lists\\arenalocker_list.xml");
	for(auto itr = objList->file().begin(); itr != objList->file().end(); ++itr)
	{
		_pairs.push_back(GameManager::createObject(_scene,(*itr),_physics.get(),Graphics));
	}
	std::cout << "Arena Locker - models loaded" << std::endl;

	std::vector<LevelData::Waypoint> waypoints;
	LevelData::LevelParser parser;
	parser.setFile("resource\\models\\arena_locker\\arenalocker_test.ent");
	parser.parseWaypoints(&waypoints);
	std::cout << "Arena Locker - parser finished" << std::endl;

	Ogre::Entity* levelEnt = static_cast<Ogre::Entity*>(_pairs.begin()->ogreNode->getAttachedObject(0));
	InputGeometry levelGeometry(levelEnt);

	RecastConfiguration params;
	params.setAgentHeight(2.5f);
	params.setAgentRadius(.2f);

	_recast.reset(new RecastInterface(_scene,params));
	_recast->getRecastConfig().walkableRadius = static_cast<int>(.2f); // zero?
	_recast->buildNavMesh(&levelGeometry);
	_recast->exportPolygonMeshToObj("ARENALOCKER_RECAST_MESH.obj");

	rcdtConfig config;
	config.recastConfig = &_recast->getRecastConfig();
	config.userConfig = &_recast->getRecastBuildConfiguration();

	_detour.reset(new DetourInterface(_recast->getPolyMesh(),_recast->getDetailMesh(),config));

	_crowd.reset(new CrowdManager(_detour.get(),&config));

	//NPC list
	auto npcList = list("resource\\xml\\lists\\arenalocker_npc_list.xml");
	for(auto itr = npcList->file().begin(); itr != npcList->file().end(); ++itr)
	{
		characterobject_t* obj = characterObject(*itr).release();
		Ogre::SceneNode* node = GameManager::createCharacterObject(_scene,obj,Graphics);

		NPCCharacter* npc = new NPCCharacter(obj->name(),obj->scriptName(),node,_crowd.get());
		npc->setMaxSpeed(.9f);
		_npcs.push_back(npc);
		LuaManager::getSingleton().addEntity(npc->getName(),npc);

		delete obj;
	}

	//sound(walking of feet and such)
	sSound walkingSound;
	walkingSound.is3D = true;
	walkingSound.isLooping = false;
	walkingSound.name = "lockerFootsteps";
	walkingSound.type = SFX;
	Sound->createSound(walkingSound,"resource\\music\\locker\\footsteps.mp3");
	_sounds.push_back(walkingSound);

	_pauseMenu.reset(new PauseMenu(State::GAME_LOCKER));
	_pauseMenu->Setup(Input,Graphics,Gui,Sound);
}

int ArenaLocker::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_camera->setPosition(Ogre::Vector3(0,0,0));
	_camera->setFarClipDistance(1000);
	_camera->lookAt(Ogre::Vector3::ZERO);

	Ogre::Light* light = _scene->createLight("arenaLockerLight");
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(Ogre::Vector3(1.5,1.5,2.0));
	Graphics->setLightRange(light,15);

	bool exitNow = _stateShutdown;
	bool paused = false;

	LuaData footstepData;
	int timeSinceLastFootstep = 0;

	float time,delta,oldtime = static_cast<float>(Graphics->getTimer()->getMilliseconds());
	while(!_stateShutdown)
	{
		//updates input manager.
		if(!Input->Update(false))
		{
			_stateShutdown = true;
		}

		time = static_cast<float>(Graphics->getTimer()->getMilliseconds());
		delta = time - oldtime;
		oldtime = time;

		if(paused)
		{
			paused = false;
			delta = 16.6f;
		}

		//Update the crowd manager
		_crowd->updateTick(delta / 1000.0f);

		for(auto itr = _npcs.begin(); itr != _npcs.end(); ++itr)
		{
			(*itr)->update(delta);
		}

		if(!GameManager::UpdateManagers(Graphics,_physics.get(),delta))
		{
			_stateShutdown = true;
		}

		/*if(LuaManager::getSingletonPtr()->searchForLuaData("lockerNPCFootsteps",&footstepData))
		{
			int data;
			if(Utility::getIntFromLuaData(footstepData,data))
			{
				if(data == 1)
				{
					if(timeSinceLastFootstep > 500)
					{
						Ogre::Vector3 vel = Ogre::Vector3::ZERO;
						Sound->playSound(_sounds[0],_npcs[0]->getPosition(),vel);
						timeSinceLastFootstep = 0;
					}
					else
					{
						timeSinceLastFootstep += delta;
					}
				}
			}
		}*/

		_handleSoundEvents(LuaManager::getSingletonPtr()->getSoundEventQueue(),Sound);
		Sound->Update(Input->getConfiguration());

		if(Input->escapePressed() && !paused)
		{
			int ret = _pauseMenu->Run(Input,Graphics,Gui,Sound);
			if(ret == State::END)
			{
				_stateShutdown = true;
				_returnValue = ret;
			}
			else
			{
				paused = true;
			}
		}
	}

	if(exitNow)
	{
		MessageBoxA(NULL,"Check the WTLD.log file or the debug console for information on this error.","Error",MB_OK);
		return State::END;
	}

	//Eventually will be GAME_LOBBY or something.
	return _returnValue;
}

void ArenaLocker::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	std::for_each(_npcs.begin(),_npcs.end(),[] (NPCCharacter* npc) {
		delete npc;
		npc = nullptr;
	});
	_npcs.clear();

	std::for_each(_pairs.begin(),_pairs.end(),[] (OgreBulletPair ipair) {
		if(ipair.btBody->getCollisionShape()->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
		{
			btBvhTriangleMeshShape* mesh = static_cast<btBvhTriangleMeshShape*>(ipair.btBody->getCollisionShape());
			btTriangleMesh* trimesh = static_cast<btTriangleMesh*>(mesh->getUserPointer());
			delete trimesh;
		}
	});

	std::for_each(_sounds.begin(),_sounds.end(),[Sound] (sSound snd) {
		Sound->destroySound(snd);
	});

	_pauseMenu->Shutdown(Input,Graphics,Gui,Sound);

	Graphics->getRenderWindow()->removeAllViewports();

	_scene->destroyCamera(_camera);
	_scene->clearScene();

	Graphics->getRoot()->destroySceneManager(_scene);
}

void ArenaLocker::_handleScript(unsigned long deltaTime)
{
	LuaManager::getSingleton().prepFunction("arena_Locker");
	LuaManager::getSingleton().pushFunctionArg(static_cast<double>(deltaTime));
	Ogre::Vector3 pos(1,2,4);
	LuaManager::getSingleton().addDataPointer("playerPosition",&pos);
	LuaManager::getSingleton().pushFunctionArgVector(pos);
	LuaManager::getSingleton().callFunction(2,0);
	LuaManager::getSingleton().removeDataPointer("playerPosition");
}

void ArenaLocker::_handleSoundEvents(std::vector<SoundEvent>& events, SoundManager* Sound)
{
	if(events.size() == 0)
	{
		return;
	}
	//std::map<sSound,FMOD_VECTOR> soundsToPlay;
	/*auto findSoundStruct = [this] (std::string& name) -> sSound
	{
		for(auto i = _sounds.begin(); i != _sounds.end(); ++i)
		{
			if((*i) == name)
			{
				return (*i);
			}
		}
	};*/

	sSound sndTmp;
	FMOD_VECTOR vel; vel.x = 0; vel.y = 0; vel.z = 0;
	for(auto itr = events.begin(); itr != events.end(); ++itr)
	{
		auto sndItr = std::find(_sounds.begin(),_sounds.end(),(*itr).name);
		if(sndItr != _sounds.end())
		{
			sndTmp = *sndItr;
		}
		else
		{
			continue;
		}

		if(itr->is3D)
		{
			Sound->playSound(sndTmp,itr->position,vel);
		}
		else
		{
			Sound->playSound(sndTmp);
		}
	}

	events.clear();

	return;
}