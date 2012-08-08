#include "StdAfx.h"

#include <DetourCommon.h>
#include <DetourCrowd.h>
#include <DetourNavMesh.h>
#include <DetourNavMeshBuilder.h>
#include <DetourNavMeshQuery.h>
#include "RecastDetourUtil.h"

#ifndef _DETOUR_INTERFACE_H_
#define _DETOUR_INTERFACE_H_

#define MAX_PATHSLOT 128 // max # of paths
#define MAX_PATHPOLY 256 // max # of polygons in path
#define MAX_PATHVERT 512 // max # of verts in path

class PathData
{
public:
	float x[MAX_PATHVERT];
	float y[MAX_PATHVERT];
	float z[MAX_PATHVERT];
	int maxVertex;
	int target;
};

struct rcPolyMesh;
struct rcPolyMeshDetail;
struct RecastDetourConfiguration;
typedef RecastDetourConfiguration rcdtConfig;

class DetourInterface
{
public:
	enum DT_POLYAREA_TYPE
	{
		DT_PA_GROUND = 1,
		DT_PA_WATER,
		DT_PA_ROAD,
		DT_PA_DOOR,
		DT_PA_GRASS,
		DT_PA_JUMP,
		DT_PA_MAX
	};

	enum DT_POLYFLAG_TYPE
	{
		DT_PF_WALK = 0x01,
		DT_PF_SWIM = 0x02,
		DT_PF_DOOR = 0x04,
		DT_PF_JUMP = 0x08,
		DT_PF_DISABLED = 0x10,
		DT_PF_ALL = 0xffff
	};

	enum DT_PATHFIND_RETURN
	{
		DT_PATH_SUCCESS = 0,
		DT_PATH_NOPOLY_START,
		DT_PATH_NOPOLY_END,
		DT_PATH_NOCREATE,
		DT_PATH_NOFIND,
		DT_PATH_NOCREATESTRAIGHT,
		DT_PATH_NOFINDSTRAIGHT
	};
	//create constructors that create dtNavMesh/dtNavQuery/etc
	DetourInterface(rcPolyMesh* polyMesh,rcPolyMeshDetail* detailMesh,rcdtConfig& config);
	~DetourInterface();

	bool findNearestPointOnNavmesh(const Ogre::Vector3& position,Ogre::Vector3& resultPoint);

	Ogre::Vector3 getRandomNavMeshPoint();

	DT_PATHFIND_RETURN findPath(const Ogre::Vector3& startPosition,const Ogre::Vector3& endPosition,int target,PathData* path);
	DT_PATHFIND_RETURN findPath(float* startPosition, float* endPosition, int target,PathData* path);

	bool isMeshBuilt() { return _isMeshBuilt; }

	void detourCleanup();

	dtNavMesh* getNavMesh() { return _navMesh; }
	dtNavMeshQuery* getNavQuery() { return _navQuery; }

private:
	dtNavMesh* _navMesh;
	dtNavMeshQuery* _navQuery;

	bool _isMeshBuilt;

	//PathData
	PathData _pathsData[MAX_PATHSLOT];
};

#endif