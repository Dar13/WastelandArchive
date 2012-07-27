#include "StdAfx.h"
#include "PhysicsManager.h"

//====================
// Bullet Manager
//====================

PhysicsManager::PhysicsManager()
{
	//Sets everything to NULL.
	_World = 0;
	_OverlapPairCache = 0;
	_Dispatch = 0;
	_Config = 0;
	_Solver = 0;
	_Gravity = btVector3(0,0,0);
	_debugDrawer = 0;
}

PhysicsManager::~PhysicsManager()
{
	Shutdown(false);

	delete _debugDrawer;
}

void PhysicsManager::Setup()
{
	_Config = new btDefaultCollisionConfiguration();
	_Dispatch = new btCollisionDispatcher(_Config);
	_OverlapPairCache = new btDbvtBroadphase();
	_Solver = new btSequentialImpulseConstraintSolver();
	_World = new btDiscreteDynamicsWorld(_Dispatch,_OverlapPairCache,_Solver,_Config);
}

void PhysicsManager::setDebugDrawer(CDebugDraw* drawer)
{
	_debugDrawer = drawer;
}

btRigidBody* PhysicsManager::addRigidBody(btCollisionShape* shape,Ogre::SceneNode* node,btScalar &mass,btTransform &initTransform)
{
	_Shapes.push_back(shape);

	btVector3 inertia(0,0,0);
	if(mass != 0.0f)
	{
		shape->calculateLocalInertia(mass,inertia);
	}
	OgreMotionState* motState = new OgreMotionState(initTransform,node);

	btRigidBody::btRigidBodyConstructionInfo rbinfo(mass,motState,shape,inertia);
	btRigidBody* body = new btRigidBody(rbinfo);

	_World->addRigidBody(body);

	return body;
}

btPoint2PointConstraint* PhysicsManager::createBallSocketConstraint(btRigidBody* bodyA,const btVector3& pivotA,bool disableCollisions)
{
	btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*bodyA,pivotA);
	_World->addConstraint(p2p,disableCollisions);
	return p2p;
}

btPoint2PointConstraint* PhysicsManager::createBallSocketConstraint(btRigidBody* bodyA,btRigidBody* bodyB,const btVector3& pivotA,const btVector3& pivotB,bool disableCollisions)
{
	btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*bodyA,*bodyB,pivotA,pivotB);
	_World->addConstraint(p2p,disableCollisions);
	return p2p;
}

btHingeConstraint* PhysicsManager::createHingeConstraint(btRigidBody* bodyA, const btTransform& frameA, bool referenceFrameA)
{
	btHingeConstraint* hinge = new btHingeConstraint(*bodyA,frameA,referenceFrameA);
	_World->addConstraint(hinge,false);
	return hinge;
}

btHingeConstraint* PhysicsManager::createHingeConstraint(btRigidBody* bodyA, const btVector3& pivotA, btVector3& axisA, bool referenceFrameA)
{
	btHingeConstraint* hinge = new btHingeConstraint(*bodyA,pivotA,axisA,referenceFrameA);
	_World->addConstraint(hinge,true);
	return hinge;
}

btHingeConstraint* PhysicsManager::createHingeConstraint(btRigidBody* bodyA, btRigidBody* bodyB, 
														 const btVector3& pivotA, const btVector3& pivotB, 
														 btVector3& axisA, btVector3& axisB, bool referenceFrameA)
{
	btHingeConstraint* hinge = new btHingeConstraint(*bodyA,*bodyB,pivotA,pivotB,axisA,axisB,referenceFrameA);
	_World->addConstraint(hinge,true);
	return hinge;
}

btHingeConstraint* PhysicsManager::createHingeConstraint(btRigidBody* bodyA, btRigidBody* bodyB, 
														 const btTransform& frameA, const btTransform& frameB, 
														 bool referenceFrameA)
{
	btHingeConstraint* hinge = new btHingeConstraint(*bodyA,*bodyB,frameA,frameB,referenceFrameA);
	_World->addConstraint(hinge,false);
	return hinge;
}

void PhysicsManager::setGravity(btVector3 &gravity)
{
	_Gravity = gravity;
	_World->setGravity(_Gravity);
}

