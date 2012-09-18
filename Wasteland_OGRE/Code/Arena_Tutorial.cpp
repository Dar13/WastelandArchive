#include "StdAfx.h"

#include "Arena_Tutorial.h"
#include "GameManager.h"
#include "debug\console.h"
#include "Utility.h"
#include <boost\lexical_cast.hpp>

#include "LuaManager.h"

ArenaTutorial::ArenaTutorial()
{
	_camera = 0;
	_scene = 0;
	_rootNode = 0;
	_stateShutdown = false;
	_view = 0;
	_deltaTime = 0;
	_oldTime = 0;
}

void ArenaTutorial::Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_scene = Graphics->getRoot()->createSceneManager(Ogre::ST_INTERIOR,"arenaTut");

	_camera = _scene->createCamera("arenaTutCam");
	_camera->setAspectRatio(16.0f/9.0f);
	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));

	_rootNode = _scene->getRootSceneNode();

	//physics setup
	_physics.reset(new PhysicsManager() );
	_physics->Setup();
	btVector3 grav(0.0f,-9.8f,0.0f);
	_physics->setGravity(grav);

	//using a list instead of hardcoded files
	std::auto_ptr<list_t> objList = list("resource\\xml\\lists\\arena_list.xml");
	for(list_t::file_const_iterator itr = objList.get()->file().begin(); itr != objList.get()->file().end(); ++itr)
	{
		std::string tmp = (*itr);
		_pairs.push_back(GameManager::createObject(_scene,tmp,_physics.get(),Graphics));
	}

	//camera setup
	//_camera->setPosition(Ogre::Vector3(30,1.9f,0));
	_camera->setPosition(Ogre::Vector3(0.0f,1.9f,0.0f));
	_camera->setNearClipDistance(.001f);
	_camera->setFarClipDistance(1000.0f);
	_camera->lookAt(0,1.9f,0);

	//player setup
	_player.reset(new Player());
	
	//set the camera aspect ratio
	_camera->setAspectRatio(16.0f/9.0f);
	//wireframe
	//_camera->setPolygonMode(Ogre::PM_WIREFRAME);

	//let's try out the character controller
	_controller.reset(new CharacterController(_camera,Ogre::Vector3(70.0f,1.9f,0.0f),_physics->getWorld(),Graphics ) );
	//since we're using the character controller, should also lock the mouse.
	Input->setMouseLock(true);

	//let's setup the EWS system
	_ews.reset(new EWSManager(_scene));

	LevelData::LevelParser parser;
	parser.setFile("resource\\models\\mapscapeTEST\\zonedoor_scriptTest.ent");
	parser.parseDoors(&_doors);
	parser.parseLights(&_lights);
	parser.parseTriggers(&_triggers);

	std::cout << "Parser finished" << std::endl;

	_setupLights(Graphics,_scene);
	OgreBulletPair level = _pairs.at(1);
	_setupDoors(level,_scene,_physics.get(),Graphics);

	std::cout << "Level elements setup" << std::endl;

	//testing out the M9SE
	EquippableObject equipObj = GameManager::createEquippable(_scene,"resource\\xml\\weapon_m9se.xml",Graphics,Sound,true);
	_player->addEquippableObject(equipObj);
	_player->Setup("TEST",Graphics,_controller->getNode());
	
	std::cout << "Player setup" << std::endl;

	_pauseMenu.reset(new PauseMenu(State::GAME_ARENA));
	_pauseMenu->Setup(Input,Graphics,Gui,Sound);

	//physics debug drawer.
	//_physics->setDebugDrawer(new CDebugDraw(_scene,_physics->getWorld()));
}

int ArenaTutorial::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_returnValue = State::GAME_LOCKER;
	_stateShutdown=false;
	//Ogre::SceneNode* tmpNode = (Ogre::SceneNode*)_scene->getRootSceneNode()->getChild("nodetestSphere");

	OgreTransform playerTransform;

	bool polySwitch = false;
	
	float time;
	bool paused = false;
	//while the escape key isn't pressed and the state isn't told to shutdown.
	while(!_stateShutdown)
	{
		//current time
		time = static_cast<float>(Graphics->getTimer()->getMilliseconds());
		//getting the delta time
		_deltaTime = time - _oldTime;
		//setting the old time
		_oldTime = time;

		if(paused)
		{
			_deltaTime = 16.6f;
			paused = false;

		}

		//quick visual debugging tool
		if(Input->isMBPressed(OIS::MB_Right))
		{
			if(!polySwitch)
			{
				if(_camera->getPolygonMode() == Ogre::PM_WIREFRAME)
				{
					_camera->setPolygonMode(Ogre::PM_SOLID);
				}
				else
				{
					_camera->setPolygonMode(Ogre::PM_WIREFRAME);
				}
				polySwitch = true;
			}
		}
		else
		{
			if(polySwitch)
			{
				polySwitch = false;
			}
		}

		//the state shutdown based on player input(?)
		if(!Input->Update(false))
		{
			_stateShutdown = true;
		}

		//Update the character controller
		_controller->update(_deltaTime,Input,playerTransform);

		//Update Player-specific stuff
		_player->Update(Input,_physics.get(),_ews.get(),playerTransform);

		//Update the EWS system
		_ews->Update(static_cast<int>(time),
					 Input->isCFGKeyPressed(InputManager::ENVWARNSYS),
					 _player->getPlayerData(),
					 playerTransform);

		//True indicates success, so react on if it doesn't react properly
		if(!GameManager::UpdateManagers(Graphics,_physics.get(),_deltaTime))
		{
			//shuts down appstate
			_stateShutdown = true;
		}

		_updateLights();
		_updateTriggers(playerTransform,static_cast<int>(time));
		_updateDoors();

		//handling the pause menu
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

	return _returnValue;
}

