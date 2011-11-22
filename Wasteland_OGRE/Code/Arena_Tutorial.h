#ifndef _ARENA_TUTORIAL_H_
#define _ARENA_TUTORIAL_H_

#include "State.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreMesh.h>

#include <vector>

class ArenaTutorial : public State
{
public:
	//Initial setup
	ArenaTutorial();
	//No need for special destructor, all is cleaned up in Shutdown().

	//State actions
	void Setup();
	int Run();
	void Shutdown();

private:
	bool _stateShutdown;

	//Camera
	Ogre::Camera* _camera;
	
	//Viewport
	Ogre::Viewport* _view;

	Ogre::SceneManager* _scene;
	//Scene node stuff
	Ogre::SceneNode* _rootNode;
	std::vector<Ogre::SceneNode*> _nodes;

	//Entity management
	std::vector<Ogre::Entity*> _entities;

};

#endif