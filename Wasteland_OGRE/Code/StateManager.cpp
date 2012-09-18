#include "StdAfx.h"

#include "StateManager.h"

template<> StateManager* Ogre::Singleton<StateManager>::ms_Singleton = 0;

StateManager::StateManager()
{
	//Get the map setup with state pointers
	//Make sure these pointers don't get lost!

	//ToDo: Change the init to reflect the varying state classes.
	_States[State::INTRO] = nullptr;
	_States[State::MENU] = new MainMenu();
	_States[State::GAME_ARENA] = new ArenaTutorial();
	_States[State::GAME_LOCKER] = new ArenaLocker();
	_States[State::GAME_LOBBY] = nullptr;
	_States[State::GAME_OFFICE] = nullptr;
	_States[State::GAME_ROOF] = nullptr;
	_States[State::CREDITS] = nullptr;

	_Input = nullptr;
	_Graphics = nullptr;
	_Gui = nullptr;
	_Sound = nullptr;
}

void StateManager::Setup(InputManager* inputManager,GraphicsManager* graphicsManager,GUIManager* guiManager,SoundManager* soundManager)
{
	//Not sure if I need this step at the moment. We'll see.
	//looks like I need it
	_Input = inputManager;
	_Graphics = graphicsManager;
	_Gui = guiManager;
	_Sound = soundManager;
}

void StateManager::Run()
{
	//int curState=INTRO; //true start value
	int curState=State::MENU; //will really start at INTRO
	int oldState=curState;

	while(curState!=State::END)
	{
		if(_States[curState] != NULL)
		{
			//setting up the current state
			_States[curState]->Setup(_Input,_Graphics,_Gui,_Sound);
			//saving the state, so we can do clean up later
			oldState=curState;
			//run the state, gets next state from run method
			curState=_States[curState]->Run(_Input,_Graphics,_Gui,_Sound);
			//clean up old state(really the current state but eh)
			_States[oldState]->Shutdown(_Input,_Graphics,_Gui,_Sound);
			delete _States[oldState];
			//make sure they know it's deleted
			_States[oldState] = NULL;
		}
		else
		{
			//remake state
			switch(curState)
			{
			case State::INTRO:
				//_States[curState] = new
				break;
			case State::MENU:
				_States[curState] = new MainMenu();
				break;
			case State::GAME_ARENA:
				_States[curState] = new ArenaTutorial();
				break;
			case State::GAME_LOCKER:
				_States[curState] = new ArenaLocker();
				break;
				/*
			case State::GAME_LOBBY:
				//_States[curState] = new Lobby();
				break;
			case State::GAME_OFFICE:
				//_States[curState] = new Office();
				break;
			case State::GAME_ROOF:
				//_States[curState] = new Roof();
				break;
			case State::CREDITS:
				//_States[curState] = new Credits
				break;
				*/
			case State::GAME_LOBBY:
			case State::GAME_OFFICE:
			case State::GAME_ROOF:
			case State::CREDITS:
				curState = State::END;
				break;
			};
		}
	}

	return;
}

void StateManager::Shutdown()
{
	//need to delete all the states.
	//This may or may not work. We'll see.
	for(int i=State::INTRO; i<=State::END; i++)
	{
		State* t = _States[i];
		delete t;
		_States.erase(i);
	}
}