#include "StdAfx.h"

#ifndef _OIS_MANAGER_H_
#define _OIS_MANAGER_H_

/*! \brief The OIS(Object Oriented Input System) Manager. Handles all input for Wasteland.

Is entirely self-contained, has no reliance on any other Manager.
Needs an interface to find out about specific key presses.
*/

class OISManager : public Ogre::Singleton<OISManager>,OIS::MouseListener, OIS::KeyListener
{
public:
	//! Constructor, sets up the class members.
	OISManager(unsigned long windowHandle);
	~OISManager();

	//! Sets the capture window for input during the application.
	void setCaptureWindow(int width,int height);

	//! Capture the input
	void capture();

	//! Handles all key presses.
	bool keyPressed(const OIS::KeyEvent &evt);
	//! Handles all key releases.
	bool keyReleased(const OIS::KeyEvent &evt);
	//! Handles all mouse movements.
	bool mouseMoved(const OIS::MouseEvent &evt);
	//! Handles all mouse button presses.
	bool mousePressed(const OIS::MouseEvent &evt,OIS::MouseButtonID id);
	//! Handles all mouse button releases.
	bool mouseReleased(const OIS::MouseEvent &evt,OIS::MouseButtonID id);

	//! Returns true if the Escape key is pressed.
	bool escapePressed(){return _appShutdown;}
private:
	//This way, they're not implemented.
	OISManager(const OISManager&){}
	OISManager& operator= (const OISManager&){}

	//Private members
	bool _appShutdown;
	OIS::InputManager* _inputSystem;
	unsigned long _windowHandle;
	OIS::Mouse* _mouseObj;
	OIS::Keyboard* _keyObj;
};

#endif