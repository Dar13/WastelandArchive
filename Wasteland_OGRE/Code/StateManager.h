#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_

//Base state header, just for the heck of it.
#include "State.h"

#include <OgreSingleton.h>

#include <map>

class StateManager : public Ogre::Singleton<StateManager>
{
public:
	StateManager();
	~StateManager(){Shutdown();}

	//StateManager status functions.
	void Setup();
	void Run();
	void Shutdown();

	//Additional functions
	State* getState(int state){return _States[state];}

private:
	StateManager(const StateManager&);
	StateManager& operator= (const StateManager&);

	std::map<int,State*> _States;
};

#endif