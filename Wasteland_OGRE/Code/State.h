#ifndef _STATE_H_
#define _STATE_H_

#include "OISManager.h"

enum ApplicationStates
{
	INTRO = 0,
	MENU,
	GAME_ARENA,
	GAME_LOCKER,
	GAME_LOBBY,
	GAME_OFFICE,
	GAME_ROOF,
	CREDITS,
	END
};

/*!
\brief An abstract class that sets the framework for the states the application will go through.

*/

class State
{
public:
	//! Sets up the state.
	virtual void Setup(OISManager* input) = 0; 
	//!Runs the game logic for the state.
	virtual int Run(OISManager* input) = 0; 
	//!Cleans up the state.
	virtual void Shutdown(OISManager* input) = 0; 
private:
	//!A boolean to tell if the state should shutdown.
	bool _stateShutdown;
};

#endif