//deltaTime will be how many seconds have passed(hopefully less than 1).
void PhysicsManager::Update(float deltaTime)
{
	int subSteps = 0;
	if(deltaTime < (0.03333333333f))
	{
		//just an added substep, because it's lower than recommended.
		subSteps = (int)ceil(deltaTime*60)+1;
	}
	else
	{
		//works fine for higher framerates(lower deltaTimes)
		subSteps = (int)ceil(deltaTime*60);
	}

	_World->stepSimulation(btScalar(deltaTime),subSteps);

	if(_debugDrawer)
	{
		_debugDrawer->Update();
	}
}

btCollisionShape* PhysicsManager::generateCollisionShape(object_t* objectInfo)
{
	btCollisionShape* retVal = NULL;
	std::string type = objectInfo->collisionShape();

	//TriangleMesh isn't this particular function's responsibility.

	if(type == "Sphere")
	{
		retVal = new btSphereShape((btScalar)objectInfo->colSphereRadius());
	}
	
	if(type == "Box")
	{
		retVal = new btBoxShape(btVector3(objectInfo->colBoxWidth(),objectInfo->colBoxHeight(),objectInfo->colBoxDepth()));
	}

	if(type == "Cube")
	{
		retVal = new btBoxShape(btVector3((btScalar)objectInfo->colCubeSize(),(btScalar)objectInfo->colCubeSize(),(btScalar)objectInfo->colCubeSize()));
	}

	if(type == "Capsule")
	{
		retVal = new btCapsuleShape(objectInfo->colCapsuleWidth(),objectInfo->colCapsuleHeight());
	}

	if(type == "Custom")
	{
		//denotes previously serialized shape, need to load it in.
		//implementation is for another day.
		retVal = NULL;
	}

	if(type == "NULL")
	{
		//denotes that the object doesn't need a rigid body representative.
		retVal = nullptr;
	}

	return retVal;
}

bool PhysicsManager::RaycastWorld_Closest(const btVector3& start,const btVector3& end, btVector3& position, btVector3& normal)
{
	//structure that will hold the results
	btCollisionWorld::ClosestRayResultCallback rayResult(start,end);
	
	//perform the raycast, if _World is defined.
	if(_World)
	{
		_World->rayTest(start,end,rayResult);
		if(rayResult.hasHit())
		{
			//set the position
			position = rayResult.m_hitPointWorld;
			//set the normal
			normal =rayResult.m_hitNormalWorld;
			return true;
		}
	}

	//is only executed when _World is set to NULL.
	return false;

}

void PhysicsManager::Shutdown(bool reuse)
{
	
	int i;
	//constraints
	for(i = _World->getNumConstraints() - 1; i>= 0; --i)
	{
		btTypedConstraint* con = _World->getConstraint(i);
		_World->removeConstraint(con);
		delete con;
	}

	//Deletes all rigid bodies and collision shapes, basically cleans out the class.
	//rigid bodies
	for(i=_World->getNumCollisionObjects()-1; i>=0; --i)
	{
		btCollisionObject* obj = _World->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if(body != NULL)
		{
			if(body->getMotionState() != NULL)
			{
				delete body->getMotionState();
			}
		}
		_World->removeCollisionObject(obj);
		delete obj;
	}
	
	//Collision shapes
	for(int j=0; j<_Shapes.size(); ++j)
	{
		btCollisionShape* shape = _Shapes[j];
		_Shapes[j] = 0;
		delete shape;
	}
	_Shapes.clear(); //cleans up the vector.
	

	if(!reuse)
	{
		//Delete all the Bullet pointers.
		//in opposite creation order.
		if(_debugDrawer)
		{
			delete _debugDrawer;
		}
		delete _World;
		delete _Solver;
		delete _OverlapPairCache;
		delete _Dispatch;
		delete _Config;
	}
}

//====================
// Ogre MotionState
//====================
OgreMotionState::OgreMotionState(const btTransform &initialPosition,Ogre::SceneNode* node)
{
	_Object = node;
	_ObjectTransform = initialPosition;
}

//No implementation.
OgreMotionState::~OgreMotionState(){}

void OgreMotionState::setNode(Ogre::SceneNode* node)
{
	_Object = node;
}

void OgreMotionState::getWorldTransform(btTransform &worldTrans) const 
{
	worldTrans = _ObjectTransform;
}

void OgreMotionState::setWorldTransform(const btTransform &worldTrans)
{
	//Make sure there's a node to apply transform to.
	if(_Object == 0)
		return;

	btQuaternion rot = worldTrans.getRotation();
	_Object->setOrientation(rot.w(),rot.x(),rot.y(),rot.z());
	btVector3 pos = worldTrans.getOrigin();
	_Object->setPosition(pos.x(),pos.y(),pos.z());
}