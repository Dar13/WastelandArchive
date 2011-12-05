#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <OgreSingleton.h>
#include "OgreManager.h"
#include "OISManager.h"
#include "BulletManager.h"
#include "XMLReader.h"

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
		<ul>
			<li>"name" Name for both the Entity and the SceneNode.</li>
			<li>"filename" FileName for the Entity to load from(if needed for the type of Entity/SceneNode).</li>
			<li>"type" The type of entity to be created.</li>
			<li>"resgroup" Resource Group that holds the FileName.</li>
		</ul>
	*/
	Ogre::SceneNode* createLevel(Ogre::SceneManager* scene,std::map<std::string,std::string> &options);

	/*!Creates an object, complete with Bullet Physics rigid body.

	\param scene Ogre SceneManager, required for SceneNode creation.
	\param options Options for the Ogre SceneNode and/or Bullet rigid body.
	<ul>
		<li>"name" Name for both the Entity and the SceneNode.</li>
		<li>"filename" FileName for the Entity to load from(if needed for the type of Entity/SceneNode).</li>
		<li>"type" The type of entity to be created.</li>
		<li>"resgroup" Resource Group that holds the FileName.</li>
	</ul>
	\param mass Mass for the Bullet rigid body.
	\param init Initial transformation(position/rotation) for the Bullet rigid body/Ogre SceneNode.
	\param shape Shape of the Bullet rigid body.

	\returns OgreBulletPair containing SceneNode* and btRigidBody*.
	*/
	OgreBulletPair createObject(Ogre::SceneManager* scene,
								object_t* objectInfo);
	/*! Creates object, generates Ogre SceneNode and Bullet rigid body.

		\param node Premade SceneNode.
		\param shape Premade Bullet collision shape.
		\param mass Mass for the rigid body.
		\param init Initial position/rotation for the rigid body/scene node.

		\returns OgreBulletPair containing the SceneNode* and btRigidBody*.
	*/
	OgreBulletPair createObject(Ogre::SceneNode* node,
								object_t* objectInfo);

	//! Create a btKinematicCharacterController, and link it to a Ogre::Camera*
	/*!
		\param camera The camera you want the character controller to control.
		\param initPosition The initial position the character controller will start at.
	*/
	void createCharacterController(Ogre::Camera* camera,Ogre::Vector3 initPosition);

	/*! Updates the character controller created in createCharacterController.

		If Ogre::Camera* is passed in, then this function will also update the camera's position and rotation.

		\param camera If NULL(0) is passed in, then the class with refer to the stored camera.
		If you didn't set a camera in the createCharacterController() then no camera will be updated at all.
	*/
	void updateCharacterController(Ogre::Camera* camera = NULL);

	/*! Updates the individual managers.

	\returns true if all managers updated properly.
	*/
	bool UpdateManagers();

private:
	//private utility function.
	//! Utilizes a special OgreManager function to create a triangle mesh collision shape.
	//! \sa OgreManager::getMeshInformation()
	btBvhTriangleMeshShape* buildLevelCollisionShape(Ogre::SceneNode* node);
	
	//private timing variables
	float Time,oldTime,deltaTime;

	//Character controller variables
	btKinematicCharacterController* _charController;
	btPairCachingGhostObject* _charGhost;
	Ogre::Camera* _charCamera;

	//Utility functions, converts Ogre::Vector3 to btVector3
	btVector3 convertOgreVector3(const Ogre::Vector3 &v);
	Ogre::Vector3 convertBulletVector3(const btVector3 &v);

	//Facilitates Ogre::Singleton.
	GameManager(const GameManager&);
	GameManager& operator=(GameManager&);
};

#endif