//clean-up of state
void ArenaTutorial::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_controller->shutdown();

	//undo what I set in OIS
	Input->setMouseLock(false);

	std::for_each(_pairs.begin(),_pairs.end(),[] (OgreBulletPair ipair) {
		if(ipair.btBody->getCollisionShape()->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
		{
			btBvhTriangleMeshShape* mesh = static_cast<btBvhTriangleMeshShape*>(ipair.btBody->getCollisionShape());
			btTriangleMesh* trimesh = static_cast<btTriangleMesh*>(mesh->getUserPointer());
			delete trimesh;
		}
	});

	//clean up what you initialized in the Setup() function.
	Graphics->getRenderWindow()->removeAllViewports();
	
	//Destroy the camera, since we've taken care of the viewport.
	_scene->destroyCamera(_camera);
	//Destroy all SceneNodes, Entites, etc.
	_scene->clearScene();

	//Set to null.
	_rootNode=0;

	//Clear the vectors
	_nodes.clear();
	_entities.clear();
	
	//I'm clearing them due to physicsManager clean-up.
	_constraints.clear();

	//cleaning up state-specific pointers.
	for(auto itr = _channels.begin(); itr != _channels.end(); ++itr) { (*itr)->stop(); }
	for(auto itr = _sounds.begin(); itr != _sounds.end(); ++itr) { (*itr).sound->release(); }
	_channels.clear();
	_sounds.clear();
	//since all pointers are std::unique_ptr members, then they will be deleted upon class destruction.

	//Destroy the scene manager.
	Graphics->getRoot()->destroySceneManager(_scene);

	_lights.clear();
	_triggers.clear();
	_doors.clear();
}

void ArenaTutorial::_setupLights(GraphicsManager* g, Ogre::SceneManager* scene)
{
	for(auto itr = _lights.begin(); itr != _lights.end(); ++itr)
	{
		switch((*itr)->getLightType())
		{
		case Ogre::Light::LT_SPOTLIGHT:
			static_cast<LevelData::SpotLightData*>((*itr).get())->createLight(scene,g);
			break;
		case Ogre::Light::LT_POINT:
			static_cast<LevelData::PointLightData*>((*itr).get())->createLight(scene,g);
			break;
		case Ogre::Light::LT_DIRECTIONAL:
			static_cast<LevelData::DirectionalLightData*>((*itr).get())->createLight(scene,g);
			break;
		};
	}
}

void ArenaTutorial::_updateLights()
{
	for(auto itr = _lights.begin(); itr != _lights.end(); ++itr)
	{
		(*itr)->update();
	}
}

void ArenaTutorial::_updateTriggers(OgreTransform& playerTransform, int currentTime)
{
	for(auto itr = _triggers.begin(); itr != _triggers.end(); ++itr)
	{
		switch((*itr)->getTriggerType())
		{
		case LevelData::PLAYER:
			static_cast<LevelData::PlayerTrigger*>((*itr).get())->update(playerTransform);
			break;
		case LevelData::ENTITY:
			static_cast<LevelData::EntityTrigger*>((*itr).get())->update();
			break;
		case LevelData::TIME:
			static_cast<LevelData::TimeTrigger*>((*itr).get())->update(currentTime);
			break;
		};
	}
}

void ArenaTutorial::_setupDoors(OgreBulletPair& mainLevel,Ogre::SceneManager* scene,PhysicsManager* p,GraphicsManager* g)
{
	for(auto itr = _doors.begin(); itr != _doors.end(); ++itr)
	{
		(*itr)->createDoor(scene,g,p,&mainLevel);
		LuaManager::getSingleton().addEntity((*itr)->getName(),(*itr).get());
	}
}

void ArenaTutorial::_updateDoors()
{
	for(std::vector<std::unique_ptr<LevelData::DoorData>>::iterator itr = _doors.begin(); itr != _doors.end(); ++itr)
	{
		(*itr)->update();
	}
}