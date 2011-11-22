#ifndef _ARENA_LOCKER_H_
#define _ARENA_LOCKER_H_

#include "State.h"

class ArenaLocker : public State
{
public:
	//Initial setup
	ArenaLocker();

	//Actions for this state.
	void Setup();
	int Run();
	void Shutdown();

private:
	//tells the state to shutdown or not.
	bool _stateShutdown;
};

#endif