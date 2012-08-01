#include "StdAfx.h"

#include "DetourInterface.h"
#include <Recast.h>
#include "Utility.h"

DetourInterface::DetourInterface(rcPolyMesh* polyMesh,rcPolyMeshDetail* detailMesh,rcdtConfig& config)
	: _navMesh(nullptr),
	  _navQuery(nullptr),
	  _isMeshBuilt(false)
{
	detourCleanup();

	unsigned char* navData = 0;
	int navDataSize = 0;

	if(config.recastConfig->maxVertsPerPoly > DT_VERTS_PER_POLYGON)
	{
		//Not needed, is set to false anyways.
		//_isMeshBuilt = false;
		return;
	}

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Detour - Stage 1" << std::endl;
#endif

	for(int i = 0; i < polyMesh->npolys; ++i)
	{
		if(polyMesh->areas[i] == RC_WALKABLE_AREA)
		{
			polyMesh->areas[i] = DT_PA_GROUND;
			polyMesh->flags[i] = DT_PF_WALK;
		}
	}

	dtNavMeshCreateParams params;
	memset(&params,0,sizeof(params));
	params.verts = polyMesh->verts;
	params.vertCount = polyMesh->nverts;
	params.polys = polyMesh->polys;
	params.polyAreas = polyMesh->areas;
	params.polyFlags = polyMesh->flags;
	params.polyCount = polyMesh->npolys;
	params.nvp = polyMesh->nvp;
	params.detailMeshes = detailMesh->meshes;
	params.detailVerts = detailMesh->verts;
	params.detailVertsCount = detailMesh->nverts;
	params.detailTris = detailMesh->tris;
	params.detailTriCount = detailMesh->ntris;

	//offmesh connections are not implemented. I don't even know what they are!
	params.offMeshConCount = 0;

	params.walkableHeight = config.userConfig->getAgentHeight();
	params.walkableRadius = config.userConfig->getAgentRadius();
	params.walkableClimb = config.userConfig->getAgentMaxClimb();
	params.buildBvTree = true;

	rcVcopy(params.bmin, config.recastConfig->bmin);
	rcVcopy(params.bmax, config.recastConfig->bmax);
	params.cs = config.recastConfig->cs;
	params.ch = config.recastConfig->ch;

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Detour - Stage 2" << std::endl;
#endif

	if(!dtCreateNavMeshData(&params,&navData,&navDataSize))
	{
		std::cout << "Error! Detour - could not build navmesh!" << std::endl;
		std::cout << " - " << params.cs << std::endl;
		std::cout << " - " << params.ch << std::endl;
		std::cout << " - " << params.walkableRadius << std::endl;
		//_isMeshBuilt = false;
		return;
	}

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Detour - Stage 3" << std::endl;
#endif

	_navMesh = dtAllocNavMesh();
	if(!_navMesh)
	{
		dtFree(_navMesh);
		std::cout << "Error! Detour - could not create Detour navmesh!" << std::endl;
		return;
	}

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Detour - Stage 4" << std::endl;
#endif

	dtStatus status;
	status = _navMesh->init(navData,navDataSize,DT_TILE_FREE_DATA);
	if(dtStatusFailed(status))
	{
		dtFree(navData);
		std::cout << "Error! Could not initialize Detour NavMesh." << std::endl;
		return;
	}

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Detour - Stage 5" << std::endl;
#endif

	_navQuery = dtAllocNavMeshQuery();
	status = _navQuery->init(_navMesh,2048);
	if(dtStatusFailed(status))
	{
		std::cout << "Error! Detour - could not initialize Detour navmesh query." << std::endl;
		return;
	}

	_isMeshBuilt = true;
}

Ogre::Vector3 DetourInterface::getRandomNavMeshPoint()
{
	dtQueryFilter filter;
	filter.setIncludeFlags(0xFFFF);
	filter.setExcludeFlags(0);
	filter.setAreaCost(DT_PA_GROUND,1.0f);
	//filter.setAreaCost(DT_TILECACHE_WALKABLE_AREA,1.0f);

	float resultPoint[3];
	dtPolyRef resultPoly;
	_navQuery->findRandomPoint(&filter,frand,&resultPoly,resultPoint);

	return Ogre::Vector3(resultPoint[0],resultPoint[1],resultPoint[2]);
}

