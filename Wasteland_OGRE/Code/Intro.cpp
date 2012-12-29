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

	_fader = new ScreenFader("Overlays/FadeInOut","Overlays/FadeMaterial",&_faderCallback);
	_faderUpdater.setFader(_fader);
	Graphics->getRoot()->addFrameListener(&_faderUpdater);

	return;
}

int Introduction::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	//Ogre::Material* mat = static_cast<Ogre::Entity*>(_logoSceneNode->getAttachedObject(0))->getSubEntity(1)->getMaterial().get();

	_fader->startFadeIn(2.0);
	_fader->setHideOverlayAfterFadeOut(false);
	bool fadeIn = true;
	bool fadeOut = false;

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

		if(duration > 7000 && !fadeOut)
		{
			_stateShutdown = true;
		}

		if(_stateShutdown && !fadeOut)
		{
			fadeOut = true;
			_fader->startFadeOut(2.0);
			_stateShutdown = false;
		}

		if(fadeOut)
		{
			if(_faderCallback.isFadeFinished())
			{
				_stateShutdown = true;
			}
		}
	}


	//return _returnValue;
	return State::GAME_LOCKER;
}

void Introduction::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_scene->getRootSceneNode()->removeAndDestroyAllChildren();

	Graphics->Render();
	
	Graphics->getRenderWindow()->removeAllViewports();

	_scene->destroyCamera(_camera);

	Graphics->getRoot()->destroySceneManager(_scene);

	delete _fader;
	Graphics->getRoot()->removeFrameListener(&_faderUpdater);

	return;
}

void Intro_FaderCallback::fadeInCallback()
{
	_finished = true;
}

void Intro_FaderCallback::fadeOutCallback()
{
	_finished = true;
}

void Intro_FaderCallback::updateFade(double progress,double currentTime,int fadeOp)
{
	_finished = false;
}