#include "StdAfx.h"

#ifndef _INPUT_GEOMETRY_H_
#define _INPUT_GEOMETRY_H_

#define MAX_VOLUME_OBSTACLES 256

class InputGeometry
{
public:
	InputGeometry(Ogre::Entity* sourceMesh);
	InputGeometry(std::vector<Ogre::Entity*> sourceMeshes);
	InputGeometry(std::vector<Ogre::Entity*> sourceMeshes,const Ogre::AxisAlignedBox& tileBounds);

	~InputGeometry();

	void writeToObj(const std::string& filename);

	float* getVertices();
	int getVertexCount();

	int* getTriangles();
	int getTriangleCount();

	float* getNormals();

	float* getMeshBoundsMin();
	float* getMeshBoundsMax();

	bool isEmpty();

private:
	void _calculateExtents();

	void _convertOgreEntities();
	void _convertOgreEntities(const Ogre::AxisAlignedBox& tileBounds);

	float* _vertices;
	int _numVertices;

	int* _triangles;
	int _numTriangles;

	float* _normals;

	float* _boundMin;
	float* _boundMax;

	std::vector<Ogre::Entity*> _sourceMeshes;
	Ogre::SceneNode* _referenceNode;
};

#endif