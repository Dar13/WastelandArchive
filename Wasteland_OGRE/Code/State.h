#ifndef _STATE_H_
#define _STATE_H_

enum ApplicationStates
{
	MENU = 0,
	INTRO,
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
	State();
	//! Sets up the state.
	virtual void Setup() = 0; 
	//!Runs the game logic for the state.
	virtual int Run() = 0; 
	//!Cleans up the state.
	virtual void Shutdown() = 0; 
private:
	//!A boolean to tell if the state should shutdown.
	bool _stateShutdown;
};

#endif