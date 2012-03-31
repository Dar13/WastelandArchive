#include "StdAfx.h"
#include "GameManager.h"
#include "debug\print.h"

//=======================================
/*
(!!!)NOTE(!!!)
	Bullet units are 1 unit = 1 meter.
	So that means that OGRE units are now 1 unit = 1 meter.
*/
//=======================================

namespace GameManager
{

	bool UpdateManagers(OgreManager* graphicsManager,BulletManager* phyManager,float deltaTime)
	{
		bool retVal = true;

		//Update Bullet
		phyManager->Update(deltaTime /  1000.0f);

		//if the debug printer is active, update it
		if(DebugPrint::getSingleton().isActive())
		{
			DebugPrint::getSingleton().Update();
		}

		//Update Ogre
		Ogre::WindowEventUtilities::messagePump();
		//If an error occurred...
		if(!graphicsManager->Render())
		{
			//state needs to end
			retVal=false;
		}

		return retVal;
	}

	void setDebugDrawer(Ogre::SceneManager* scene,BulletManager* phyManager)
	{
		CDebugDraw* draw = new CDebugDraw(scene,phyManager->getWorld());
		phyManager->setDebugDrawer(draw);
	}

	//just a wrapper for the createObject function. Gets rid of some of the code needed in the states.
	OgreBulletPair createObject(Ogre::SceneManager* scene,std::string& file,BulletManager* phyManager,OgreManager* graphics)
	{
		OgreBulletPair ret;
		object_t* obj = object(file.c_str()).release();
		ret = createObject(scene,obj,phyManager,graphics);
		delete obj;

		return ret;
	}

	OgreBulletPair createObject(Ogre::SceneManager* scene,object_t* objectInfo,BulletManager* phyManager,OgreManager* graphics)
	{
		//return variable
		OgreBulletPair retVal;

		//Ogre part of this arrangement.
		Ogre::SceneNode* node = graphics->createSceneNode(scene,objectInfo,NULL);
		retVal.ogreNode = node;

		btCollisionShape* shape = NULL;

		//Only do this if it's an object.
		if(objectInfo->type() == "entity")
		{
			if(objectInfo->mass()!=0.0f)
			{
				//get the collision shape.
				shape = phyManager->generateCollisionShape(objectInfo);
				//shape = BulletManager::getSingleton().generateCollisionShape(objectInfo);
			}
			else
			{
				//assumes that all non-mass objects will be static triangle meshes.
				//unless otherwise told
				if(objectInfo->collisionShape() == "TriangleMesh")
				{
					shape = buildTriangleCollisionShape(node);
				}
				else
				{
					shape = phyManager->generateCollisionShape(objectInfo);
					//shape = BulletManager::getSingleton().generateCollisionShape(objectInfo);
				}
			}
		
			btTransform init;
			init.setIdentity();
			init.setOrigin(btVector3(objectInfo->positionX(),objectInfo->positionY(),objectInfo->positionZ()));

			//Easy function call.
			retVal.btBody = phyManager->addRigidBody(shape,node,objectInfo->mass(),init);
			//retVal.btBody=BulletManager::getSingleton().addRigidBody(shape,node,objectInfo->mass(),init);
		}

		//return by value, not reference.
		return retVal;
	}

	OgreBulletPair createObject(Ogre::SceneNode* node,object_t* objectInfo,BulletManager* phyManager)
	{
		//Here's the variable that'll be passed back(by-value, not reference!).
		OgreBulletPair retVal;

		//That's dead simple, it's already passed in!
		retVal.ogreNode=node;

		//Easy(ish) function call.
		btCollisionShape* shape = NULL;
		if(objectInfo->mass()!=0.0f)
		{
			shape = phyManager->generateCollisionShape(objectInfo);
			//shape = BulletManager::getSingleton().generateCollisionShape(objectInfo);
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
		retVal.btBody = phyManager->addRigidBody(shape,node,objectInfo->mass(),init);
		//retVal.btBody = BulletManager::getSingleton().addRigidBody(shape,node,objectInfo->mass(),init);

		return retVal;
	}

	//----------------------------------------
	//Private, utility functions start here..
	//----------------------------------------

	//manually builds triangle mesh collision shape.
	btBvhTriangleMeshShape* buildTriangleCollisionShape(Ogre::SceneNode* node)
	{
		btBvhTriangleMeshShape* shape;
		btTriangleMesh* bmesh = new btTriangleMesh();

		Ogre::MeshPtr mesh = ((Ogre::Entity*)node->getAttachedObject(0))->getMesh();
		size_t vertCnt,inCnt;
		Ogre::Vector3* vertices;
		unsigned long* indices;

		OgreManager::getSingleton().getMeshInformation(&mesh,vertCnt,vertices,inCnt,indices);

		btVector3 vert1,vert2,vert3;

		for(unsigned int i=0; i<inCnt; i+=3)
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

};