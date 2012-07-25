#include "StdAfx.h"

#include <Recast.h>

#ifndef _RECAST_INTERFACE_H_
#define _RECAST_INTERFACE_H_

class RecastConfiguration
{
public:

private:
	//For full explanation of some of the member variables go here:
	//https://github.com/duststorm/OgreRecastDemo/blob/master/include/OgreRecast.h
	/*
	Cellsize (cs) is the width and depth resolution used when sampling the source geometry.
	The width and depth of the cell columns that make up voxel fields.
	Cells are laid out on the width/depth plane of voxel fields. Width is associated with
	the x-axis of the source geometry. Depth is associated with the z-axis. A lower value 
	allows for the generated meshes to more closely match the source geometry, but at a 
	higher processing and memory cost.
	
	The xz-plane cell size to use for fields. [Limit: > 0] [Units: wu].
	cs and ch define voxel/grid/cell size. So their values have significant side effects on 
	all parameters defined in voxel units. The minimum value for this parameter depends on 
	the platform's floating point accuracy, with the practical minimum usually around 0.05.
	*/
	float _cellSize;

	/*
	Cellheight (ch) is the height resolution used when sampling the source geometry. The 
	height of the voxels in voxel fields. Height is associated with the y-axis of the 
	source geometry. A smaller value allows for the final meshes to more closely match the 
	source geometry at a potentially higher processing cost.
	(Unlike cellSize, using a lower value for cellHeight does not significantly increase memory use.)
    
	The y-axis cell size to use for fields. [Limit: > 0] [Units: wu].
	cs and ch define voxel/grid/cell size. So their values have significant side effects 
	on all parameters defined in voxel units. The minimum value for this parameter depends on 
	the platform's floating point accuracy, with the practical minimum usually around 0.05.
    
	Setting ch lower will result in more accurate detection of areas the agent can still pass 
	under, as min walkable height is discretisized in number of cells. Also walkableClimb's 
	precision is affected by ch in the same way, along with some other parameters.
	*/
	float _cellHeight;

	/*
	Maximum slope that is considered traversable(in degrees(!!)).
	0 <= value < 90
	Practical upper limit is ~85 degrees
	Alias: maxTraversableSlope
	*/
	float _agentMaxSlope;

	/*
	Height of an agent. Defines minimum height that agents can walk under.

	Alias: minTraversableHeight
	*/
	float _agentHeight;

	/*
	The Maximum ledge height that is considered to still be traversable.
	This parameter serves at setting walkableClimb (maxTraversableStep) 
	parameter, precision of this parameter is determined by cellHeight (ch).
	[Limit: >=0]
	Allows the mesh to flow over low lying obstructions such as curbs and up/down stairways. 
	The value is usually set to how far up/down an agent can step.
	*/
	float _agentMaxClimb;

	//Radius of agent. Used in Detour too.
	float _agentRadius;

	//Maximum length of contour edges. Vertices will be added as needed to keep this value true.
	float _edgeMaxLength;

	//Maximum allowed error for contour border edges.
	//Aliases: maxSimplificationError,edgeMaxDeviation
	float _edgeMaxError;
};

class RecastInterface
{
public:

private:

};

#endif