#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_

#include <OgreSingleton.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class BulletManager : public Ogre::Singleton<BulletManager>
{
public:
	BulletManager();
	~BulletManager();

	void Setup();
	void Update(float deltaTime);
	void Shutdown(bool reuse = false);

	void setGravity(btVector3 &gravity);
	void addRigidBody(btCollisionShape* shape,Ogre::SceneNode* node, btScalar &mass, btTransform &initTransform);
	
	btDiscreteDynamicsWorld* getWorld();

private:
	//Needed for this class to be a singleton
	BulletManager(const BulletManager&);
	BulletManager& operator=(const BulletManager&);

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

};

class OgreMotionState : public btMotionState
{
public:
	//Construction/destruction
	OgreMotionState(const btTransform &initialPosition, Ogre::SceneNode* node);
	virtual ~OgreMotionState();

	//other stuff
	void setNode(Ogre::SceneNode* node);

	//Bullet stuff.
	virtual void getWorldTransform(btTransform &worldTrans) const ;

	virtual void setWorldTransform(const btTransform &worldTrans);

private:
	Ogre::SceneNode* _Object;
	btTransform _ObjectTransform;
};

#endif