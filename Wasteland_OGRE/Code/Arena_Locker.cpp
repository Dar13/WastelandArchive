#include "StdAfx.h"
#include "Arena_Locker.h"

#include "LevelData.h"

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
		std::cout << (*itr) << std::endl;
	}
	std::cout << "Arena Locker - models loaded" << std::endl;

	std::vector<LevelData::Waypoint> waypoints;
	LevelData::LevelParser parser;
	parser.setFile("resource\\models\\arena_locker\\arenalocker_test.ent");
	parser.parseWaypoints(&waypoints);
	std::cout << "Arena Locker - parser finished" << std::endl;

	LevelData::WaypointSet waypointSet(waypoints,true);
	//_cameraTrack = waypointSet.generateSpline();
	_cameraTrack.setCamera(_camera);
	_cameraTrack.setWaypoints(waypointSet);
	_cameraTrack.generateCurve();
	std::cout << "Arena Locker - camera track created" << std::endl;
	
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

	Ogre::SceneNode* _testNode = _rootNode->createChildSceneNode("testNode",light->getPosition());
	light->setPosition(Ogre::Vector3::ZERO);
	//_testNode->attachObject(light);
	Ogre::Entity* testEnt = _scene->createEntity("entBox","test/test_box.mesh","Models");
	_testNode->attachObject(testEnt);

	NodeTrack testTrack;
	testTrack.setWaypoints(_cameraTrack.getWaypoints());
	testTrack.generateCurve();
	testTrack.setNode(_testNode);
	testTrack.setAtStart();
	testTrack.setTargetTimeLength(10000.0f);
	testTrack.update(0.0f);

	bool trackUpdate = false;

	float time,delta,oldtime = Graphics->getTimer()->getMilliseconds();
	while(!_stateShutdown)
	{
		//checks for escapekey press and updates input manager.
		_stateShutdown = Input->Update(true);

		time = static_cast<float>(Graphics->getTimer()->getMilliseconds());
		delta = time - oldtime;
		oldtime = time;

		if(Input->isMBPressed(OIS::MB_Right))
		{
			if(!trackUpdate)
			{
				testTrack.update(50.0f);
				trackUpdate = true;
				std::cout << "testTrack updated" << std::endl;
				std::cout << _testNode->getPosition() << std::endl;
			}
		}
		else
		{
			trackUpdate = (trackUpdate) ? false : trackUpdate;
		}

		testTrack.update(delta);

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