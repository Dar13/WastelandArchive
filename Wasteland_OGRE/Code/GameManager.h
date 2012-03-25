#include "StdAfx.h"
#include "interfaces\interfaces.h"

#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "OgreManager.h"
#include "OISManager.h"
#include "BulletManager.h"
#include "XMLReader.h"
#include "EWS.h"

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
Will handle certain gameplay-specific managers(EWS and player) from
beginning to end of the game(from 'new' to 'delete'). Keeps from crowding
Main.cpp
*/

class GameManager : public Ogre::Singleton<GameManager>
{
public:
	//! Constructor
	GameManager();
	~GameManager(); 

	/*!Creates an object, complete with Bullet Physics rigid body.

	\param scene Ogre SceneManager, required for SceneNode creation.
	\param objectInfo Pointer to class that holds all information for object.

	\returns OgreBulletPair containing SceneNode* and btRigidBody*.
	*/
	OgreBulletPair createObject(Ogre::SceneManager* scene,
								std::string& file);
	OgreBulletPair createObject(Ogre::SceneManager* scene,
								object_t* objectInfo);
	/*! Creates object, generates Ogre SceneNode and Bullet rigid body.

		\param node Premade SceneNode.
		\param objectInfo Pointer to class that holds all object information.

		\returns OgreBulletPair containing the SceneNode* and btRigidBody*.
	*/
	OgreBulletPair createObject(Ogre::SceneNode* node,
								object_t* objectInfo);

	/*! Tells class to use the debug drawer, to verify collisions are happening correctly.
		
		\param scene Current SceneManager for Ogre, needed to draw the stuff.
	*/
	void useDebugDrawer(Ogre::SceneManager* scene);

	/*! Loads all weapons in list xml file that is passed in.

	*/
	void loadWeapons(std::string file);

	/*! Updates the individual managers.

	\returns true if all managers updated properly.
	*/
	bool UpdateManagers();

	/*! Load configuration values

	*/
	void setConfiguration(configuration_t* configuration);

	/*! Get the current configuration values.

	\returns configuration_t pointer to variable.
	*/
	configuration_t* getConfiguration();

	float getCurrentElapsedTime()
	{
		return deltaTime;
	}

private:
	//private utility function.
	//! Utilizes a special OgreManager function to create a triangle mesh collision shape.
	//! \sa OgreManager::getMeshInformation()
	btBvhTriangleMeshShape* buildTriangleCollisionShape(Ogre::SceneNode* node);
	
	//private timing variables
	float Time,oldTime,deltaTime;

	//Utility functions, converts Ogre::Vector3 to btVector3
	static btVector3 convertOgreVector3(const Ogre::Vector3 &v);
	static Ogre::Vector3 convertBulletVector3(const btVector3 &v);
	static btQuaternion convertOgreQuat(const Ogre::Quaternion &q);
	static Ogre::Quaternion convertBulletQuat(const btQuaternion &q);
	static btMatrix3x3 convertOgreMatrix3(const Ogre::Matrix3 &m);
	static Ogre::Matrix3 convertBulletMatrix3(const btMatrix3x3 &m);

	//Holds current configuration values.
	configuration_t* _config;

	//Ogre stuff
	Ogre::SceneManager* _currentScene;

	//Facilitates Ogre::Singleton.
	GameManager(const GameManager&);
	GameManager& operator=(GameManager&);
};

#endif