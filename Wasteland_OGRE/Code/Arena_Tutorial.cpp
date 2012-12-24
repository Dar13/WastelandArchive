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
	_scene = Graphics->getRoot()->createSceneManager("OctreeSceneManager","arenaTut");

	_camera = _scene->createCamera("arenaTutCam");
	_camera->setAspectRatio(16.0f/9.0f);
	_camera->setQueryFlags(CAMERA_MASK);
	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));

	_camera->setFarClipDistance(100);
	_camera->setNearClipDistance(.001);
	
	_shadowListener = new ShadowListener();

	_scene->setShadowTextureSelfShadow(true);
	_scene->setShadowTextureCasterMaterial("shadow_caster");
	_scene->setShadowTextureCount(4);
	_scene->setShadowTextureSize(1024);
	_scene->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_GR);
	_scene->setShadowCasterRenderBackFaces(false);
	//_scene->setShowDebugShadows(true);
	//_scene->setShadowColour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));
	const unsigned numberShadowRTTs = _scene->getShadowTextureCount();
	for(unsigned int i = 0; i < numberShadowRTTs; ++i)
	{
		Ogre::TexturePtr tex = _scene->getShadowTexture(i);
		Ogre::Viewport* vp = tex->getBuffer()->getRenderTarget()->getViewport(0);
		vp->setBackgroundColour(Ogre::ColourValue(1,1,1,1));
		vp->setClearEveryFrame(true);
	}
	
	_scene->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);
	_scene->addListener(_shadowListener);

	_shadowCompositorListener = new ShadowCompositorListener(_camera);
	
	_SSAO = Ogre::CompositorManager::getSingleton().addCompositor(_view,"ssao");
	//_SSAO->setEnabled(true);
	_SSAO->addListener(_shadowCompositorListener);

	_scene->setAmbientLight(Ogre::ColourValue(.3f,.3f,.3f));

	Ogre::Light* test = _scene->createLight("testLight");
	test->setType(Ogre::Light::LT_SPOTLIGHT);
	test->setCastShadows(true);
	Graphics->setLightRange(test,30.0f);
	test->setDiffuseColour(Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));
	test->setSpecularColour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));
	test->setSpotlightInnerAngle(Ogre::Degree(25));
	test->setSpotlightOuterAngle(Ogre::Degree(45));
	test->setDirection(Ogre::Vector3(0,0,-1));
	Ogre::SceneNode* light = _scene->getRootSceneNode()->createChildSceneNode("testLight");
	light->attachObject(test);
	light->setPosition(0.0f,.0f,-.50f);
	light->getParent()->removeChild(light);

	DamageInterface* damage = new DamageInterface();

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
	_camera->setPosition(Ogre::Vector3(0,1.9f,0));
	//_camera->setPosition(Ogre::Vector3(0.0f,5.0f,0.0f));
	_camera->setNearClipDistance(.001f);
	_camera->setFarClipDistance(1000.0f);
	_camera->lookAt(0,1.9f,0);

	//player setup
	_player.reset(new Player());
	
	//set the camera aspect ratio
	_camera->setAspectRatio(16.0f/9.0f);

	///Ogre::Camera* testCamera = _scene->createCamera("ARENA_TUT_TESTCAM");
	//testCamera->lookAt(0,1.9f,0);
	//testCamera->setPosition(Ogre::Vector3(0.0f,1.9f,0.0f));

	//let's try out the character controller
	_controller.reset(new CharacterController(_camera,Ogre::Vector3(1.0f,1.9f,5.0f),Ogre::Vector3(0.0,0.0,-5.0f),_physics->getWorld(),Graphics ) );
	//since we're using the character controller, should also lock the mouse.
	Input->setMouseLock(true);

	_controller->getNode()->addChild(light);

	//_camera->setAutoTracking(true,_controller->getNode());

	//let's setup the EWS system
	_ews.reset(new EWSManager(_scene));

	LevelData::LevelParser parser;
	parser.setFile("resource\\models\\mapscapeTEST\\zonedoor_scriptTest.ent");
	parser.parseDoors(&_doors);
	parser.parseLights(&_lights);
	parser.parseTriggers(&_triggers,_scene->getRootSceneNode());

	std::cout << "Parser finished" << std::endl;

	_setupLights(Graphics,_scene);
	OgreBulletPair level = _pairs.at(0);
	_setupDoors(level,_scene,_physics.get(),Graphics);

	std::cout << "Level elements setup" << std::endl;

	//testing out the M9SE
	EquippableObject equipObj = GameManager::createEquippable(_scene,"resource\\xml\\weapon_m9se.xml",Graphics,Sound,true);
	_player->addEquippableObject(equipObj);
	_player->Setup("TEST",Graphics,_controller->getNode(),damage);
	
	std::cout << "Player setup" << std::endl;

	Ogre::Entity* levelEnt = static_cast<Ogre::Entity*>(_pairs.begin()->ogreNode->getAttachedObject(0));
	InputGeometry levelGeometry(levelEnt);

	RecastConfiguration params;
	params.setAgentHeight(2.5f);
	params.setAgentRadius(.2f);
	params.setCellSize(.3f);

	_recast.reset(new RecastInterface(_scene,params));
	_recast->getRecastConfig().walkableRadius = static_cast<int>(.2f);
	_recast->buildNavMesh(&levelGeometry);
	_recast->exportPolygonMeshToObj("ARENATUTORIAL_RECAST_MESH.obj");

	rcdtConfig config = _recast->getConfigurations();
	//rcdtConfig config(&_recast->getRecastConfig(),&_recast->getRecastBuildConfiguration());
	//config.recastConfig = &_recast->getRecastConfig();
	//config.userConfig = &_recast->getRecastBuildConfiguration();

	_detour.reset(new DetourInterface(_recast->getPolyMesh(),_recast->getDetailMesh(),config));

	_crowd.reset(new CrowdManager(_detour.get(),&config));

	auto npcList = list("resource\\xml\\lists\\arenalocker_npc_list.xml");
	for(auto itr = npcList->file().begin(); itr != npcList->file().end(); ++itr)
	{
		characterobject_t* obj = characterObject(*itr).release();
		Ogre::SceneNode* node = GameManager::createCharacterObject(_scene,obj,Graphics);

		if(obj->type() == "NPC")
		{
			NPCCharacter* npc = new NPCCharacter(obj->name(),obj->scriptName(),node,_crowd.get());
			npc->setMaxSpeed(.9f);
			_npcs.push_back(npc);
			LuaManager::getSingleton().addEntity(npc->getName(),npc);
		}

		delete obj;
	}

	_pauseMenu.reset(new PauseMenu(State::GAME_ARENA));
	_pauseMenu->Setup(Input,Graphics,Gui,Sound);

	//physics debug drawer.
	//_physics->setDebugDrawer(new CDebugDraw(_scene,_physics->getWorld()));
}

