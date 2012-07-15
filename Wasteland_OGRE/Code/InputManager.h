#include "StdAfx.h"
#include "interfaces\interfaces.h"

#include <OIS.h>

#ifndef _OIS_MANAGER_H_
#define _OIS_MANAGER_H_



/*! \brief The OIS(Object Oriented Input System) Manager. Handles all inputManager for Wasteland.

Is entirely self-contained, has no reliance on any other Manager.
Needs an interface to find out about specific key presses.
*/

class InputManager : OIS::MouseListener, OIS::KeyListener
{
public:
	enum CONFIG_KEY_VALUES
	{
		FORWARD = 0,
		BACKWARD,
		RIGHT,
		LEFT,
		JUMP,
		SPRINT,
		USE,
		ENVWARNSYS,
		RELOAD,
		WEAPON1,
		WEAPON2,
		MAXVALUE
	};
	//! Constructor, sets up the class members.
	InputManager(unsigned long windowHandle);
	~InputManager();

	//! Sets the capture window for input during the application.
	void setCaptureWindow(int width,int height);

	bool Update(bool checkEscapeKey = true);

	//! Capture the input
	void capture();

	//! Gets whether a config key is pressed.
	bool isCFGKeyPressed(unsigned int key);

	//! Sets whether or not OIS should lock the abs location of the mouse.
	void setMouseLock(bool lock);

	//! Sets absolute location of the mouse
	void setMousePosition(int& x,int& y);

	//! Gets current state of mouse movement horizontally(x-axis).
	int getMouseMoveX();
	//! Gets current state of mouse movement vertically(y-axis).
	int getMouseMoveY();
	//! Gets current state of mouse movement depth(wheel/z-axis).
	int getMouseMoveZ();

	//! Get current state 
	bool isMBPressed(OIS::MouseButtonID id);

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

	//! Sets configuration.
	void setConfiguration(configuration_t* config);
	//! Gets configuration.
	configuration_t* getConfiguration() { return _config; }

	//! Returns true if the Escape key is pressed.
	bool escapePressed(){return _appShutdown;}
private:
	//Private members
	bool _appShutdown;

	//OIS stuff
	OIS::InputManager* _inputSystem;
	unsigned long _windowHandle;
	OIS::Mouse* _mouseObj;
	OIS::Keyboard* _keyObj;

	//configuration
	configuration_t* _config;
	std::vector<std::string> _keyValues;
	std::vector<bool> _keyDown;

	//control character keyvalues
	std::vector<std::string> _ckeyValues;

	//holds conversion from OIS keycodes to char
	std::map<int,char> _KC_map;
	char getCharFromKeyCode(unsigned int keyCode);

	//mouse movement variables
	bool _lockMouse;
	int _mouseMoveX,_mouseMoveY,_mouseMoveZ;

	//mouse click variable(s)
	bool _mouseRB,_mouseMB,_mouseLB;
};

#endif