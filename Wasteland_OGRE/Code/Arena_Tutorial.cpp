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
}

void ArenaTutorial::Setup()
{
	_scene = OgreManager::getSingleton().getRoot()->createSceneManager(Ogre::ST_INTERIOR,"arenaTut");

	_camera = _scene->createCamera("arenaTutCam");
	_view = OgreManager::getSingleton().getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));

	_rootNode = _scene->getRootSceneNode();

	//some special physics setup
	BulletManager::getSingleton().setGravity(btVector3(0,-9.8f,0));
	
	//Loading the level through the GameManager(takes care of Bullet initialization,etc)
	object_t* level = object("resource\\xml\\test.xml").release();
	OgreBulletPair levelPair = GameManager::getSingleton().createObject(_scene,level);
	//levelPair.ogreNode->setVisible(false);
	levelPair.btBody->setCcdSweptSphereRadius(0.1f);
	delete level;

	object_t* sphere = object("resource\\xml\\test_sphere.xml").release();
	OgreBulletPair spherePair = GameManager::getSingleton().createObject(_scene,sphere);
	delete sphere;

	object_t* box = object("resource\\xml\\test_box.xml").release();
	OgreBulletPair boxPair = GameManager::getSingleton().createObject(_scene,box);
	delete box;

	_pairs.push_back(spherePair);
	_pairs.push_back(boxPair);

	_camera->setPosition(Ogre::Vector3(-1,1.9f,0));
	_camera->setNearClipDistance(.001);
	_camera->setFarClipDistance(1000);
	_camera->lookAt(0,1.8,0);
	
	//set the camera aspect ratio
	_camera->setAspectRatio(4.0f/3.0f);

	//let's try out the character controller
	GameManager::getSingleton().createCharacterController(_camera,_camera->getPosition());
	//since we're using the character controller, should also lock the mouse.
	OISManager::getSingleton().setMouseLock(true);

	DebugPrint::getSingleton().Setup(_scene);
	//GameManager::getSingleton().useDebugDrawer(_scene);
}

int ArenaTutorial::Run()
{
	_stateShutdown=false;
	Ogre::Node* tmp = _scene->getRootSceneNode()->getChild("nodetestSphere");

	//while the escape key isn't pressed and the state isn't told to shutdown.
	while(!_stateShutdown)
	{
		//True indicates success, so react on if it doesn't react properly
		if(!GameManager::getSingleton().UpdateManagers())
			_stateShutdown = true;
	}

	//no matter what, end the program after this state.
	return END;
}

void ArenaTutorial::Shutdown()
{
	//undo what I set in OIS
	OISManager::getSingleton().setMouseLock(false);

	//clean up what you initialized in the Setup() function.
	OgreManager::getSingleton().getRenderWindow()->removeAllViewports();
	
	//Destroy the camera, since we've taken care of the viewport.
	_scene->destroyCamera(_camera);
	//Destroy all SceneNodes, Entites, etc.
	_scene->clearScene();

	//Set to null.
	_rootNode=0;

	//Clear the vectors
	_nodes.clear();
	_entities.clear();

	//Has to be cleaned after every Setup, in the same app-state.
	DebugPrint::getSingleton().Clean();

	//Destroy the scene manager.
	OgreManager::getSingleton().getRoot()->destroySceneManager(_scene);
}