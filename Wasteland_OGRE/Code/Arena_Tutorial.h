#include "StdAfx.h"

#ifndef _ARENA_TUTORIAL_H_
#define _ARENA_TUTORIAL_H_

#include "State.h"
#include "OISManager.h"
#include "CharacterController.h"
#include "EWS.h"

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
	void Setup(OISManager* input);
	//! Runs the state.
	int Run(OISManager* input);
	//! Shutsdown and cleans up the application state.
	void Shutdown(OISManager* input);

private:
	bool _stateShutdown;

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

	//Entity management
	std::vector<Ogre::Entity*> _entities;

	CharacterController* _controller;
	EWSManager* _ews;
};

#endif