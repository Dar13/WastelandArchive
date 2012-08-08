#include "StdAfx.h"

#include <Recast.h>
#include <RecastDump.h>
#include "RecastDetourUtil.h"
#include "RecastInputGeometry.h"

#ifndef _RECAST_INTERFACE_H_
#define _RECAST_INTERFACE_H_

//The interface between Recast and Ogre
//Detour will have its own interface.

class RecastInterface
{
public:
	//Initializes the wrapper. It'll be app-state specific, as it relies on a scene manager.
	RecastInterface(Ogre::SceneManager* scene,RecastConfiguration config = RecastConfiguration());
	~RecastInterface();

	void configure(RecastConfiguration config);

	//Update method for debugging static meshes.
	//Not sure if I'll need it, we'll see.
	//void update();

	//Gets the configuration options used by the wrapper in constructing the current nav mesh.
	RecastConfiguration& getRecastBuildConfiguration();

	//The amount the drawn debug navmesh polys are offset from the ground.
	float getNavmeshOffsetFromGround();
	
	//Builds a navmesh based on the parameters sent in.
	bool buildNavMesh(std::vector<Ogre::Entity*> sourceMeshes);
	bool buildNavMesh(InputGeometry* inputGeom);

	//Generates an ogre-drawable mesh from the nav-mesh.
	void createRecastPolygonMesh(const std::string& name,const unsigned short *vertices,const int numVerts,
								 const unsigned short *polygons,const int numPolys,const unsigned char *areas,
								 const int maxPolys,const unsigned short* regions,const int numVertsPerPoly,
								 const float cellSize,const float cellHeight,const float* origin,bool colorRegions = true);

	void exportPolygonMeshToObj(const std::string& filename);

	void removeDrawnNavMesh(unsigned int tileRef);

	rcConfig& getRecastConfig() { return _config; }

	rcPolyMesh* getPolyMesh() { return _polyMesh; }
	rcPolyMeshDetail* getDetailMesh() { return _detailMesh; }

	void recastClean();

protected:
	Ogre::SceneManager* _scene;

	unsigned char* _triangleAreas;
	rcHeightfield* _solid;
	rcCompactHeightfield* _compactHeightfield;
	rcContourSet* _contourSet;
	rcPolyMesh* _polyMesh;
	rcConfig _config;
	RecastConfiguration _recastParams;
	rcPolyMeshDetail* _detailMesh;

	InputGeometry* _inputGeom;
	rcContext* _context;

	Ogre::StaticGeometry* _staticGeom;
	bool _rebuildStaticGeom;

private:
	void _printConfig();
};

#endif