#include "StateManager.h"

template<> StateManager* Ogre::Singleton<StateManager>::ms_Singleton = 0;

StateManager::StateManager()
{
	//Get the map setup with state pointers
	//Make sure these pointers don't get lost!

	//ToDo: Change the init to reflect the varying state classes.
	//_States.insert(std::make_pair(MENU,new State()));
}

void StateManager::Setup()
{
	//Not sure if I need this step at the moment. We'll see.
}

void StateManager::Run()
{
	int curState=MENU;

	while(curState!=END)
	{
		_States[curState]->Setup();
		curState=_States[curState]->Run();
		_States[curState]->Shutdown();
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