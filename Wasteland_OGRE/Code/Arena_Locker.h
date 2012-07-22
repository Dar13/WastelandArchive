#include "StdAfx.h"

#ifndef _ARENA_LOCKER_H_
#define _ARENA_LOCKER_H_

#include "State.h"

/*! \brief **NOT FULLY IMPLEMENTED** Application state that is the Locker Room scene in Wasteland.

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
	//tells the state to shutdown or not.
	bool _stateShutdown;

	std::vector<OgreBulletPair> _pairs;

	//Ogre camera
	Ogre::Camera* _camera;
	Ogre::SimpleSpline _cameraTrack;

	Ogre::Viewport* _view;

	Ogre::SceneManager* _scene;
	Ogre::SceneNode* _rootNode;

	std::unique_ptr<PhysicsManager> _physics;

	std::vector<sSound> _sounds;
	std::vector<FMOD::Channel*> _channels;

};

#endif