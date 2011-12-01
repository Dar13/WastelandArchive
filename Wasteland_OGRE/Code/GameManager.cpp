#include "GameManager.h"

template<> GameManager* Ogre::Singleton<GameManager>::ms_Singleton = 0;

GameManager::GameManager()
{
	Time = (float)OgreManager::getSingleton().getTimer()->getMilliseconds();
}

GameManager::~GameManager()
{
	//destroy whatever I initialized(if anything) in the constructor.
}

bool GameManager::UpdateManagers()
{
	bool retVal = true;
	//Update OIS
	OISManager::getSingleton().capture();
	retVal = !OISManager::getSingleton().escapePressed();

	//Update Bullet
	oldTime = Time;
	Time = (float)OgreManager::getSingleton().getTimer()->getMilliseconds();
	deltaTime = (Time - oldTime)/1000.0f;
	BulletManager::getSingleton().Update(deltaTime);

	//Update Ogre
	Ogre::WindowEventUtilities::messagePump();
	//If an error occurred...
	if(!OgreManager::getSingleton().Render())
	{
		//state needs to end
		retVal=false;
	}

	return retVal;
}

void GameManager::createCharacterController(Ogre::Camera* camera,Ogre::Vector3 initPosition)
{
	btTransform initial;
	initial.setIdentity();
	initial.setOrigin(btVector3(initPosition.x,initPosition.y,initPosition.z));

	_charGhost = new btPairCachingGhostObject();
	_charGhost->setWorldTransform(initial);

	BulletManager::getSingleton().getWorld()->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	btScalar charHeight = 32.5f;
	btScalar charWidth = 12.5f;
	btConvexShape* capsule = new btCapsuleShape(charWidth,charHeight);
	_charGhost->setCollisionShape(capsule);
	_charGhost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btScalar stepHeight = 0.35f;
	_charController = new btKinematicCharacterController(_charGhost,capsule,stepHeight);

	//adding the ghost and character controller to the physics world
	BulletManager::getSingleton().getWorld()->addCollisionObject(_charGhost,btBroadphaseProxy::CharacterFilter,btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	BulletManager::getSingleton().getWorld()->addAction(_charController);

	//Stores the current camera that the character controller is controlling.
	_charCamera = camera;

	return;
}

OgreBulletPair GameManager::createObject(Ogre::SceneManager* scene,std::map<std::string,std::string> &options,btScalar &mass, btTransform &init,btCollisionShape* shape)
{
	//return variable
	OgreBulletPair retVal;

	//Creating the Ogre SceneNode*
	Ogre::Entity* ent = scene->createEntity(options["name"],options["filename"],options["resgroup"]);
	Ogre::Vector3 pos; btVector3 orig = init.getOrigin();
	pos = convertBulletVector3(orig);
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

//----------------------------------------
//Private, utility functions start here...
//----------------------------------------

//converts bullet vector3 to ogre vector3
Ogre::Vector3 GameManager::convertBulletVector3(const btVector3 &v)
{
	return Ogre::Vector3(v.x(),v.y(),v.z());
}

//converts ogre vector3 to bullet vector3
btVector3 GameManager::convertOgreVector3(const Ogre::Vector3 &v)
{
	return btVector3(v.x,v.y,v.z);
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

	for(unsigned int i=0; i<vertCnt; i+=3)
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