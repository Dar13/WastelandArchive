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
	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));

	_rootNode = _scene->getRootSceneNode();

	_physics.reset(new PhysicsManager());
	_physics->Setup();
	_physics->setGravity(btVector3(0.0f,-9.8f,0.0f));

	
}

int ArenaLocker::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{

	while(!_stateShutdown)
	{
		//checks for escapekey press and updates input manager.
		_stateShutdown = Input->Update(true);
	}
}

void ArenaLocker::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{

}