#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <OgreSingleton.h>
#include "OgreManager.h"
#include "OISManager.h"
#include "BulletManager.h"

/*==========================================================
This is the class where all the different Managers
(Ogre,Bullet,OIS,Sound,etc.)will interact in order to 
perform certain tasks(example: building a btBvhTriangleMeshShape
from an Ogre::Mesh. This is the abstraction upon which the
majority of the game code will be built, avoiding reliance
on the individual managers.
==========================================================*/
struct OgreBulletPair
{
	Ogre::SceneNode* ogreNode;
	btRigidBody* btBody;
};

class GameManager : public Ogre::Singleton<GameManager>
{
public:
	GameManager();
	~GameManager(); 
	
	/*!
	Creates a level, complete with a Bullet Physics rigid body.
	List of options:
	filename = filename for the mesh (MANDATORY)
	resgroup = name of resource group (MANDATORY)
	name = name for the entity and scenenode (MANDATORY)
	*/
	Ogre::SceneNode* createLevel(Ogre::SceneManager* scene,std::map<std::string,std::string> &options);

	/*
	Creates an object, complete with Bullet Physics rigid body.
	Options:
	ogre_specific(OGRE)
	----------------------------------------------
	filename = filename for mesh(OGRE)
	resgroup = name of resource group(OGRE)
	name = name of ogre entity/scenenode(OGRE)
	----------------------------------------------
	*/
	OgreBulletPair createObject(Ogre::SceneManager* scene,
								std::map<std::string,std::string> &options,
								btScalar &mass, 
								btTransform &init,
								btCollisionShape* shape);
	//Overload of above.
	OgreBulletPair createObject(Ogre::SceneNode* node,
								btCollisionShape* shape,
								btScalar &mass,
								btTransform &init);

	//Create a btKinematicCharacterController, and link it to a Ogre::Camera*
	void createCharacterController(Ogre::Camera* camera,Ogre::Vector3 initPosition);

	//Updates the individual managers.
	bool UpdateManagers();

private:
	//private utility function.
	btBvhTriangleMeshShape* buildLevelCollisionShape(Ogre::SceneNode* node);
	
	//private timing variables
	float Time,oldTime,deltaTime;

	//Character controller variables
	btKinematicCharacterController* _Controller;
	btPairCachingGhostObject* _Ghost;

	//Facilitates Ogre::Singleton.
	GameManager(const GameManager&);
	GameManager& operator=(GameManager&);
};

#endif