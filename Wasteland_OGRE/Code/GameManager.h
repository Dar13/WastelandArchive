#include "StdAfx.h"
#include "interfaces\interfaces.h"

#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "GraphicsManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"
#include "Utility.h"

#ifndef _GM_NO_EXTRAS_
#include "GunData.h"
#endif


/*! \brief The Manager namespace of all the managers. The final abstraction layer.

This namespace is where all the different Managers
(Ogre,Bullet,OIS,Sound,etc.)will interact in order to 
perform certain tasks(example: building a btBvhTriangleMeshShape
from an Ogre::Mesh. This is the abstraction upon which the
majority of the game code will be built, avoiding reliance
on the individual managers. Keeps from crowding
Main.cpp
*/

namespace GameManager
{
	/*!Creates an object, complete with Bullet Physics rigid body.

	\param scene Ogre SceneManager, required for SceneNode creation.
	\param objectInfo Pointer to class that holds all information for object.

	\returns OgreBulletPair containing SceneNode* and btRigidBody*.
	*/
	OgreBulletPair createObject(Ogre::SceneManager* scene,
								std::string& file,
								PhysicsManager* phyManager,
								GraphicsManager* graphicsManager);
	OgreBulletPair createObject(Ogre::SceneManager* scene,
								object_t* objectInfo,
								PhysicsManager* phyManager,
								GraphicsManager* graphicsManager);
	Ogre::SceneNode* createObject(Ogre::SceneManager* scene,
								  std::string& file,
								  GraphicsManager* graphics);
	/*! Creates object, generates Ogre SceneNode and Bullet rigid body.

		\param node Premade SceneNode.
		\param objectInfo Pointer to class that holds all object information.

		\returns OgreBulletPair containing the SceneNode* and btRigidBody*.
	*/
	OgreBulletPair createObject(Ogre::SceneNode* node,
								object_t* objectInfo,
								PhysicsManager* phyManager,
								GraphicsManager* graphicsManager);

	Ogre::SceneNode* createCharacterObject(Ogre::SceneManager* scene,
										   characterobject_t* obj,
										   GraphicsManager* graphics = nullptr);

	/*! Creates an equippable object, compatible with the Player class.
		
		\param scene The current SceneManager.
		\param file The file where the equippable object is defined in the correct XML format.
		\param graphicsManager A pointer to the GraphicsManager
		\param soundManager A pointer to the SoundManager
		\param isWeapon Tells whether to perform weapon-specific preparation on the object

		\returns EquippableObject Contains all the data needed for the object.
		
	*/
	EquippableObject createEquippable(Ogre::SceneManager* scene,
									  const std::string& file,
									  GraphicsManager* graphicsManager,
									  SoundManager* soundManager,
									  bool isWeapon = true);

	/*! Tells class to use the debug drawer, to verify collisions are happening correctly.
		
		\param scene Current SceneManager for Ogre, needed to draw the stuff.
	*/
	void setDebugDrawer(Ogre::SceneManager* scene,PhysicsManager* phyManager);

	void RenderScene(GraphicsManager* Graphics,Ogre::Viewport* view);
	
	/*! Updates the individual managers.

	\returns true if all managers updated properly.
	*/
	bool UpdateManagers(GraphicsManager* graphicsManager,PhysicsManager* phyManager,float deltaTime);
	
	//! Utilizes a special GraphicsManager function to create a triangle mesh collision shape.
	//! \sa GraphicsManager::getMeshInformation()
	btBvhTriangleMeshShape* buildTriangleCollisionShape(Ogre::SceneNode* node,GraphicsManager* Graphics);
};

#endif