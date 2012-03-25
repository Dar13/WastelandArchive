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

void StateManager::Setup(OISManager* ois)
{
	//Not sure if I need this step at the moment. We'll see.
	//looks like I need it
	_OIS = ois;
}

void StateManager::Run()
{
	//int curState=INTRO; //true start value
	int curState=GAME_ARENA; //will really start at INTRO
	int oldState=curState;

	while(curState!=END)
	{
		_States[curState]->Setup(_OIS);
		oldState=curState;
		curState=_States[curState]->Run(_OIS);
		_States[oldState]->Shutdown(_OIS);
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