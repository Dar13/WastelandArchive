#include "StdAfx.h"

#include "StateManager.h"

template<> StateManager* Ogre::Singleton<StateManager>::ms_Singleton = 0;

StateManager::StateManager()
{
	//Get the map setup with state pointers
	//Make sure these pointers don't get lost!

	//ToDo: Change the init to reflect the varying state classes.
	_States.insert(std::make_pair(GAME_ARENA,new ArenaTutorial()));
}

void StateManager::Setup(OISManager* input,OgreManager* graphics)
{
	//Not sure if I need this step at the moment. We'll see.
	//looks like I need it
	_Input = input;
	_Graphics = graphics;
}

void StateManager::Run()
{
	//int curState=INTRO; //true start value
	int curState=GAME_ARENA; //will really start at INTRO
	int oldState=curState;

	while(curState!=END)
	{
		if(_States[curState] != NULL)
		{
			//setting up the current state
			_States[curState]->Setup(_Input,_Graphics);
			//saving the state, so we can do clean up later
			oldState=curState;
			//run the state, gets next state from run method
			curState=_States[curState]->Run(_Input,_Graphics);
			//clean up old state(really the current state but eh)
			_States[oldState]->Shutdown(_Input,_Graphics);
			delete _States[oldState];
			//make sure they know it's deleted
			_States[oldState] = NULL;
		}
		else
		{
			//remake state
			switch(curState)
			{
			case INTRO:
				//_States[curState] = new
				break;
			case MENU:
				//_States[curState] = new
				break;
			case GAME_ARENA:
				_States[curState] = new ArenaTutorial();
				break;
			case GAME_LOCKER:
				//_States[curState] = new ArenaLocker();
				break;
			case GAME_LOBBY:
				//_States[curState] = new Lobby();
				break;
			case GAME_OFFICE:
				//_States[curState] = new Office();
				break;
			case GAME_ROOF:
				//_States[curState] = new Roof();
				break;
			case CREDITS:
				//_States[curState] = new Credits
				break;
			case END:

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
	for(int i=MENU; i<=END; i++)
	{
		State* t = _States[i];
		delete t;
		_States.erase(i);
	}
}