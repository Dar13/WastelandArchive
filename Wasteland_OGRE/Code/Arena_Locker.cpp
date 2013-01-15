#include "StdAfx.h"
#include "Arena_Locker.h"

#include "LevelData.h"

#include "RecastInterface.h"
#include "DetourInterface.h"
#include "LuaManager.h"

#include "AI\npc_character.h"

#include "CrowdManager.h"

#include "SoundManager.h"

#include "interfaces\soundlist.hxx"

ArenaLocker::ArenaLocker()
	: _camera(nullptr),
	  _view(nullptr),
	  _rootNode(nullptr),
	  _stateShutdown(nullptr),
	  _scene(nullptr),
	  _cameraMovementTime(0.0f)
{
	_returnValue = State::GAME_LOBBY;
}

void ArenaLocker::Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_scene = Graphics->createSceneManager(Ogre::ST_INTERIOR,"arenaLocker");
	_rootNode = _scene->getRootSceneNode();

	_camera = _scene->createCamera("arenaLockerCam");
	_camera->setAspectRatio(16.0f / 9.0f);
	_camera->setNearClipDistance(0.001f);
	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));
	std::cout << "Arena Locker - scene manager and camera/viewport created" << std::endl;

	_physics.reset(new PhysicsManager());
	btVector3 grav(0.0f,-9.8f,0.0f);
	_physics->Setup(grav);
	std::cout << "Arena Locker - physics setup" << std::endl;

	//list of physics-based entities and such
	_loadPhysicsEntities("resource\\xml\\lists\\arenalocker_list.xml");
	std::cout << "Arena Locker - models loaded" << std::endl;

	std::vector<LevelData::Waypoint> waypoints;
	LevelData::LevelParser parser;
	parser.setFile("resource\\models\\arena_locker\\arenalocker_test.ent");
	parser.parseWaypoints(&waypoints);
	std::cout << "Arena Locker - parser finished" << std::endl;

	Ogre::Entity* levelEnt = static_cast<Ogre::Entity*>(_pairs.begin()->ogreNode->getAttachedObject(0));
	_navigationMeshSetup(levelEnt);

	//_AIManager.reset(new AIManager());
	//_AIManager->loadNPCs("resource\\xml\\lists\\arenalocker_npc_list.xml",_crowd.get(),_scene,.9f);
	std::cout << "NPCs loaded" << std::endl;

	_loadSounds("resource\\xml\\arena_locker\\locker_soundlist.xml",Sound);
	std::cout << "Sounds loaded" << std::endl;

	_pauseMenu.reset(new PauseMenu(State::GAME_LOCKER));
	_pauseMenu->Setup(Input,Graphics,Gui,Sound);
	std::cout << "Pause menu initialized" << std::endl;

	_oldCameraPositionTarget = Ogre::Vector3(-7,.36f,-8.0f);
	LuaManager::getSingleton().callFunction("Arena_InitCameraMovement",1);
	_cameraPositionTarget = LuaManager::getVectorFromLua(LuaManager::getSingletonPtr()->getLuaState(),1);
	LuaManager::getSingleton().clearLuaStack();

	//TEST
	_sphere = Graphics->createSceneNode(_scene,object("resource/xml/test_sphere.xml").get(),_rootNode);
	//TEST
}

int ArenaLocker::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	//_camera->setPosition(Ogre::Vector3(-7.0f,.36f,-8.07f));
	_camera->setPosition(Ogre::Vector3(-20.0f,10.0f,-10.0f));
	_camera->setFarClipDistance(1000);
	_camera->lookAt(Ogre::Vector3::ZERO);

	Ogre::Light* light = _scene->createLight("arenaLockerLight");
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(Ogre::Vector3(1.5,1.5,2.0));
	Graphics->setLightRange(light,15);

	bool exitNow = _stateShutdown;
	bool paused = false;

	float time; _oldTime = Graphics->getCurrentTimeMs();
	while(!_stateShutdown)
	{
		//updates input manager.
		if(!Input->Update(false))
		{
			_stateShutdown = true;
		}

		time = Graphics->getCurrentTimeMs();
		_deltaTime = time - _oldTime;
		_oldTime = time;

		if(paused)
		{
			paused = false;
			_deltaTime = 16.6f;
		}

		_handleScript();
		//_handleCamera();

		//Update the crowd manager
		//_crowd->updateTick(_deltaTime / 1000.0f);

		//_AIManager->update(_deltaTime);

		if(!GameManager::UpdateManagers(Graphics,_physics.get(),_deltaTime))
		{
			_stateShutdown = true;
		}

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

	Graphics->cleanAndDestroySceneManager(_scene);

	LuaManager::getSingletonPtr()->deepClean();
}

