#include "StdAfx.h"
#include "Arena_Locker.h"

#include "LevelData.h"

#include "RecastInterface.h"
#include "DetourInterface.h"
#include "LuaManager.h"

#include "AI\npc_character.h"

#include "CrowdManager.h"

ArenaLocker::ArenaLocker()
{	
	_camera = nullptr;
	_view = nullptr;
	_rootNode = nullptr;
	_stateShutdown = false;
	_scene = nullptr;
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
	
	//TESTING IT OUT

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

		NPCCharacter npc(obj->name(),obj->scriptName(),node,_crowd.get());
		_npcs.push_back(npc);

		delete obj;
	}

	//_handleScript(1001);
}

int ArenaLocker::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_camera->setPosition(Ogre::Vector3(-5,25,-5));
	_camera->setFarClipDistance(1000);
	_camera->lookAt(Ogre::Vector3::ZERO);

	Ogre::Light* light = _scene->createLight("arenaLockerLight");
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(Ogre::Vector3(1.5,1.5,2.0));
	Graphics->setLightRange(light,15);

	bool exitNow = _stateShutdown;

	float time,delta,oldtime = static_cast<float>(Graphics->getTimer()->getMilliseconds());
	while(!_stateShutdown)
	{
		//checks for escapekey press and updates input manager.
		_stateShutdown = Input->Update(true);

		time = static_cast<float>(Graphics->getTimer()->getMilliseconds());
		delta = time - oldtime;
		oldtime = time;

		//Update the crowd manager
		_crowd->updateTick(delta / 1000.0f);

		for(auto itr = _npcs.begin(); itr != _npcs.end(); ++itr)
		{
			itr->update(delta);
		}

		if(!GameManager::UpdateManagers(Graphics,_physics.get(),delta))
		{
			_stateShutdown = true;
		}
	}

	if(exitNow)
	{
		MessageBoxA(NULL,"Check the WTLD.log file or the debug console for information on this error.","Error",MB_OK);
		return State::END;
	}

	//Eventuall will read GAME_LOBBY or something.
	return State::END;
}

void ArenaLocker::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
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