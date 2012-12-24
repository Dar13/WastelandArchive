#include "StdAfx.h"

#ifndef _INTRO_H_
#define _INTRO_H_

#include "State.h"

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
	Ogre::Viewport* _view;
	Ogre::SceneManager* _scene;
	Ogre::Camera* _camera;

	Ogre::SceneNode* _logoSceneNode;

};

#endif