int ArenaTutorial::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_returnValue = State::GAME_LOCKER;
	_stateShutdown=false;

	OgreTransform playerTransform;

	bool polySwitch = false;

	LuaManager::getSingleton().addDataPointer("playerPosition",static_cast<void*>(&playerTransform.position));
	
	float time = Graphics->getTimer()->getMilliseconds();
	_oldTime = time;
	bool paused = false;
	//while the escape key isn't pressed and the state isn't told to shutdown.
	while(!_stateShutdown)
	{
		//std::cout << "BatchCount:" << Graphics->getRenderWindow()->getBatchCount() << std::endl;
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

		_crowd->updateTick(_deltaTime / 1000.0f);

		for(auto itr = _npcs.begin(); itr != _npcs.end(); ++itr)
		{
			(*itr)->update(_deltaTime);
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
					//_camera->setPolygonMode(Ogre::PM_WIREFRAME);
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

		//std::cout << _controller->getNode()->getPosition() << std::endl;
		//std::cout << static_cast<Ogre::Camera*>(_controller->getNode()->getAttachedObject(0))->getPosition() << std::endl;

		//Update the EWS system
		_ews->Update(static_cast<int>(time),
					 Input->isCFGKeyPressed(InputManager::ENVWARNSYS),
					 _player->getPlayerData(),
					 playerTransform);

		//GameManager::RenderScene(Graphics,_view);

		//True indicates success, so react on if it doesn't react properly
		if(!GameManager::UpdateManagers(Graphics,_physics.get(),_deltaTime))
		{
			//shuts down appstate
			_stateShutdown = true;
		}

		_updateLights();
		_updateTriggers(playerTransform,static_cast<int>(_deltaTime));
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

	LuaManager::getSingleton().purgeData();
	LuaManager::getSingleton().purgeEntities();

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

	std::for_each(_npcs.begin(),_npcs.end(),[] (NPCCharacter* npc) {
		delete npc;
	});

	delete _shadowCompositorListener;
	_scene->removeListener(_shadowListener);
	delete _shadowListener;

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
		//switch((*itr)->getTriggerType())
		//{
		//case LevelData::ENTITY:
		//	static_cast<LevelData::EntityTrigger*>((*itr).get())->update();
		//	break;
		//case LevelData::TIME:
		//	static_cast<LevelData::TimeTrigger*>((*itr).get())->update(currentTime);
		//	break;
		//case LevelData::GLOBAL:
		//case LevelData::PLAYER:
		//	//static_cast<LevelData::GlobalTrigger*>((*itr).get())->update(playerTransform,currentTime);
		//	//static_cast<LevelData::PlayerTrigger*>((*itr).get())->update(playerTransform);
		//	(*itr)->update(playerTransform,currentTime);
		//	break;
		//};
		(*itr)->update(playerTransform,currentTime);
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