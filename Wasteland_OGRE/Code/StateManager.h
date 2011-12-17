#include "StdAfx.h"

#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_

//Base state header, just for the heck of it.
#include "State.h"

//actual states
#include "ArenaALL.h"

/*!
\brief The manager class for all of the application states.

Controls how the application progresses through its states.
Will NOT use any other manager's functions and/or variables.
*/

class StateManager : public Ogre::Singleton<StateManager>
{
public:
	//! Initializes the pointers for the applications states.
	StateManager();
	//! Calls the shutdown function.
	~StateManager(){Shutdown();}

	//! Sets up the StateManager.
	void Setup();
	//! Runs the StateManager, until the End state is reached.
	void Run();
	//! Shutsdown and cleans up the StateManager. Is called in the destructor.
	void Shutdown();

	//! Gets the state pointer that relates to the given application state.
	State* getState(int state){return _States[state];}

private:
	StateManager(const StateManager&);
	StateManager& operator= (const StateManager&);

	std::map<int,State*> _States;
};

#endif