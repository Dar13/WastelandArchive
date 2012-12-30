#include "StdAfx.h"
#include "interfaces\interfaces.h"

#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_

#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "BulletDebugDraw\DebugDraw.hpp"

/*! \brief This class manages all of Bullet Physics.

Performs various tasks specific to Bullet Physics, is mainly self-contained.
*/

class PhysicsManager
{
public:
	//! Initializes all Bullet pointers to zero.
	PhysicsManager();
	~PhysicsManager();

	//! Sets up Bullet Physics.
	void Setup(btVector3& gravitySpeeds);
	//! Steps the Bullet Physics simulation.
	/*! 
		\param deltaTime Elapsed time represented in seconds.
	*/
	void Update(float deltaTime);
	//! Shutsdown Bullet, deleting all collision shapes and rigid bodies.
	/*!
		\param reuse If true,tells the function to spare the main Bullet pointers, allowing their reuse in a 'new' simulation.
	*/
	void Shutdown(bool reuse = false);

	//! Sets the btVector3 used for gravity in the current Bullet simulation.
	/*!
		\param gravity the btVector3 used to calculate the gravity forces used on all applicable rigid bodies.
	*/
	void setGravity(btVector3 &gravity);

	//! Creates and adds a rigid body to the Bullet simulation.
	/*!
		\param shape A Bullet collision shape.
		\param node The visual representation of the rigid body in Ogre3D.
		\param mass The mass of the rigid body.
		\param initTrans The initial position/rotation of the rigid body in the simulation.
	*/
	btRigidBody* addRigidBody(btCollisionShape* shape,Ogre::SceneNode* node, btScalar &mass, btTransform &initTransform);

	btPoint2PointConstraint* createBallSocketConstraint(btRigidBody* bodyA,const btVector3& pivotA,bool disableCollisions = false);
	btPoint2PointConstraint* createBallSocketConstraint(btRigidBody* bodyA, btRigidBody* bodyB, 
														const btVector3& pivotA,const btVector3& pivotB,
														bool disableCollisions = false);

	btHingeConstraint* createHingeConstraint(btRigidBody* bodyA, const btTransform& frameA, 
											 bool referenceFrameA = false);
	btHingeConstraint* createHingeConstraint(btRigidBody* bodyA, const btVector3& pivotA, 
											 btVector3& axisA, bool referenceFrameA = false);
	btHingeConstraint* createHingeConstraint(btRigidBody* bodyA, btRigidBody* bodyB, 
											 const btVector3& pivotA, const btVector3& pivotB, 
											 btVector3& axisA, btVector3& axisB, bool referenceFrameA = false);
	btHingeConstraint* createHingeConstraint(btRigidBody* bodyA, btRigidBody* bodyB, 
											 const btTransform& frameA, const btTransform& frameB, 
											 bool referenceFrameA = false);

	btCollisionShape* generateCollisionShape(object_t* objectInfo);
	
	//! Returns the Bullet Physics world pointer.
	btDiscreteDynamicsWorld* getWorld(){return _World;}

	//! Sets Debug Drawer variable. This class takes over the responsibility of cleaning it up.
	void setDebugDrawer(CDebugDraw* drawer);

	//! Returns bool if anything is hit. Position and normal are filled, start and end must be provided.
	bool RaycastWorld_Closest(const btVector3& start, const btVector3& end, btVector3& position, btVector3& normal);

private:

	//Bullet pointers
	btDiscreteDynamicsWorld* _World;
	btBroadphaseInterface* _OverlapPairCache;
	btSequentialImpulseConstraintSolver* _Solver;
	btCollisionDispatcher* _Dispatch;
	btDefaultCollisionConfiguration* _Config;

	//Holds all the collision shapes we need to get rid of.
	btAlignedObjectArray<btCollisionShape*> _Shapes;

	//Current gravity.
	btVector3 _Gravity;

	//Debug Drawer variable.
	CDebugDraw* _debugDrawer;

};

/*! \brief This class transfers Bullet transformations to Ogre scene nodes.

Is the primary vehicle of interaction between Bullet Physics and Ogre3D.
*/

class OgreMotionState : public btMotionState
{
public:
	//! Contructor
	/*!
		\param initialPosition The initial transformation to be applied.
		\param node The SceneNode that all transformations will be applied to.
	*/
	OgreMotionState(const btTransform &initialPosition, Ogre::SceneNode* node);
	virtual ~OgreMotionState();

	//! Sets the node of the MotionState.
	void setNode(Ogre::SceneNode* node);

	//! Gets the current transformation.
	virtual void getWorldTransform(btTransform &worldTrans) const ;

	//! Sets the current transformation.
	virtual void setWorldTransform(const btTransform &worldTrans);

private:
	Ogre::SceneNode* _Object;
	btTransform _ObjectTransform;
};

#endif