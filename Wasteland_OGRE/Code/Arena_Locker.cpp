#include "StdAfx.h"
#include "Arena_Locker.h"

#include "LevelData.h"

#include "RecastInterface.h"

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
	_physics->setGravity(btVector3(0.0f,-9.8f,0.0f));
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

	LevelData::WaypointSet waypointSet(waypoints,true);
	std::cout << "Arena Locker - camera track created" << std::endl;

	Ogre::Entity* testEntity = _scene->createEntity("testLevel","arena_locker/testlevel.mesh","Models");
	_rootNode->attachObject(testEntity);
	InputGeometry testGeom(testEntity);
	
	RecastConfiguration testParams;
	testParams.setAgentHeight(2.5f);
	testParams.setAgentRadius(.2f);
	
	RecastInterface recast(_scene,testParams);
	recast.buildNavMesh(&testGeom);
	recast.exportPolygonMeshToObj("RECAST_NAVMESH_TEST.obj");
	recast.recastClean();

	_rootNode->detachObject(testEntity);
	_scene->destroyEntity(testEntity);
	
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

	return State::END;
}

void ArenaLocker::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	Graphics->getRenderWindow()->removeAllViewports();

	_scene->destroyCamera(_camera);
	_scene->clearScene();

	Graphics->getRoot()->destroySceneManager(_scene);
}