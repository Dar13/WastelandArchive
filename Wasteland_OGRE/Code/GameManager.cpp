#include "StdAfx.h"
#include "GameManager.h"

template<> GameManager* Ogre::Singleton<GameManager>::ms_Singleton = 0;

GameManager::GameManager()
{
	_config = NULL;
	Time = (float)OgreManager::getSingleton().getTimer()->getMilliseconds();
}

GameManager::~GameManager()
{
	//destroy whatever I initialized(if anything) in the constructor.
	if(_config)
	{
		delete _config;
	}
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

void GameManager::loadConfiguration(std::string& file)
{
	_config = configuration(file).release();
	OISManager::getSingleton().setConfiguration(_config);
}

configuration_t* GameManager::getConfiguration()
{
	return _config;
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
	_charNode = camera->getSceneManager()->getRootSceneNode()->createChildSceneNode("charCameraNode");
	_charNode->setPosition(_charCamera->getPosition());
	_charNode->setOrientation(_charCamera->getOrientation());
	_charNode->attachObject(_charCamera);

	return;
}

void GameManager::updateCharacterController(Ogre::Camera* camera)
{
	//update bullet character controller and apply transformations to ogre camera
	//if no camera is available, then don't apply to a camera
	if(!camera && !_charCamera)
	{
		//for now return
		return;
	}
	//if a camera is passed in, but there isn't one set already then set the internal camera to the passed-in one.
	if(camera && !_charCamera)
	{
		_charCamera = camera;
	}

	//use internal camera
	Ogre::Quaternion oRot; btQuaternion btRot;
	btTransform camTrans = _charGhost->getWorldTransform();
	btRot = camTrans.getRotation();
	oRot.x = btRot.x();
	oRot.y = btRot.y();
	oRot.z = btRot.z();
	oRot.w = btRot.w();
	Ogre::Vector3 oPos = convertBulletVector3(camTrans.getOrigin());
	//update camera position/rotation
	//should I use a scene node instead...?
	_charNode->setPosition(oPos);
	_charNode->setOrientation(oRot);

}

OgreBulletPair GameManager::createObject(Ogre::SceneManager* scene,object_t* objectInfo)
{
	//return variable
	OgreBulletPair retVal;

	//Ogre part of this arrangement.
	Ogre::SceneNode* node = OgreManager::getSingleton().createSceneNode(scene,objectInfo);
	retVal.ogreNode = node;

	btCollisionShape* shape = NULL;

	//Only do this if it's an object.
	if(objectInfo->type() == "entity")
	{
		if(objectInfo->mass()!=0.0f)
		{
			//get the collision shape.
			shape = BulletManager::getSingleton().generateCollisionShape(objectInfo);
		}
		else
		{
			//assumes that all non-mass objects will be static triangle meshes.
			shape = buildTriangleCollisionShape(node);
		}
		
		btTransform init;
		init.setIdentity();
		init.setOrigin(btVector3(objectInfo->positionX(),objectInfo->positionY(),objectInfo->positionZ()));

		//Easy function call.
		retVal.btBody=BulletManager::getSingleton().addRigidBody(shape,node,objectInfo->mass(),init);
	}


	//return by-value, not reference.
	return retVal;
}

OgreBulletPair GameManager::createObject(Ogre::SceneNode* node,object_t* objectInfo)
{
	//Here's the variable that'll be passed back(by-value, not reference!).
	OgreBulletPair retVal;

	//That's dead simple, it's already passed in!
	retVal.ogreNode=node;

	//Easy(ish) function call.
	btCollisionShape* shape = NULL;
	if(objectInfo->mass()!=0.0f)
	{
		shape = BulletManager::getSingleton().generateCollisionShape(objectInfo);
	}
	else
	{
		shape = buildTriangleCollisionShape(node);
	}
	btTransform init; init.setIdentity();
	btVector3 pos;
	pos.setX(objectInfo->positionX());
	pos.setY(objectInfo->positionY());
	pos.setZ(objectInfo->positionZ());
	retVal.btBody = BulletManager::getSingleton().addRigidBody(shape,node,objectInfo->mass(),init);

	return retVal;
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

btBvhTriangleMeshShape* GameManager::buildTriangleCollisionShape(Ogre::SceneNode* node)
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