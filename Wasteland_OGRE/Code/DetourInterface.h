#include "StdAfx.h"

#include <Recast.h>
#include <DetourCommon.h>

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

class DetourInterface
{
public:
	//create constructors that create dtNavMesh/dtNavQuery/etc

	int findPath(float* startPos,float* endPos,int pathSlot,int targetID);
	int findPath(const Ogre::Vector3& startPos,const Ogre::Vector3& endPos,int pathSlot,int targetID);

	bool findNearestPointOnNavmesh(const Ogre::Vector3& position,Ogre::Vector3& resultPoint);

	Ogre::Vector3 getRandomNavMeshPoint();

private:
	dtNavMesh* _navMesh;
	dtNavMeshQuery* _navQuery;

	//PathData
	PathData _pathsData[MAX_PATHSLOT];
};

#endif