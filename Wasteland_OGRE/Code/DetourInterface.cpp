#include "StdAfx.h"

#include "DetourInterface.h"
#include <Recast.h>

DetourInterface::DetourInterface(rcPolyMesh* polyMesh)
{
	unsigned char* navData = 0;
	int navDataSize = 0;

	for(int i = 0; i < polyMesh->npolys; ++i)
	{
		if(polyMesh->areas[i] == RC_WALKABLE_AREA)
		{
			polyMesh->areas[i] == DT_PA_GROUND;
			polyMesh->flags[i] == DT_PF_WALK;
		}
	}

	//to be continued.
}