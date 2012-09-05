#include "StdAfx.h"

#include "RecastInterface.h"
#include "Utility.h"
#include "GraphicsManager.h"

RecastInterface::RecastInterface(Ogre::SceneManager* scene,RecastConfiguration config)
	: _scene(scene),
	  _triangleAreas(nullptr),
	  _solid(nullptr),
	  _compactHeightfield(nullptr),
	  _contourSet(nullptr),
	  _polyMesh(nullptr),
	  _detailMesh(nullptr),
	  _context(nullptr),
	  _staticGeom(nullptr)
{
	recastClean();

	configure(config);
}

RecastInterface::~RecastInterface()
{
	recastClean();
}

void RecastInterface::configure(RecastConfiguration config)
{
	if(_context)
	{
		delete _context;
	}
	_context = new rcContext(true);

	memset(&_config,0,sizeof(_config));
	_config.cs = config.getCellSize();
	_config.ch = config.getCellHeight();
	_config.walkableSlopeAngle = config.getAgentMaxSlope();
	_config.walkableClimb = config._getWalkableClimb();
	_config.walkableRadius = config._getWalkableRadius();
	_config.walkableHeight = config._getWalkableHeight();
	_config.maxEdgeLen = config._getMaxEdgeLength();
	_config.minRegionArea = config._getMinRegionArea();
	_config.mergeRegionArea = config._getMergeRegionArea();
	_config.detailSampleDist = config._getDetailSampleDist();
	_config.detailSampleMaxError = config._getDetailSampleMaxError();
	_config.maxVertsPerPoly = config.getVerticesPerPolygon();
	_config.maxSimplificationError = config.getEdgeMaxError();
	_config.tileSize = 0;

	_recastParams = config;
}

void RecastInterface::recastClean()
{
	if(_triangleAreas)
	{
		delete[] _triangleAreas;
		_triangleAreas = nullptr;
	}

	rcFreeHeightField(_solid);
	_solid = nullptr;

	rcFreeCompactHeightfield(_compactHeightfield);
	_compactHeightfield = nullptr;

	rcFreeContourSet(_contourSet);
	_contourSet = nullptr;

	rcFreePolyMesh(_polyMesh);
	_polyMesh = nullptr;

	rcFreePolyMeshDetail(_detailMesh);
	_detailMesh = nullptr;

	if(_context)
	{
		delete _context;
		_context = nullptr;
	}
}

bool RecastInterface::buildNavMesh(std::vector<Ogre::Entity*> sourceMeshes)
{
	if(sourceMeshes.empty())
	{
		std::cout << "NavMesh build failed: No entities provided" << std::endl;
		Ogre::LogManager::getSingletonPtr()->logMessage("NavMesh build failed: No entities provided");
		return false;
	}

	InputGeometry* param = new InputGeometry(sourceMeshes);
	bool result = buildNavMesh(param);
	delete param;

	return result;
}

