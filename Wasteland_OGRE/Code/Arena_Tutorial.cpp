#include "StdAfx.h"

#include "Arena_Tutorial.h"
#include "GameManager.h"

ArenaTutorial::ArenaTutorial()
{
	_camera = 0;
	_scene = 0;
	_rootNode = 0;
}

void ArenaTutorial::Setup()
{
	_scene = OgreManager::getSingleton().getRoot()->createSceneManager(Ogre::ST_INTERIOR,"arenaTut");

	_camera = _scene->createCamera("arenaTutCam");
	_view = OgreManager::getSingleton().getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));

	_rootNode = _scene->getRootSceneNode();

	//some special physics setup
	BulletManager::getSingleton().setGravity(btVector3(0,-20.0f,0));
	
	//Loading the level through the GameManager(takes care of Bullet initialization,etc)
	object_t* level = object("resource\\xml\\test.xml").release();
	OgreBulletPair levelPair = GameManager::getSingleton().createObject(_scene,level);

	object_t* tmpObj = NULL;
	list_t* tmpList = list("resource\\xml\\list.xml").release();
	for(list_t::file_const_iterator itr = tmpList->file().begin(); itr != tmpList->file().end(); ++itr)
	{
		tmpObj = object((*itr)).release();
		OgreBulletPair curVal = GameManager::getSingleton().createObject(_scene,tmpObj);
		_pairs.push_back(curVal);
		delete tmpObj;
	}
	delete tmpList;

	_camera->setPosition(Ogre::Vector3(300,60,0));
	GameManager::getSingleton().createCharacterController(_camera,_camera->getPosition());
	_camera->lookAt(0,0,0);
	//set the camera aspect ratio
	_camera->setAspectRatio(4.0f/3.0f);

}

int ArenaTutorial::Run()
{
	_stateShutdown=false;

	Ogre::SceneNode* enem = _scene->getSceneNode("nodeEnemy");
	Ogre::Entity* enemEnt = (Ogre::Entity*)enem->getAttachedObject("entEnemy");
	Ogre::AnimationState* enemAnim = enemEnt->getAnimationState("MoveFast");
	enemAnim->setLoop(true);
	enemAnim->setEnabled(true);
	OgreManager::getSingleton().addAnimationState(enemAnim);

	Ogre::Node* tmp = _scene->getRootSceneNode()->getChild("nodeEnemy");

	//while the escape key isn't pressed and the state isn't told to shutdown.
	while(!_stateShutdown)
	{
		//True indicates success, so react on if it doesn't react properly
		if(!GameManager::getSingleton().UpdateManagers())
			_stateShutdown = true;

		//point camera at scene node
		//_camera->lookAt(tmp->getPosition());
	}

	//no matter what, end the program after this state.
	return END;
}

void ArenaTutorial::Shutdown()
{
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

	//Destroy the scene manager.
	OgreManager::getSingleton().getRoot()->destroySceneManager(_scene);
}