#include "StdAfx.h"

#ifndef _AI_INCLUDE_H_
#define _AI_INCLUDE_H_

namespace AI
{
	enum BehaviorCodes
	{
		BHV_IDLE = 0,
		BHV_WANDER,
		BHV_TALK,
		BHV_MOVE,
		BHV_FOLLOW
	};

	enum ActionCodes
	{
		ACT_IDLE = 0,
		ACT_SHOOT,
		ACT_RELOAD,
		ACT_CHANGEWEP
	};
}

#endif