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
	GUN_TYPE _correspondGunType(const std::string& typ);
	GUN_NAME _correspondGunName(const std::string& name);

	bool UpdateManagers(GraphicsManager* graphicsManager,PhysicsManager* phyManager,float deltaTime)
	{
		bool retVal = true;

		//Update Bullet
		if(phyManager != NULL)
			phyManager->Update(deltaTime /  1000.0f);

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

	void setDebugDrawer(Ogre::SceneManager* scene,PhysicsManager* phyManager)
	{
		CDebugDraw* draw = new CDebugDraw(scene,phyManager->getWorld());
		phyManager->setDebugDrawer(draw);
	}

	//just a wrapper for the createObject function. Gets rid of some of the code needed in the states.
	OgreBulletPair createObject(Ogre::SceneManager* scene,std::string& file,PhysicsManager* phyManager,GraphicsManager* graphicsManager)
	{
		OgreBulletPair ret;
		object_t* obj = object(file.c_str()).release();
		ret = createObject(scene,obj,phyManager,graphicsManager);
		delete obj;

		return ret;
	}

	OgreBulletPair createObject(Ogre::SceneManager* scene,object_t* objectInfo,PhysicsManager* phyManager,GraphicsManager* graphicsManager)
	{
		//return variable
		OgreBulletPair retVal;

		//Ogre part of this arrangement.
		Ogre::SceneNode* node = graphicsManager->createSceneNode(scene,objectInfo,NULL);
		retVal.ogreNode = node;

		btCollisionShape* shape = NULL;

		//Only do this if it's an object.
		if(objectInfo->type() == "entity")
		{
			if(objectInfo->mass()!=0.0f)
			{
				//get the collision shape.
				shape = phyManager->generateCollisionShape(objectInfo);
				//shape = PhysicsManager::getSingleton().generateCollisionShape(objectInfo);
			}
			else
			{
				//assumes that all non-mass objects will be static triangle meshes.
				//unless otherwise told
				if(objectInfo->collisionShape() == "TriangleMesh")
				{
					shape = buildTriangleCollisionShape(node,graphicsManager);
				}
				else
				{
					shape = phyManager->generateCollisionShape(objectInfo);
					//shape = PhysicsManager::getSingleton().generateCollisionShape(objectInfo);
				}
			}
		
			btTransform init;
			init.setIdentity();
			init.setOrigin(btVector3(objectInfo->positionX(),objectInfo->positionY(),objectInfo->positionZ()));

			//Easy function call.
			retVal.btBody = phyManager->addRigidBody(shape,node,objectInfo->mass(),init);
			//retVal.btBody=PhysicsManager::getSingleton().addRigidBody(shape,node,objectInfo->mass(),init);
		}

		//return by value, not reference.
		return retVal;
	}

	OgreBulletPair createObject(Ogre::SceneNode* node,object_t* objectInfo,PhysicsManager* phyManager,GraphicsManager* graphicsManager)
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
			//shape = PhysicsManager::getSingleton().generateCollisionShape(objectInfo);
		}
		else
		{
			shape = buildTriangleCollisionShape(node,graphicsManager);
		}
		btTransform init; init.setIdentity();
		btVector3 pos;
		pos.setX(objectInfo->positionX());
		pos.setY(objectInfo->positionY());
		pos.setZ(objectInfo->positionZ());
		retVal.btBody = phyManager->addRigidBody(shape,node,objectInfo->mass(),init);
		//retVal.btBody = PhysicsManager::getSingleton().addRigidBody(shape,node,objectInfo->mass(),init);

		return retVal;
	}

	EquippableObject createEquippable(Ogre::SceneManager* scene,
									  const std::string& file,
									  GraphicsManager* graphicsManager,
									  bool isWeapon = true)
	{
		EquippableObject retVal;
		baseEquippable b;
		b.setEquipped(false);
		if(isWeapon)
		{
			weapon_t* wep = weapon(file.c_str()).release();
			b.setWeapon(true);
			retVal.equip.reset(new cGunData(b,
											_correspondGunType(wep->type()),
											_correspondGunName(wep->name()),
											static_cast<int>(wep->gameplay().reloadQty()),4)
							                );
			retVal.node = graphicsManager->createSceneNode(scene,wep,nullptr);
			static_cast<cGunData*>(retVal.equip.get())->setSoundFrames(wep);
			delete wep;
		}
		else
		{
			//fill this in later.
		}

		return retVal;
	}

	//----------------------------------------
	//Private, utility functions start here..
	//----------------------------------------

	GUN_TYPE _correspondGunType(const std::string& typ)
	{
		if(typ == "PISTOL")
		{
			return PISTOL;
		}
		if(typ == "ASSAULT")
		{
			return ASSAULT;
		}
		if(typ == "SMG")
		{
			return SMG;
		}
		if(typ == "HEAVY")
		{
			return HEAVY;
		}
		if(typ == "SHOTGUN")
		{
			return SHOTGUN;
		}

		return NO_TYPE;
	}

	GUN_NAME _correspondGunName(const std::string& name)
	{
		if(name == "M9SE")
		{
			return M9;
		}
		if(name == "G36C")
		{
			return G36C;
		}
		return NO_NAME;
	}

	//manually builds triangle mesh collision shape.
	btBvhTriangleMeshShape* buildTriangleCollisionShape(Ogre::SceneNode* node,GraphicsManager* Graphics)
	{
		btBvhTriangleMeshShape* shape;
		btTriangleMesh* bmesh = new btTriangleMesh();

		Ogre::MeshPtr mesh = ((Ogre::Entity*)node->getAttachedObject(0))->getMesh();
		size_t vertCnt,inCnt;
		Ogre::Vector3* vertices;
		unsigned long* indices;

		Graphics->getMeshInformation(&mesh,vertCnt,vertices,inCnt,indices);

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