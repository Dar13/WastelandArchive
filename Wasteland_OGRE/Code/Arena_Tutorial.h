#include "StdAfx.h"

#ifndef _ARENA_TUTORIAL_H_
#define _ARENA_TUTORIAL_H_

#include "State.h"
#include "InputManager.h"
#include "CharacterController.h"
#include "EWS.h"
#include "PhysicsManager.h"
#include "LevelData.h"

#include "RecastInterface.h"
#include "DetourInterface.h"
#include "CrowdManager.h"

#include "AI\npc_character.h"
#include "AI\enemy_character.h"

#define _PAUSE_ONLY_
#include "menu\Menu.h"
#undef _PAUSE_ONLY_

// forward declaration, keeping the header file slim.
struct OgreBulletPair;

/*! \brief Application state that is the Tutorial scene in Wasteland.

Teaches player how to move, shoot, and adds some background to the story. 
Shows a more personal experience of the world that the protagonist is in.
*/

class ArenaTutorial : public State
{
public:
	//! Constructor, initializes all class variables.
	ArenaTutorial();

	//! State-specific setup.
	void Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);
	//! Runs the state.
	int Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);
	//! Shutsdown and cleans up the application state.
	void Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

private:
	//! Tracks all OgreBulletPairs
	std::vector<OgreBulletPair> _pairs;

	//! State-specific Ogre::Camera*
	Ogre::Camera* _camera;
	
	//! State-specific Ogre viewport.
	Ogre::Viewport* _view;

	//! State-specific Ogre SceneManager. Allows for greater performance tweaking.
	Ogre::SceneManager* _scene;
	//Scene node stuff
	Ogre::SceneNode* _rootNode;
	std::vector<Ogre::SceneNode*> _nodes;

	Ogre::CompositorInstance* _SSAO;
	ShadowListener* _shadowListener;
	ShadowCompositorListener* _shadowCompositorListener;

	//Entity management
	std::vector<Ogre::Entity*> _entities;

	//player variables
	std::unique_ptr<Player> _player;

	//EWS and character controller variables
	std::unique_ptr<CharacterController> _controller;
	std::unique_ptr<EWSManager> _ews;

	//Pause menu variable
	std::unique_ptr<PauseMenu> _pauseMenu;

	//Physics stuff
	std::unique_ptr<PhysicsManager> _physics;
	std::vector<btTypedConstraint*> _constraints;

	//Sound stuff
	std::vector<sSound> _sounds;
	std::vector<FMOD::Channel*> _channels;

	//Recast/Detour
	std::unique_ptr<DetourInterface> _detour;
	std::unique_ptr<RecastInterface> _recast;

	std::unique_ptr<CrowdManager> _crowd;

	std::vector<NPCCharacter*> _npcs;

	//Lights management
	void _setupLights(GraphicsManager* g, Ogre::SceneManager* scene);
	void _updateLights();
	std::vector<std::unique_ptr<LevelData::LightData>> _lights;

	//TriggerZone Management
	void _updateTriggers(OgreTransform* playerTransform,int currentTime);
	std::vector<std::unique_ptr<LevelData::TriggerZone>> _triggers;

	//Doors management
	void _setupDoors(OgreBulletPair& mainLevel,Ogre::SceneManager* scene,PhysicsManager* p,GraphicsManager* g);
	void _updateDoors();
	std::vector<std::unique_ptr<LevelData::DoorData>> _doors;
};

#endif