#include "GameManager.h"

template<> GameManager* Ogre::Singleton<GameManager>::ms_Singleton = 0;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
	//destroy whatever I initialized(if anything) in the constructor.
}

OgreBulletPair GameManager::createObject(Ogre::SceneManager* scene,std::map<std::string,std::string> &options,btScalar &mass, btTransform &init,btCollisionShape* shape)
{
	//return variable
	OgreBulletPair retVal;

	//Creating the Ogre SceneNode*
	Ogre::Entity* ent = scene->createEntity(options["name"],options["filename"],options["resgroup"]);
	Ogre::Vector3 pos; btVector3 orig = init.getOrigin();
	pos.x = orig.x(); pos.y = orig.y(); pos.z = orig.z();
	Ogre::Quaternion rot; btQuaternion orot = init.getRotation();
	rot.w = orot.w(); rot.x = orot.x(); rot.y = orot.y(); rot.z = orot.z();
	Ogre::SceneNode* node = scene->getRootSceneNode()->createChildSceneNode(options["name"],pos,rot);
	retVal.ogreNode = node;

	//Easy function call.
	retVal.btBody=BulletManager::getSingleton().addRigidBody(shape,node,mass,init);

	//return by-value, not reference.
	return retVal;
}

OgreBulletPair GameManager::createObject(Ogre::SceneNode* node,btCollisionShape* shape, btScalar &mass, btTransform &init)
{
	//Here's the variable that'll be passed back(by-value, not reference!).
	OgreBulletPair retVal;

	//That's dead simple, it's already passed in!
	retVal.ogreNode=node;

	//Easy function call.
	retVal.btBody = BulletManager::getSingleton().addRigidBody(shape,node,mass,init);

	return retVal;
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