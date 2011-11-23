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

private:
	btBvhTriangleMeshShape* buildLevelCollisionShape(Ogre::SceneNode* node);
	GameManager(const GameManager&);
	GameManager& operator=(GameManager&);
};

#endif