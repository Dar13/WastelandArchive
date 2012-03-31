#include "StdAfx.h"

#include "Arena_Tutorial.h"
#include "GameManager.h"
#include "debug\print.h"

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

void ArenaTutorial::Setup(OISManager* Input,OgreManager* Graphics)
{
	_scene = Graphics->getRoot()->createSceneManager(Ogre::ST_INTERIOR,"arenaTut");

	_camera = _scene->createCamera("arenaTutCam");
	_camera->setAspectRatio(16.0f/9.0f);
	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));

	_rootNode = _scene->getRootSceneNode();

	//physics setup
	_physics.reset(new BulletManager() );
	_physics->Setup();
	_physics->setGravity(btVector3(0.0f,-9.8f,0.0f));
	
	//Loading the level through the GameManager(takes care of Bullet initialization,etc)
	/*
	object_t* level = object("resource\\xml\\test.xml").release();
	OgreBulletPair levelPair = GameManager::getSingleton().createObject(_scene,level);
	delete level;

	object_t* sphere = object("resource\\xml\\test_sphere.xml").release();
	OgreBulletPair spherePair = GameManager::getSingleton().createObject(_scene,sphere);
	delete sphere;

	object_t* box = object("resource\\xml\\test_box.xml").release();
	OgreBulletPair boxPair = GameManager::getSingleton().createObject(_scene,box);
	delete box;

	object_t* test_barrier = object("resource\\xml\\test_enem.xml").release();
	OgreBulletPair barPair = GameManager::getSingleton().createObject(_scene,test_barrier);
	delete test_barrier;
	
	_pairs.push_back(spherePair);
	_pairs.push_back(boxPair);
	_pairs.push_back(barPair);
	*/

	//using a list instead of hardcoded files
	std::auto_ptr<list_t> objList = list("resource\\xml\\arena_list.xml");
	for(list_t::file_const_iterator itr = objList.get()->file().begin(); itr != objList.get()->file().end(); ++itr)
	{
		std::string tmp = (*itr);
		_pairs.push_back(GameManager::createObject(_scene,tmp,_physics.get(),Graphics));
	}

	_camera->setPosition(Ogre::Vector3(-1,1.9f,0));
	_camera->setNearClipDistance(.001f);
	_camera->setFarClipDistance(1000.0f);
	_camera->lookAt(0,1.8f,0);

	//_camera->setPolygonMode(Ogre::PM_WIREFRAME);
	
	//set the camera aspect ratio
	_camera->setAspectRatio(4.0f/3.0f);

	//let's try out the character controller
	_controller.reset(new CharacterController(_camera,_camera->getPosition(),_physics->getWorld() ) );
	//since we're using the character controller, should also lock the mouse.
	//OISManager::getSingleton().setMouseLock(true);
	Input->setMouseLock(true);

	//DebugPrint::getSingleton().Setup(_scene);
	//GameManager::getSingleton().useDebugDrawer(_scene);

	//let's setup the EWS system
	_ews.reset(new EWSManager(_scene));
}

int ArenaTutorial::Run(OISManager* Input,OgreManager* Graphics)
{
	_stateShutdown=false;
	Ogre::Node* tmp = _scene->getRootSceneNode()->getChild("nodetestSphere");

	float time;
	//while the escape key isn't pressed and the state isn't told to shutdown.
	while(!_stateShutdown)
	{
		//current time
		time = static_cast<float>(Graphics->getTimer()->getMilliseconds());
		//getting the delta time
		_deltaTime = time - _oldTime;
		//setting the old time
		_oldTime = time;

		//should the state shutdown based on player input?
		_stateShutdown = Input->Update(true);

		//testing out something
		if(Input->isMBPressed(OIS::MB_Right))
		{
			
		}

		//Update the character controller
		_controller->update(_deltaTime,Input);

		//Update the EWS system
		_ews->Update(100,static_cast<int>(time));

		//True indicates success, so react on if it doesn't react properly
		if(!GameManager::UpdateManagers(Graphics,_physics.get(),_deltaTime))
			_stateShutdown = true;

		//EWS updates in GameManager::UpdateManagers()
		//EWSManager::getSingletonPtr()->Update(50,0);
	}

	//no matter what, end the program after this state. **TESTING ONLY**
	return END;
}

//clean-up of state
void ArenaTutorial::Shutdown(OISManager* Input,OgreManager* Graphics)
{
	//Needs to reset first.
	//EWSManager::getSingleton().Reset();

	//undo what I set in OIS
	Input->setMouseLock(false);

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

	//cleaning up state-specific pointers.
	//since all pointers are auto_ptr members, then they will be deleted upon class destruction.

	//Has to be cleaned after every Setup, in the same app-state.
	DebugPrint::getSingleton().Clean();

	//Destroy the scene manager.
	Graphics->getRoot()->destroySceneManager(_scene);
}