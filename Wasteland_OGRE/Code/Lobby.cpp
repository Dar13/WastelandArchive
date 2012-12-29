#include "StdAfx.h"

#include "Lobby.h"
#include "GameManager.h"

Lobby::Lobby()
	: _view(nullptr),
	  _scene(nullptr),
	  _camera(nullptr)
{
	_stateShutdown = false;
	_deltaTime = 0.0f;
	_oldTime = 0.0f;
	_returnValue = State::END;
}

void Lobby::Setup(InputManager* Input, GraphicsManager* Graphics, GUIManager* Gui, SoundManager* Sound)
{
	_scene = Graphics->getRoot()->createSceneManager(Ogre::ST_INTERIOR,"LobbySM");

	_camera = _scene->createCamera("LobbyCamera");

	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue::Black);

	_pauseMenu.reset(new PauseMenu(State::GAME_LOBBY));
	_pauseMenu->Setup(Input,Graphics,Gui,Sound);

	return;
}

int Lobby::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	float time = Graphics->getCurrentTimeMs();
	_oldTime = time;

	bool paused = false;

	while(!_stateShutdown)
	{
		time = Graphics->getCurrentTimeMs();
		_deltaTime = time - _oldTime;
		_oldTime = time;

		if(paused)
		{
			paused = false;
			_deltaTime = 16.6f;
		}

		if(!GameManager::UpdateManagers(Graphics,nullptr,_deltaTime))
		{
			_stateShutdown = true;
		}

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

	return _returnValue;
}

void Lobby::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_pauseMenu->Shutdown(Input,Graphics,Gui,Sound);

	Graphics->getRenderWindow()->removeAllViewports();

	Graphics->cleanAndDestroySceneManager(_scene);
}