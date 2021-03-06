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
	cGunData::GUN_TYPE _correspondGunType(const std::string& typ);
	cGunData::GUN_NAME _correspondGunName(const std::string& name);

	void RenderScene(GraphicsManager* Graphics,Ogre::Viewport* view)
	{
		Graphics->getRoot()->getRenderSystem()->_setViewport(view);
		Graphics->getRoot()->getRenderSystem()->clearFrameBuffer(Ogre::FBT_COLOUR | Ogre::FBT_DEPTH);

		Graphics->getRenderWindow()->update(false);
		Graphics->getRenderWindow()->swapBuffers(true);
	}

	bool UpdateManagers(GraphicsManager* graphicsManager,PhysicsManager* phyManager,float deltaTime)
	{
		bool retVal = true;

		//Update Bullet
		if(phyManager != nullptr)
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
		Ogre::SceneNode* node = GraphicsManager::createSceneNode(scene,objectInfo,NULL);

		btCollisionShape* shape = NULL;

		//Only do this if it's an object.
		if(objectInfo->type() == "entity")
		{
			if(fabs(0.0f - objectInfo->mass()) > std::numeric_limits<float>::epsilon())
			{
				//get the collision shape.
				shape = phyManager->generateCollisionShape(objectInfo);
			}
			else
			{
				//assumes that all non-mass objects will be static triangle meshes.
				//unless otherwise told
				if(objectInfo->collisionShape() == "TriangleMesh")
				{
					shape = buildTriangleCollisionShape(node,graphicsManager);
					node->getAttachedObject(0)->setQueryFlags(LEVEL_MASK);
				}
				else
				{
					shape = phyManager->generateCollisionShape(objectInfo);
					node->getAttachedObject(0)->setQueryFlags(SCENERY_MASK);
				}
			}
		
			btTransform init;
			init.setIdentity();
			init.setOrigin(btVector3(objectInfo->positionX(),objectInfo->positionY(),objectInfo->positionZ()));

			//Easy function call.
			if(shape != NULL)
			{
				retVal.btBody = phyManager->addRigidBody(shape,node,objectInfo->mass(),init);
			}
			else
			{
				retVal.btBody = NULL;
			}
		}

		retVal.ogreNode = node;

		//return by value, not reference.
		return retVal;
	}

	Ogre::SceneNode* createObject(Ogre::SceneManager* scene,std::string& file,GraphicsManager* graphics)
	{
		object_t* obj = object(file).get();

		Ogre::SceneNode* node = graphics->createSceneNode(scene,obj,NULL);

		delete obj;

		return node;
	}

	OgreBulletPair createObject(Ogre::SceneNode* node,object_t* objectInfo,PhysicsManager* phyManager,GraphicsManager* graphicsManager)
	{
		//Here's the variable that'll be passed back(by-value, not reference!).
		OgreBulletPair retVal;

		//Easy(ish) function call.
		btCollisionShape* shape = NULL;
		if(fabs(0.0f - objectInfo->mass()) > std::numeric_limits<float>::epsilon())
		{
			shape = phyManager->generateCollisionShape(objectInfo);
			node->getAttachedObject(0)->setQueryFlags(SCENERY_MASK);
		}
		else
		{
			shape = buildTriangleCollisionShape(node,graphicsManager);
			node->getAttachedObject(0)->setQueryFlags(LEVEL_MASK);
		}
		btTransform init; init.setIdentity();
		btVector3 pos;
		pos.setX(objectInfo->positionX());
		pos.setY(objectInfo->positionY());
		pos.setZ(objectInfo->positionZ());
		retVal.btBody = phyManager->addRigidBody(shape,node,objectInfo->mass(),init);
		
		//That's dead simple, it's already passed in!
		retVal.ogreNode=node;

		return retVal;
	}

	Ogre::SceneNode* createCharacterObject(Ogre::SceneManager* scene,
										   characterobject_t* obj,
										   GraphicsManager* graphics)
	{
		Ogre::SceneNode* node = nullptr;

		node = GraphicsManager::createSceneNode(scene,obj);
		node->getAttachedObject(0)->setQueryFlags(CHARACTER_MASK);

		if(node == nullptr)
		{
			std::cout << "Error creating CharacterObject SceneNode!" << std::endl;
			std::cout << "Character name:" << obj->name() << std::endl;
		}

		return node;
	}

	EquippableObject createEquippable(Ogre::SceneManager* scene,
									  const std::string& file,
									  GraphicsManager* graphicsManager,
									  SoundManager* soundManager,
									  bool isWeapon)
	{
		EquippableObject retVal;
		retVal.equip = nullptr; retVal.node = nullptr;
		baseEquippable b;
		b.setEquipped(false);
		if(isWeapon)
		{
			weapon_t* wep = weapon(file.c_str()).release();
			b.setWeapon(true);
			retVal.equip = new cGunData(b,
										_correspondGunType(wep->type()),
										_correspondGunName(wep->name()),
										static_cast<int>(wep->gameplay().reloadQty()),4);
			retVal.node = graphicsManager->createSceneNode(scene,wep,nullptr);
			retVal.node->getAttachedObject(0)->setQueryFlags(SCENERY_MASK);
			static_cast<cGunData*>(retVal.equip)->setAccuracyRadius(static_cast<int>(wep->gameplay().accuracy()));
			static_cast<cGunData*>(retVal.equip)->setDamagePerBullet(static_cast<int>(wep->gameplay().damage()));
			static_cast<cGunData*>(retVal.equip)->setEffectiveRange(static_cast<int>(wep->gameplay().range()));
			static_cast<cGunData*>(retVal.equip)->setFireRate(static_cast<int>(wep->gameplay().firerate()));
			static_cast<cGunData*>(retVal.equip)->setSoundFrames(wep,soundManager);
			static_cast<cGunData*>(retVal.equip)->setAnimationFrames(static_cast<Ogre::Entity*>(retVal.node->getAttachedObject(0)));
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

	cGunData::GUN_TYPE _correspondGunType(const std::string& typ)
	{
		if(typ == "PISTOL")
		{
			return cGunData::PISTOL;
		}
		if(typ == "ASSAULT")
		{
			return cGunData::ASSAULT;
		}
		if(typ == "SMG")
		{
			return cGunData::SMG;
		}
		if(typ == "HEAVY")
		{
			return cGunData::HEAVY;
		}
		if(typ == "SHOTGUN")
		{
			return cGunData::SHOTGUN;
		}

		return cGunData::NO_TYPE;
	}

	cGunData::GUN_NAME _correspondGunName(const std::string& name)
	{
		if(name == "M9SE")
		{
			return cGunData::M9;
		}
		if(name == "G36C")
		{
			return cGunData::G36C;
		}
		return cGunData::NO_NAME;
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

		//delete bmesh;
		shape->setUserPointer(static_cast<void*>(bmesh));

		return shape;
	}

};