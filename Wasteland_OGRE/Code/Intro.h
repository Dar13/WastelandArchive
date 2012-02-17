#include "StdAfx.h"

#ifndef _INTRO_H_
#define _INTRO_H_

#include "State.h"

class Introduction : public State
{
public:
	//! Constructor.
	Introduction();

	//! Setup.
	void Setup();
	//! Runs state.
	int Run();
	//! Shuts down state.
	void Shutdown();

private:
	bool _stateShutdown;

	//not sure what else I'll need.
};

#endif