bool RecastInterface::buildNavMesh(InputGeometry* inputGeom)
{
	

	//Step 1 : Initialize build configuration
	//Start the timers.
	
#ifdef _DEBUG
	std::cout << "NavMesh build started." << std::endl;
	unsigned long start = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	unsigned long end = 0;
#endif

	//Step 2 : Rasterize input polygon soup
	//InputGeometry* input = inputGeom; WTF? Is this necessary?
	rcVcopy(_config.bmin, inputGeom->getMeshBoundsMin());
	rcVcopy(_config.bmax, inputGeom->getMeshBoundsMax());
	rcCalcGridSize(_config.bmin,_config.bmax,_config.cs,&_config.width,&_config.height);

	int numVerts = inputGeom->getVertexCount();
	int numTris = inputGeom->getTriangleCount();

#ifdef _DEBUG
	Ogre::Vector3 min,max;
	Utility::floatPtr_toVector3(inputGeom->getMeshBoundsMin(),min);
	Utility::floatPtr_toVector3(inputGeom->getMeshBoundsMax(),max);
	std::cout << "Bounds: min=" << min << " max=" << max << std::endl;
	
	std::cout << "Building navmesh" << std::endl;
	std::cout << " - " << _config.width << " x " << _config.height << std::endl;
	std::cout << " - " << numVerts / 1000.0f << "K vertices, ";
	std::cout << numTris / 1000.0f << "K triangles" << std::endl;

	//_printConfig();
#endif

	_solid = rcAllocHeightfield();
	if(!_solid)
	{
		std::cout << "Error! Out of memory needed for '_solid'." << std::endl;
		return false;
	}
	if(!rcCreateHeightfield(_context,*_solid,
							_config.width,_config.height,
							_config.bmin,_config.bmax,
							_config.cs,_config.ch))
	{
		std::cout << "Error! Couldn't create heightfield, try higher cellSize and cellHeight values." << std::endl;
		return false;
	}

	//holds triangle area types
	_triangleAreas = new unsigned char[numTris];
	if(!_triangleAreas)
	{
		std::cout << "Error! Out of memory '_triangleAreas'([" << numTris << "])" << std::endl;
		return false;
	}

	//find triangles that are walkable in slope and rasterize them
	memset(_triangleAreas,0,numTris * sizeof(unsigned char));
	rcMarkWalkableTriangles(_context,_config.walkableSlopeAngle,
							inputGeom->getVertices(),inputGeom->getVertexCount(),
							inputGeom->getTriangles(),inputGeom->getTriangleCount(),
							_triangleAreas);
	rcRasterizeTriangles(_context,inputGeom->getVertices(),inputGeom->getVertexCount(),
						 inputGeom->getTriangles(),_triangleAreas,
						 inputGeom->getTriangleCount(),*_solid,_config.walkableClimb);

	//I know I put this option in the params, but...
	if(!_recastParams.getKeepIntermediateResults())
	{
		delete[] _triangleAreas;
		_triangleAreas = nullptr;
	}

	//Step 3 : Filter walkables surfaces
	//Initial pass of filtering to remove unwanted overhangs caused
	//by the conservative rasterization.
	//Also filters spans where the character can't stand.
	rcFilterLowHangingWalkableObstacles(_context,_config.walkableClimb,*_solid);
	rcFilterLedgeSpans(_context,_config.walkableHeight,_config.walkableClimb,*_solid);
	rcFilterWalkableLowHeightSpans(_context,_config.walkableHeight,*_solid);

	//Step 4 : Partition walkable surface to simple regions
	//Compact the heightfield so that it is faster to handle from now on.
	_compactHeightfield = rcAllocCompactHeightfield();
	if(!_compactHeightfield)
	{
		std::cout << "Error! Out of memory '_compactHeightfield'" << std::endl;
		return false;
	}
	if(!rcBuildCompactHeightfield(_context,
								  _config.walkableHeight,_config.walkableClimb,
								  *_solid,*_compactHeightfield))
	{
		std::cout << "Error! BuildNav - Could not build compact data." << std::endl;
		return false;
	}

	if(!_recastParams.getKeepIntermediateResults())
	{
		rcFreeHeightField(_solid);
		_solid = nullptr;
	}

	//Erode walkable area by agent radius
	if(!rcErodeWalkableArea(_context,_config.walkableRadius,*_compactHeightfield))
	{
		std::cout << "Error! BuildNav - Could not erode walkable areas." << std::endl;
		return false;
	}

	//Prepare for region partitioning, generate distance field
	if(!rcBuildDistanceField(_context,*_compactHeightfield))
	{
		std::cout << "Error! BuildNav - Could not build distance field." << std::endl;
		return false;
	}

	//Partition the walkable surface into simple regions w/o holes
	if(!rcBuildRegions(_context,*_compactHeightfield,
					   _config.borderSize,
					   _config.minRegionArea,_config.mergeRegionArea))
	{
		std::cout << "Error! BuildNav - Could not build regions." << std::endl;
		return false;
	}

	//Step 5 : Trace and simplify region contours.
	//create contours
	_contourSet = rcAllocContourSet();
	if(!_contourSet)
	{
		std::cout << "Error! BuildNav - Out of memory '_contourSet'" << std::endl;
		return false;
	}

	if(!rcBuildContours(_context,*_compactHeightfield,_config.maxSimplificationError,_config.maxEdgeLen,*_contourSet))
	{
		
		std::cout << "Error! BuildNav - Could not create contours." << std::endl;
		return false;
	}

	if(_contourSet->nconts == 0)
	{
		//Check this thread for details on solving these:
		//http://groups.google.com/group/recastnavigation/browse_thread/thread/a6fbd509859a12c8
		std::cout << "Error! BuildNav - RecastNav created no contours!" << std::endl;
		std::cout << _compactHeightfield->spanCount << std::endl;
	}

	//Step 6 : Build polygons mesh from contours
	//Build polygon navmesh from the contours
	_polyMesh = rcAllocPolyMesh();
	if(!_polyMesh)
	{
		std::cout << "Error! Out of memory '_polyMesh'." << std::endl;
		return false;
	}

	if(!rcBuildPolyMesh(_context,*_contourSet,_config.maxVertsPerPoly,*_polyMesh))
	{
		std::cout << "Error! BuildNav - Could not triangulate contours." << std::endl;
	}

	//Step 7 : Create detail mesh which allows access to approximate height on each polygon.
	_detailMesh = rcAllocPolyMeshDetail();
	if(!_detailMesh)
	{
		std::cout << "Error! Out of memory '_detailMesh'." << std::endl;
		return false;
	}

	if(!rcBuildPolyMeshDetail(_context,
							  *_polyMesh,*_compactHeightfield,
							  _config.detailSampleDist,_config.detailSampleMaxError,
							  *_detailMesh))
	{
		std::cout << "Error! BuildNav - Could not build detail mesh." << std::endl;
		return false;
	}

	if(!_recastParams.getKeepIntermediateResults())
	{
		rcFreeCompactHeightfield(_compactHeightfield);
		_compactHeightfield = nullptr;

		rcFreeContourSet(_contourSet);
		_contourSet = nullptr;
	}

	//Recast navmesh is finished!
#ifdef _DEBUG
	end = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();
	std::cout << "Navmesh build finished." << std::endl;
	std::cout << " - Time elapsed:" << end - start << "ms" << std::endl;
#endif

	return true;
}

