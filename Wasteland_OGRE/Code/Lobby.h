#include "StdAfx.h"

#ifndef _LOBBY_H_
#define _LOBBY_H_

#include "State.h"

//need the pause menu
#define _PAUSE_ONLY_
#include "menu\Menu.h"
#undef _PAUSE_ONLY_

class Lobby : public State
{
public:
	Lobby();

	void Setup(InputManager* Input = nullptr, GraphicsManager* Graphics = nullptr, GUIManager* Gui = nullptr, SoundManager* Sound = nullptr);

	int Run(InputManager* Input = nullptr, GraphicsManager* Graphics = nullptr, GUIManager* Gui = nullptr,SoundManager* Sound = nullptr);

	void Shutdown(InputManager* Input = nullptr, GraphicsManager* Graphics = nullptr, GUIManager* Gui = nullptr, SoundManager* Sound = nullptr);

private:
	Ogre::Viewport* _view;
	Ogre::SceneManager* _scene;
	Ogre::Camera* _camera;

	std::unique_ptr<PauseMenu> _pauseMenu;
};

#endif