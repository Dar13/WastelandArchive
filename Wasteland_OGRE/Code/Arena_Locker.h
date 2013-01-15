#include "StdAfx.h"

#ifndef _ARENA_LOCKER_H_
#define _ARENA_LOCKER_H_

#include "State.h"

#include "RecastInterface.h"
#include "DetourInterface.h"
#include "AIManager.h"

//I only want the pause menu
#define _PAUSE_ONLY_
#include "menu\Menu.h"
#undef _PAUSE_ONLY_

//#include "CrowdManager.h"

/*! \brief Application state that is the Locker Room scene in Wasteland.

Is not a truly interactive scene, but shows an important plot point.
*/

class ArenaLocker : public State
{
public:
	//! Constructor, initializes all class members.
	ArenaLocker();

	//! State-specific setup.
	void Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);
	//! Runs the state code.
	int Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);
	//! Cleans up the state, releases all memory/assets allocated to it.
	void Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

private:
	void _handleScript();

	void _handleCamera();

	void _handleSoundEvents(std::vector<SoundEvent>& events, SoundManager* Sound);

	void _loadPhysicsEntities(std::string fileName);
	void _loadSounds(std::string fileName, SoundManager* Sound);

	void _navigationMeshSetup(Ogre::Entity* levelEntity);

	//Pause menu variable
	std::unique_ptr<PauseMenu> _pauseMenu;

	//tells the state to shutdown or not.
	bool _stateShutdown;

	std::vector<OgreBulletPair> _pairs;

	//Ogre camera
	Ogre::Camera* _camera;

	Ogre::Viewport* _view;

	Ogre::SceneManager* _scene;
	Ogre::SceneNode* _rootNode;


	//TEST VARIABLE
	Ogre::SceneNode* _sphere;
	//TEST VARIABLE

	std::unique_ptr<PhysicsManager> _physics;

	std::unique_ptr<CrowdManager> _crowd;

	std::unique_ptr<DetourInterface> _detour;
	std::unique_ptr<RecastInterface> _recast;

	std::vector<sSound> _sounds;

	std::unique_ptr<AIManager> _AIManager;

	//Very state-specific variables
	//directly related to the movement of the camera
	Ogre::Vector3 _cameraPositionTarget;
	Ogre::Vector3 _oldCameraPositionTarget;
	Ogre::Vector3 _cameraLookTarget;
	float _cameraMovementTime;

};

#endif