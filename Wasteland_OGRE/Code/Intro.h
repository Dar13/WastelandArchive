#include "StdAfx.h"

#ifndef _INTRO_H_
#define _INTRO_H_

#include "State.h"

#define _SCREEN_FADER_ONLY_
#include "GraphicsManager.h"
#undef _SCREEN_FADER_ONLY

class Intro_FaderCallback : public ScreenFaderCallback
{
public:
	void fadeInCallback();
	void fadeOutCallback();

	void updateFade(double progress,double currentTime, int fadeOp);

	bool isFadeFinished() { return _finished; }
private:
	bool _finished;
};

class Introduction : public State
{
public:
	//! Constructor.
	Introduction();

	//! Setup.
	void Setup(InputManager* Input = nullptr,GraphicsManager* Graphics = nullptr,GUIManager* Gui = nullptr,SoundManager* Sound = nullptr);
	//! Runs state.
	int Run(InputManager* Input = nullptr,GraphicsManager* Graphics = nullptr,GUIManager* Gui = nullptr,SoundManager* Sound = nullptr);
	//! Shuts down state.
	void Shutdown(InputManager* Input = nullptr,GraphicsManager* Graphics = nullptr,GUIManager* Gui = nullptr,SoundManager * Sound = nullptr);

private:
	ScreenFader* _fader;
	Intro_FaderCallback _faderCallback;
	ScreenFaderUpdater _faderUpdater;

	Ogre::Viewport* _view;
	Ogre::SceneManager* _scene;
	Ogre::Camera* _camera;

	Ogre::SceneNode* _logoSceneNode;

};

#endif