void RecastInterface::exportPolygonMeshToObj(const std::string& filename)
{
	/*std::fstream out(filename.c_str(),std::ios::out);

	for(int i=0; i < _detailMesh->nverts; ++i)
	{
		out << "v " << _polyMesh->verts[3*i] << " ";
		out << _polyMesh->verts[3*i + 1] << " ";
		out << _polyMesh->verts[3*i + 2] << std::endl;
	}

	for(int i = 0; i < _polyMesh->npolys; ++i)
	{
		out << "f " << 1 + _polyMesh->polys[3*i] << " ";
		out << 1 + _polyMesh->polys[3*i + 1] << " ";
		out << 1 + _polyMesh->polys[3*i + 2];
		out << std::endl;
	}

	out.close();*/
	Ogre::ManualObject* obj = _exportPolygonMeshToEntity(_polyMesh->verts,
														 _polyMesh->nverts,
														 _polyMesh->polys,
														 _polyMesh->npolys,
														 _polyMesh->maxpolys,
														 _polyMesh->nvp,
														 Ogre::Vector3(0,0,0));
	exportPolygonMeshToObj(obj,filename);
}

void RecastInterface::exportPolygonMeshToObj(Ogre::ManualObject* recastPolyMesh,const std::string& filename)
{
	std::fstream out(filename,std::ios::out);

	Ogre::MeshPtr mesh = recastPolyMesh->convertToMesh("tempMesh","Models");

	size_t vertCount,indexCount;
	Ogre::Vector3* vertices;
	unsigned long* indices;

	GraphicsManager::getMeshInformation(&mesh,vertCount,vertices,indexCount,indices);

	for(size_t i = 0; i < vertCount; ++i)
	{
		out << "v " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z <<std::endl;
	}

	for(size_t i = 0; i < indexCount/3; ++i)
	{
		out << "f " << 1+indices[3*i] << " " << 1+indices[3*i + 1] << " " << 1+indices[3*i + 2] << std::endl;
	}

	delete[] vertices;
	delete[] indices;

	out.close();
}

Ogre::ManualObject* RecastInterface::_exportPolygonMeshToEntity(unsigned short* vertices,
														  int numVertices,
														  unsigned short* polygons,
														  int numPolys,
														  int maxPolys,
														  int numVertsPerPoly,
														  const Ogre::Vector3& origin)
{
	if(_polyMesh == nullptr || _detailMesh == nullptr)
	{
		return NULL;
	}

	int nIndex = 0;
	Ogre::ManualObject* manObj = _scene->createManualObject("recastTempMO");
	manObj->begin("arena_locker/blue",Ogre::RenderOperation::OT_TRIANGLE_LIST);
	for( int i = 0; i < numPolys; ++i)
	{
		const unsigned short* p = &polygons[i * numVertsPerPoly * 2];

		unsigned short vi[3];
		for(int j = 2; j < numVertsPerPoly; ++j)
		{
			if(p[j] == RC_MESH_NULL_IDX) break;
			vi[0] = p[0];
			vi[1] = p[j-1];
			vi[2] = p[j];
			for(int k = 0; k < 3; ++k)
			{
				const unsigned short* v = &vertices[vi[k]*3];
				const float x = origin[0] + v[0] * _recastParams.getCellSize();
				const float y = origin[1] + v[1] * _recastParams.getCellHeight();
				const float z = origin[2] + v[2] * _recastParams.getCellSize();

				manObj->position(x,y,z);
			}
			manObj->triangle(nIndex,nIndex+1,nIndex+2);
			nIndex += 3;
		}
	}

	manObj->end();

	return manObj;
}

void RecastInterface::_printConfig()
{
	std::cout << "rcConfig" <<std::endl;
	std::cout << " - cs:" << _config.cs << std::endl;
	std::cout << " - ch:" << _config.ch << std::endl;
	std::cout << " - walkableRadius:" << _config.walkableRadius << std::endl;
	std::cout << " - walkableClimb:" << _config.walkableClimb << std::endl;
	std::cout << " - walkableMaxSlope:" << _config.walkableSlopeAngle << std::endl;
	std::cout << " - walkableHeight:" << _config.walkableHeight << std::endl;
}