bool DetourInterface::findNearestPointOnNavmesh(const Ogre::Vector3& position,Ogre::Vector3& resultPoint)
{
	float extents[3] = { 16.0f, 16.0f, 16.0f };

	dtQueryFilter filter;
	filter.setIncludeFlags(0xFFFF);
	filter.setExcludeFlags(0);
	filter.setAreaCost(DT_PA_GROUND,1.0f);
	//filter.setAreaCost(DT_TILECACHE_WALKABLE_AREA,1.0f);

	float point[3];
	Utility::vector3_toFloatPtr(position,point);
	float rPoint[3];
	dtPolyRef poly;
	dtStatus status = _navQuery->findNearestPoly(point,extents,&filter,&poly,rPoint);
	//Check if Detour found a polygon.
	if( (status & DT_FAILURE) || ( status & DT_STATUS_DETAIL_MASK) )
	{
		return false;
	}

	Utility::floatPtr_toVector3(rPoint,resultPoint);
	return true;
}

DetourInterface::DT_PATHFIND_RETURN DetourInterface::findPath(const Ogre::Vector3& startPosition,const Ogre::Vector3& endPosition,int pathNum,int target)
{
	float start[3];
	float end[3];
	Utility::vector3_toFloatPtr(startPosition,start);
	Utility::vector3_toFloatPtr(endPosition,end);
	return findPath(start,end,pathNum,target);
}

DetourInterface::DT_PATHFIND_RETURN DetourInterface::findPath(float* startPosition,float* endPosition,int pathNum,int target)
{
	dtStatus status;
	float extents[3] = { 32.0f,32.0f,32.0f };
	dtPolyRef startPoly;
	float startNearest[3];

	dtPolyRef endPoly;
	float endNearest[3];

	dtPolyRef polyPath[MAX_PATHPOLY];
	int pathCount = 0;
	float straightPath[MAX_PATHVERT * 3];
	int vertexCount = 0;

	dtQueryFilter filter;
	filter.setIncludeFlags(0xFFFF);
	filter.setExcludeFlags(0);
	filter.setAreaCost(DT_PA_GROUND,1.0f);
	
	status = _navQuery->findNearestPoly(startPosition,extents,&filter,&startPoly,startNearest);
	if( (status & DT_FAILURE) || (status & DT_STATUS_DETAIL_MASK) )
	{
		return DT_PATH_NOPOLY_START;
	}

	status = _navQuery->findNearestPoly(endPosition,extents,&filter,&endPoly,endNearest);
	if( (status & DT_FAILURE) || (status & DT_STATUS_DETAIL_MASK) )
	{
		return DT_PATH_NOPOLY_END;
	}

	status = _navQuery->findPath(startPoly,endPoly,startPosition,endPosition,&filter,polyPath,&pathCount,MAX_PATHPOLY);
	if( (status & DT_FAILURE) || (status & DT_STATUS_DETAIL_MASK) )
	{
		return DT_PATH_NOCREATE;
	}
	if(pathCount == 0) { return DT_PATH_NOFIND; }

	status = _navQuery->findStraightPath(startPosition,endPosition,polyPath,pathCount,straightPath,NULL,NULL,&vertexCount,MAX_PATHVERT);
	if( (status & DT_FAILURE) || (status & DT_STATUS_DETAIL_MASK) )
	{
		return DT_PATH_NOCREATESTRAIGHT;
	}
	if(vertexCount == 0) { return DT_PATH_NOFINDSTRAIGHT; }

	//have our path.
	//copy it to the path storehouse.


}

void DetourInterface::detourCleanup()
{
	dtFreeNavMesh(_navMesh);
	_navMesh = 0;

	dtFreeNavMeshQuery(_navQuery);
	_navQuery = 0;
}