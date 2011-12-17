#include "StdAfx.h"

#ifndef _ARENA_LOCKER_H_
#define _ARENA_LOCKER_H_

#include "State.h"

/*! \brief **NOT FULLY IMPLEMENTED** Application state that is the Locker Room scene in Wasteland.

Is not a truly interactive scene, but shows an important plot point.
*/

class ArenaLocker : public State
{
public:
	//! Constructor, initializes all class members.
	ArenaLocker();

	//! State-specific setup.
	void Setup();
	//! Runs the state code.
	int Run();
	//! Cleans up the state, releases all memory/assets allocated to it.
	void Shutdown();

private:
	//tells the state to shutdown or not.
	bool _stateShutdown;
};

#endif