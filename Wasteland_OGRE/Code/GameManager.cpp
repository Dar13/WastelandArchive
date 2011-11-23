#include "GameManager.h"

template<> GameManager* Ogre::Singleton<GameManager>::ms_Singleton = 0;

GameManager::GameManager()
{
	//initialize whatever pointers this class'll hold.
}

GameManager::~GameManager()
{
	//destroy whatever I initialized(if anything) in the constructor.
}

btBvhTriangleMeshShape* GameManager::buildLevelCollisionShape(Ogre::SceneNode* node)
{
	btBvhTriangleMeshShape* shape;
	btTriangleMesh* bmesh = new btTriangleMesh();

	Ogre::MeshPtr mesh = ((Ogre::Entity*)node->getAttachedObject(0))->getMesh();
	size_t vertCnt,inCnt;
	Ogre::Vector3* vertices;
	unsigned long* indices;

	OgreManager::getSingleton().getMeshInformation(&mesh,vertCnt,vertices,inCnt,indices);

	btVector3 vert1,vert2,vert3;

	for(int i=0; i<vertCnt; i+=3)
	{
		vert1.setValue(vertices[indices[i]].x,vertices[indices[i]].y,vertices[indices[i]].z);
		vert2.setValue(vertices[indices[i+1]].x,vertices[indices[i+1]].y,vertices[indices[i+1]].z);
		vert3.setValue(vertices[indices[i+2]].x,vertices[indices[i+2]].y,vertices[indices[i+2]].z);

		bmesh->addTriangle(vert1,vert2,vert3);
	}

	shape = new btBvhTriangleMeshShape(bmesh,true,true);

	delete[] vertices;
	delete[] indices;

	return shape;
}

Ogre::SceneNode* GameManager::createLevel(Ogre::SceneManager* scene,std::map<std::string,std::string> &options)
{
	Ogre::Entity* tempEnt = scene->createEntity(options["name"],options["filename"],options["resgroup"]);
	Ogre::SceneNode* tempNode = scene->getRootSceneNode()->createChildSceneNode(options["name"]);
	tempNode->attachObject(tempEnt);

	btBvhTriangleMeshShape* shape = buildLevelCollisionShape(tempNode);
	btScalar mass = 0.0f; //can be zero, it's a level collision shape. not going anywhere.
	btVector3 inertia(0,0,0); //no inertia, can't move;
	btTransform init;
	init.setIdentity();
	init.setOrigin(btVector3(0,0,0));
	BulletManager::getSingleton().addRigidBody(shape,tempNode,mass,init);

	return tempNode;
}