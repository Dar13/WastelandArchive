#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <OgreSingleton.h>
#include "OgreManager.h"
#include "OISManager.h"
#include "BulletManager.h"

/*! \brief Allows convenient returns of both Bullet rigid bodies and Ogre SceneNodes.
*/
struct OgreBulletPair
{
	Ogre::SceneNode* ogreNode;
	btRigidBody* btBody;
};

/*! \brief The Manager class of all the managers. The final abstraction layer.

This class is where all the different Managers
(Ogre,Bullet,OIS,Sound,etc.)will interact in order to 
perform certain tasks(example: building a btBvhTriangleMeshShape
from an Ogre::Mesh. This is the abstraction upon which the
majority of the game code will be built, avoiding reliance
on the individual managers.
*/

class GameManager : public Ogre::Singleton<GameManager>
{
public:
	//! Constructor
	GameManager();
	~GameManager(); 
	
	//! Creates a level, complete with a Bullet Physics btBvhTriangleMesh rigid body.
	/*!
		\param scene The current Ogre SceneManager, required for the creation of the SceneNode.
		\param options All the information needed to create the Ogre SceneNode and/or the Bullet rigid body.

		Options:
			- Ogre
				-# filename = filename for the mesh (MANDATORY) 
				-# resgroup = name of resource group (MANDATORY) 
				-# name = name for the entity and scenenode (MANDATORY)
	*/
	Ogre::SceneNode* createLevel(Ogre::SceneManager* scene,std::map<std::string,std::string> &options);

	/*!Creates an object, complete with Bullet Physics rigid body.

	\param scene Ogre SceneManager, required for SceneNode creation.
	\param options Options for the Ogre SceneNode and/or Bullet rigid body.
	\param mass Mass for the Bullet rigid body.
	\param init Initial transformation(position/rotation) for the Bullet rigid body/Ogre SceneNode.
	\param shape Shape of the Bullet rigid body.
	
	Options:
		- Ogre
			-# filename = filename for mesh(OGRE)
			-# resgroup = name of resource group(OGRE)
			-# name = name of ogre entity/scenenode(OGRE)

	\returns OgreBulletPair containing SceneNode* and btRigidBody*.
	*/
	OgreBulletPair createObject(Ogre::SceneManager* scene,
								std::map<std::string,std::string> &options,
								btScalar &mass, 
								btTransform &init,
								btCollisionShape* shape);
	/*! Creates object, generates Ogre SceneNode and Bullet rigid body.

		\param node Premade SceneNode.
		\param shape Premade Bullet collision shape.
		\param mass Mass for the rigid body.
		\param init Initial position/rotation for the rigid body/scene node.

		\returns OgreBulletPair containing the SceneNode* and btRigidBody*.
	*/
	OgreBulletPair createObject(Ogre::SceneNode* node,
								btCollisionShape* shape,
								btScalar &mass,
								btTransform &init);

	//! Create a btKinematicCharacterController, and link it to a Ogre::Camera*
	/*!
		\param camera The camera you want the character controller to control.
		\param initPosition The initial position the character controller will start at.
	*/
	void createCharacterController(Ogre::Camera* camera,Ogre::Vector3 initPosition);

	//!Updates the individual managers.
	bool UpdateManagers();

private:
	//private utility function.
	//! Utilizes a special OgreManager function to create a triangle mesh collision shape.
	//! \sa OgreManager::getMeshInformation()
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