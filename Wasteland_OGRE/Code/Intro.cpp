#include "StdAfx.h"

#include "Intro.h"
#include "GameManager.h"

Introduction::Introduction()
{	
	_stateShutdown = false;
	_deltaTime = 0.0f;
	_oldTime = 0.0f;
	_returnValue = State::MENU;
}

void Introduction::Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_scene = Graphics->getRoot()->createSceneManager(Ogre::ST_INTERIOR,"IntroductionSM");

	//load the scene...
	_camera = _scene->createCamera("IntroductionCamera");

	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0.1f,0.1f,0.1f,1.0f));

	Ogre::Entity* logoEntity = _scene->createEntity("logoPipesEntity","intro//logo_pipes.mesh","Models");
	logoEntity->setMaterialLodBias(2.0f,0);

	Ogre::Entity* logoTextEntity = _scene->createEntity("logoTextEntity","intro//logo_text.mesh","Models");
	logoTextEntity->setMaterialLodBias(2.0f,0);

	Ogre::Entity* logoUnderneathEntity = _scene->createEntity("logoUnderEntity","intro//logo_underneath.mesh","Models");
	logoUnderneathEntity->setMaterialLodBias(2.0f,0);

	_logoSceneNode = _scene->getRootSceneNode()->createChildSceneNode("logoSceneNode");
	_logoSceneNode->attachObject(logoTextEntity);
	_logoSceneNode->attachObject(logoEntity);
	_logoSceneNode->attachObject(logoUnderneathEntity);

	_camera->setPosition(-17.5f,-5.0f,95.0f);
	//_camera->lookAt(_camera->getPosition().x,_camera->getPosition().y,-100.0f);
	_camera->setFarClipDistance(2000.0f);
	_camera->setNearClipDistance(1.0f);

	return;
}

int Introduction::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	//Ogre::Material* mat = static_cast<Ogre::Entity*>(_logoSceneNode->getAttachedObject(0))->getSubEntity(1)->getMaterial().get();

	float duration = 0;
	_oldTime = Graphics->getTimer()->getMilliseconds();
	float curTime = _oldTime;
	while(!_stateShutdown)
	{
		curTime = Graphics->getTimer()->getMilliseconds();
		_deltaTime = curTime - _oldTime;
		_oldTime = curTime;

		duration += _deltaTime;

		GameManager::UpdateManagers(Graphics,nullptr,_deltaTime);

		if(duration > 5000)
		{
			_stateShutdown = true;
		}
	}


	//return _returnValue;
	return State::END;
}

void Introduction::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	//any fancy destruction goes up here.
	Graphics->getRenderWindow()->removeAllViewports();

	//primary way of cleaning up the Ogre scene.
	_scene->clearScene();

	return;
}