void ArenaLocker::_handleScript()
{
	//LuaManager* lua = LuaManager::getSingletonPtr();
	//lua->prepFunction("Arena_CameraMovement");
	//lua->pushFunctionArgVector(_camera->getPosition());

	//if(_deltaTime == 0)
	//{
	//	lua->pushFunctionArg(16.0f);
	//}
	//else
	//{
	//	lua->pushFunctionArg(_deltaTime);
	//}
	//lua->callFunction(2,2);

	////check for returns
	//lua_State* luaS = lua->getLuaState();
	////assume it's a vector that's being returned. Otherwise some seriously fucked up shit is going on.
	//Ogre::Vector3 targetVector = Ogre::Vector3::UNIT_X;

	//if(lua_istable(luaS,1))
	//{
	//	targetVector = LuaManager::getVectorFromLua(luaS,1);
	//	//std::cout << targetVector << std::endl;
	//}

	//if(lua_isnumber(luaS,2))
	//{
	//	_cameraMovementTime = lua_tonumber(luaS,2);
	//}
	//else
	//{
	//	_cameraMovementTime = 0.0f;
	//}

	//if(targetVector == Ogre::Vector3::ZERO)
	//{
	//	_stateShutdown = true;
	//}
	//else
	//{
	//	//only change if it's not the same target!
	//	if(targetVector != _cameraPositionTarget)
	//	{
	//		_oldCameraPositionTarget = _cameraPositionTarget;
	//		_cameraPositionTarget = targetVector;
	//	}
	//}

	_cameraMovementTime += .005f;

	if(_cameraMovementTime > 1.0f)
	{
		_cameraMovementTime = 0.0f;
	}

	LuaManager::getSingletonPtr()->prepFunction("Bezier_Test");
	LuaManager::getSingletonPtr()->pushFunctionArg(_cameraMovementTime);
	LuaManager::getSingletonPtr()->callFunction(1,1);

	//get the vector from it.
	Ogre::Vector2 tmp = Ogre::Vector2::ZERO;
	lua_State* l = LuaManager::getSingleton().getLuaState();
	if(lua_istable(l,1))
	{
		for(int i = 1; i < 3; ++i)
		{
			lua_pushnumber(l,i);
			lua_gettable(l,1);
			if(lua_isnumber(l,-1))
			{
				tmp[i-1] = static_cast<float>(lua_tonumber(l,-1));
			}
			else
			{
				tmp[i-1] = 0;
			}
			lua_pop(l,1);
		}
	}

	if(tmp != Ogre::Vector2::ZERO)
	{
		//std::cout << tmp << std::endl;
		//move something along it.
		_sphere->setPosition(tmp.x,1.0f,tmp.y);
	}

	LuaManager::getSingletonPtr()->clearLuaStack();

	//handle updating the camera in another function.
	//keeps this one cleaner.

	return;
}

void ArenaLocker::_handleCamera()
{
	//_cameraMovementTime += _deltaTime;
	//position...
	if(_cameraPositionTarget == _camera->getPosition())
	{
		//do nothing
	}

	//otherwise, let's go about updating the position
	Ogre::Vector3 newPos = Utility::vector3_lerp(_oldCameraPositionTarget,_cameraPositionTarget,_cameraMovementTime,3.5);
	_camera->setPosition(newPos);

	//rotation...
	_camera->lookAt(_cameraPositionTarget);

	return;
}

void ArenaLocker::_handleSoundEvents(std::vector<SoundEvent>& events, SoundManager* Sound)
{
	if(events.size() == 0)
	{
		return;
	}

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

void ArenaLocker::_loadPhysicsEntities(std::string fileName)
{
	auto objList = list(fileName.c_str());
	for(auto itr = objList->file().begin(); itr != objList->file().end(); ++itr)
	{
		_pairs.push_back(GameManager::createObject(_scene,(*itr),_physics.get()));
	}
}

void ArenaLocker::_loadSounds(std::string fileName, SoundManager* Sound)
{
	auto sndList = soundList(fileName.c_str());
	sSound sound;
	for(auto itr = sndList->sound().begin(); itr != sndList->sound().end(); ++itr)
	{
		sound.is3D = itr->is3D();
		sound.isLooping = itr->looping();
		sound.name = itr->name();
		Sound->createSound(sound,itr->filename().c_str());
		_sounds.push_back(sound);
	}

	return;
}

void ArenaLocker::_navigationMeshSetup(Ogre::Entity* levelEntity)
{
	InputGeometry levelGeometry(levelEntity);

	RecastConfiguration params(.2f,2.5f);

	_recast.reset(new RecastInterface(_scene,params));
	_recast->getRecastConfig().walkableRadius = static_cast<int>(.2f); // zero?
	_recast->buildNavMesh(&levelGeometry);
	_recast->exportPolygonMeshToObj("ARENALOCKER_RECAST_MESH.obj");

	rcdtConfig config;
	config.recastConfig = &_recast->getRecastConfig();
	config.userConfig = &_recast->getRecastBuildConfiguration();

	_detour.reset(new DetourInterface(_recast->getPolyMesh(),_recast->getDetailMesh(),config));

	_crowd.reset(new CrowdManager(_detour.get(),&config));
}