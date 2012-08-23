#include "StdAfx.h"

#include "RecastInputGeometry.h"
#include "Utility.h"
#include "GraphicsManager.h"

InputGeometry::InputGeometry(Ogre::Entity* sourceMesh)
	: _numVertices(0),
	  _numTriangles(0),
	  _triangles(0),
	  _normals(0),
	  _vertices(0),
	  _boundMin(0),
	  _boundMax(0),
	  _referenceNode(nullptr)
{
	if(!sourceMesh)
	{
		return;
	}

	_sourceMeshes.push_back(sourceMesh);

	if(sourceMesh->getParentSceneNode() != nullptr)
	{
		_referenceNode = sourceMesh->getParentSceneNode()->getCreator()->getRootSceneNode();
	}
	else
	{
		return;
	}

	_calculateExtents();

	_convertOgreEntities();

	//eventually add _buildChunkyTriMesh here
}

InputGeometry::InputGeometry(std::vector<Ogre::Entity*> sourceMeshes)
	: _sourceMeshes(sourceMeshes),
	  _numVertices(0),
	  _numTriangles(0),
	  _referenceNode(0),
	  _boundMin(0),
	  _boundMax(0),
	  _normals(0),
	  _vertices(0),
	  _triangles(0)
{
	if(sourceMeshes.empty())
	{
		return;
	}

	Ogre::Entity* tmpEnt = sourceMeshes[0];
	_referenceNode = tmpEnt->getParentSceneNode()->getCreator()->getRootSceneNode();

	_calculateExtents();

	_convertOgreEntities();

	//eventually add _buildChunkTriMesh
}

InputGeometry::InputGeometry(std::vector<Ogre::Entity*> sourceMeshes,const Ogre::AxisAlignedBox& tileBounds)
	: _sourceMeshes(sourceMeshes),
	  _numVertices(0),
	  _numTriangles(0),
	  _referenceNode(0),
	  _boundMin(0),
	  _boundMax(0),
	  _normals(0),
	  _vertices(0),
	  _triangles(0)
{
	if(sourceMeshes.empty())
	{
		return;
	}

	Ogre::Entity* ent = sourceMeshes[0];
	_referenceNode = ent->getParentSceneNode()->getCreator()->getRootSceneNode();

	_boundMin = new float[3];
	_boundMax = new float[3];
	Utility::vector3_toFloatPtr(tileBounds.getMinimum(),_boundMin);
	Utility::vector3_toFloatPtr(tileBounds.getMaximum(),_boundMax);

	_convertOgreEntities(tileBounds);

	//eventually add _buildChunkyTriMesh()

}

InputGeometry::~InputGeometry()
{
	if(_vertices)
	{
		delete[] _vertices;
	}
	if(_normals)
	{
		delete[] _normals;
	}
	if(_triangles)
	{
		delete[] _triangles;
	}
	if(_boundMin)
	{
		delete[] _boundMin;
	}
	if(_boundMax)
	{
		delete[] _boundMax;
	}
}

