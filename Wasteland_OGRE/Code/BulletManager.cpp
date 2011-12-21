#include "StdAfx.h"
#include "BulletManager.h"

//====================
// Bullet Manager
//====================

template<> BulletManager* Ogre::Singleton<BulletManager>::ms_Singleton = 0;

BulletManager::BulletManager()
{
	_World = 0;
	_OverlapPairCache = 0;
	_Dispatch = 0;
	_Config = 0;
	_Solver = 0;
	_Gravity = btVector3(0,0,0);
	_debugDrawer = 0;
}

BulletManager::~BulletManager()
{
	Shutdown(false);
}

void BulletManager::Setup()
{
	_Config = new btDefaultCollisionConfiguration();
	_Dispatch = new btCollisionDispatcher(_Config);
	_OverlapPairCache = new btDbvtBroadphase();
	_Solver = new btSequentialImpulseConstraintSolver();
	_World = new btDiscreteDynamicsWorld(_Dispatch,_OverlapPairCache,_Solver,_Config);
}

void BulletManager::setDebugDrawer(CDebugDraw* drawer)
{
	_debugDrawer = drawer;
}

btRigidBody* BulletManager::addRigidBody(btCollisionShape* shape,Ogre::SceneNode* node,btScalar &mass,btTransform &initTransform)
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

void BulletManager::setGravity(btVector3 &gravity)
{
	_Gravity = gravity;
	_World->setGravity(_Gravity);
}

//deltaTime will be how many seconds have passed(hopefully less than 1).
void BulletManager::Update(float deltaTime)
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

btCollisionShape* BulletManager::generateCollisionShape(object_t* objectInfo)
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
		//implementation is for another day however.
		retVal = NULL;
	}

	return retVal;
}

void BulletManager::Shutdown(bool reuse)
{
	//Deletes all rigid bodies and collision shapes, basically cleans out the class.
	//rigid bodies
	for(int i=_World->getNumCollisionObjects()-1; i>=0; --i)
	{
		btCollisionObject* obj = _World->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if(body && body->getMotionState())
		{
			delete body->getMotionState();
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