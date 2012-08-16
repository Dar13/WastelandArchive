#include "StdAfx.h"
#include "Arena_Locker.h"

#include "LevelData.h"

#include "RecastInterface.h"
#include "DetourInterface.h"
#include "LuaManager.h"

//for now
#include "AI_include.h"

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

	/*Ogre::Entity* testEntity = _scene->createEntity("testLevel","arena_locker/testlevel.mesh","Models");
	_rootNode->attachObject(testEntity);
	InputGeometry testGeom(testEntity);
	
	RecastConfiguration testParams;
	testParams.setAgentHeight(2.5f);
	testParams.setAgentRadius(.2f);
	
	RecastInterface recast(_scene,testParams);
	recast.getRecastConfig().walkableRadius = static_cast<int>(.2f);
	recast.buildNavMesh(&testGeom);
	recast.exportPolygonMeshToObj("RECAST_NAVMESH_TEST.obj");

	rcdtConfig config;
	config.recastConfig = &recast.getRecastConfig();
	config.userConfig = &testParams;

	DetourInterface detour = DetourInterface(recast.getPolyMesh(),recast.getDetailMesh(),config);

	_rootNode->detachObject(testEntity);
	_scene->destroyEntity(testEntity);*/

	_handleScript(1001);

	//call the lua function
	LuaManager::getSingleton().prepFunction("arena_script_test");
	LuaManager::getSingleton().pushFunctionArg(1001);
	LuaManager::getSingleton().callFunction(1,1);
	
	if(lua_istable(LuaManager::getSingleton().getLuaState(),1))
	{
		lua_State* lua = LuaManager::getSingleton().getLuaState();
		//get behavior
		lua_pushstring(lua,"behavior");
		lua_gettable(lua,1);
		int behavior = lua_tonumber(lua,-1);
		lua_pop(lua,1);

		//get action
		lua_pushstring(lua,"action");
		lua_gettable(lua,1);
		int action = lua_tonumber(lua,-1);
		lua_pop(lua,1);

		std::string bhvTarget,actTarget;
		Ogre::Vector3 moveTarget;

		switch(behavior)
		{
		case AI::BHV_TALK:
			lua_pushstring(lua,"bhvtarget");
			lua_gettable(lua,1);
			bhvTarget = lua_tostring(lua,-1);
			lua_pop(lua,1);
			break;
		case AI::BHV_MOVE:
			lua_pushstring(lua,"bhvtarget");
			lua_gettable(lua,-2);
			if(lua_istable(lua,-1))
			{
				/*lua_pushnumber(lua,1);
				lua_gettable(lua,-2);
				moveTarget.x = lua_tonumber(lua,-1);
				std::cout << moveTarget.x;
				lua_pop(lua,1);
				
				lua_pushnumber(lua,2);
				lua_gettable(lua,-2);*/

				for(int i = 0; i < 3; i++)
				{
					lua_pushnumber(lua,i+1);
					lua_gettable(lua,-2);
					moveTarget[i] = lua_tonumber(lua,-1);
					lua_pop(lua,1);
				}

				std::cout << moveTarget << std::endl;

				lua_pop(lua,1);
				//lua_pop(lua,2);
			}
			break;
		case AI::BHV_FOLLOW:

			break;
		}

		switch(action)
		{
		case AI::ACT_SHOOT:

			break;
		case AI::ACT_CHANGEWEP:

			break;
		}
	}
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
	LuaManager::getSingleton().pushFunctionArgVector(pos);
	LuaManager::getSingleton().callFunction(2,0);
}