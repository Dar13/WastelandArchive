#include "StdAfx.h"
//additional physics classes
#include <btBulletDynamicsCommon.h>
#include "BulletDynamics\Character\btKinematicCharacterController.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
//input class
#include "InputManager.h"
#include "GraphicsManager.h"

class CharacterController
{
public:
	CharacterController();
	CharacterController(Ogre::Camera* camera,const Ogre::Vector3& initialPosition,const Ogre::Vector3& initialDirection,btDiscreteDynamicsWorld* phyWorld,GraphicsManager* Graphics)
	{
		create(camera,initialPosition,initialDirection,phyWorld,Graphics);
	}
	~CharacterController();

	void create(Ogre::Camera* camera,const Ogre::Vector3& initialPosition,const Ogre::Vector3& initialDirection,btDiscreteDynamicsWorld* phyWorld,GraphicsManager* Graphics);

	void update(float physicsTimeElapsed,InputManager* inputManager, OgreTransform& transform);

	Ogre::SceneNode* getNode() { return cNode; }

	void shutdown() { _world->removeAction(cController); _world->removeCollisionObject(cGhostObject); }

private:
	//prefix 'c' to denote privateness AND to differentiate from other variables.
	//prefix '_' to denote privateness with no need to differentiate from other variables.
	Ogre::Camera* cCamera;
	Ogre::SceneNode* cNode;

	btGhostPairCallback __pairCallback;

	btKinematicCharacterController* cController;
	btPairCachingGhostObject* cGhostObject;
	btDiscreteDynamicsWorld* _world;
};