void InputGeometry::_convertOgreEntities()
{
	int numNodes = _sourceMeshes.size();
	size_t* meshVertexCount = new size_t[numNodes];
	size_t* meshIndexCount = new size_t[numNodes];
	Ogre::Vector3** meshVertices = new Ogre::Vector3*[numNodes];
	unsigned long** meshIndices = new unsigned long*[numNodes];

	_numVertices = 0;
	_numTriangles = 0;
	size_t i = 0; // used as additional incrementer
	for(auto itr = _sourceMeshes.begin(); itr != _sourceMeshes.end(); ++itr)
	{
		Ogre::MeshPtr meshPtr = (*itr)->getMesh();
		GraphicsManager::getMeshInformation(&meshPtr,
											meshVertexCount[i],
											meshVertices[i],
											meshIndexCount[i],
											meshIndices[i]);
		_numVertices += meshVertexCount[i];
		_numTriangles += meshIndexCount[i];
		i++;
	}

	_vertices = new float[_numVertices * 3];
	_triangles = new int[_numTriangles]; //Triangles in Recast = Indices in Ogre

	//Triangle array is indices, number of triangles is actual number of triangles
	_numTriangles = _numTriangles / 3; // indices / 3

	int vertsIndex = 0,prevVerticesCount = 0,prevIndexCountTotal = 0;
	i = 0;
	for(auto itr = _sourceMeshes.begin(); itr != _sourceMeshes.end(); ++itr)
	{
		Ogre::Entity* ent = *itr;
		Ogre::Matrix4 transform = _referenceNode->_getFullTransform().inverse() * ent->getParentSceneNode()->_getFullTransform();
		Ogre::Vector3 vertexPos;
		for(size_t j = 0; j < meshVertexCount[i]; j++)
		{
			vertexPos = transform * meshVertices[i][j];
			_vertices[vertsIndex + 0] = vertexPos.x;
			_vertices[vertsIndex + 1] = vertexPos.y;
			_vertices[vertsIndex + 2] = vertexPos.z;
			vertsIndex += 3;
		}
		for(size_t j = 0; j < meshIndexCount[i]; j++)
		{
			_triangles[prevIndexCountTotal + j] = meshIndices[i][j] + prevVerticesCount;
		}
		prevIndexCountTotal += meshIndexCount[i];
		prevVerticesCount += meshVertexCount[i];

		i++;
	}

	delete[] meshIndices[0];
	delete[] meshVertices[0];

	delete[] meshIndices;
	delete[] meshVertices;
	delete meshVertexCount;
	delete meshIndexCount;

	_normals = new float[_numTriangles * 3];
	for(int i = 0; i< _numTriangles * 3; i += 3)
	{
		const float* v0 = &_vertices[_triangles[i] * 3];
		const float* v1 = &_vertices[_triangles[i+1] * 3];
		const float* v2 = &_vertices[_triangles[i+2] * 3];
		//std::cout << *v0 << "," << *v1 << "," << *v2 << std::endl;
		float e0[3],e1[3];
		for(int j = 0; j < 3; j++)
		{
			e0[j] = (v1[j] - v0[j]);
			e1[j] = (v2[j] - v0[j]);
		}
		float* n = &_normals[i];
        n[0] = ((e0[1]*e1[2]) - (e0[2]*e1[1]));
        n[1] = ((e0[2]*e1[0]) - (e0[0]*e1[2]));
        n[2] = ((e0[0]*e1[1]) - (e0[1]*e1[0]));

        float d = sqrtf(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
        if (d > 0)
        {
            d = 1.0f/d;
            n[0] *= d;
            n[1] *= d;
            n[2] *= d;
        }
	}
}

void InputGeometry::_convertOgreEntities(const Ogre::AxisAlignedBox& tileBounds)
{
	std::vector<Ogre::Entity*> selectedEntities;
	Ogre::AxisAlignedBox boundingBox;
	Ogre::Matrix4 transform;
	for(auto itr = _sourceMeshes.begin(); itr != _sourceMeshes.end(); ++itr)
	{
		transform = _referenceNode->_getFullTransform().inverse() * (*itr)->getParentSceneNode()->_getFullTransform();
		boundingBox = (*itr)->getBoundingBox();
		boundingBox.transform(transform);
		if(boundingBox.intersects(tileBounds))
		{
			selectedEntities.push_back(*itr);
		}
	}

	_sourceMeshes.clear();
	_sourceMeshes = selectedEntities;

	_convertOgreEntities();
}

void InputGeometry::_calculateExtents()
{
	Ogre::Entity* ent = _sourceMeshes[0];
	Ogre::AxisAlignedBox sourceMeshBB = ent->getBoundingBox();
	Ogre::Matrix4 transform;
	transform = _referenceNode->_getFullTransform().inverse() * ent->getParentSceneNode()->_getFullTransform();
	sourceMeshBB.transform(transform);
	Ogre::Vector3 min = sourceMeshBB.getMinimum();
	Ogre::Vector3 max = sourceMeshBB.getMaximum();

	for(auto itr = _sourceMeshes.begin(); itr != _sourceMeshes.end(); ++itr)
	{
		Ogre::Entity* tmpEnt = *itr;

		transform = _referenceNode->_getFullTransform().inverse() * ent->getParentSceneNode()->_getFullTransform();

		sourceMeshBB = ent->getBoundingBox();
		sourceMeshBB.transform(transform);
		Ogre::Vector3 min2 = sourceMeshBB.getMinimum();
		if(min2.x < min.x)
            min.x = min2.x;
        if(min2.y < min.y)
            min.y = min2.y;
        if(min2.z < min.z)
            min.z = min2.z;

		Ogre::Vector3 max2 = sourceMeshBB.getMaximum();
		if(max2.x > max.x)
            max.x = max2.x;
        if(max2.y > max.y)
            max.y = max2.y;
        if(max2.z > max.z)
            max.z = max2.z;
	}

	if(!_boundMin)
	{
		_boundMin = new float[3];
	}
	if(!_boundMax)
	{
		_boundMax = new float[3];
	}
	Utility::vector3_toFloatPtr(min,_boundMin);
	Utility::vector3_toFloatPtr(max,_boundMax);
}

float* InputGeometry::getMeshBoundsMin()
{
	return _boundMin;
}

float* InputGeometry::getMeshBoundsMax()
{
	return _boundMax;
}

int InputGeometry::getVertexCount()
{
	return _numVertices;
}

int InputGeometry::getTriangleCount()
{
	return _numTriangles;
}

int* InputGeometry::getTriangles()
{
	return _triangles;
}

float* InputGeometry::getVertices()
{
	return _vertices;
}

float* InputGeometry::getNormals()
{
	return _normals;
}

bool InputGeometry::isEmpty()
{
	return (_numVertices <= 0 || _numTriangles <= 0);
}

void InputGeometry::writeToObj(const std::string& filename)
{
	std::fstream out(filename.c_str(),std::ios::out);

	for(int i = 0; i < _numVertices; i++)
	{
		std::string line = "v " + Ogre::StringConverter::toString(_vertices[(3*i)]) + 
			" " + Ogre::StringConverter::toString(_vertices[(3*i) + 1]) + 
			" " + Ogre::StringConverter::toString(_vertices[(3*i) + 2]);
		out << line << std::endl;
	}

	for(int i = 0; i < _numTriangles; i++)
	{
		std::string line = "f " + Ogre::StringConverter::toString(1+_triangles[3*i]) + 
			" " + Ogre::StringConverter::toString(1 + _triangles[3*i + 1]) + 
			" " + Ogre::StringConverter::toString(1 + _triangles[3*i + 2]);
		out << line << std::endl;
	}

	out.close();
}