#include "StdAfx.h"

#include "OISManager.h"

#include <OgreStringConverter.h>

template<> OISManager* Ogre::Singleton<OISManager>::ms_Singleton=0;

bool isControlCharacter(OIS::KeyCode keyCode);

OISManager::OISManager(unsigned long windowHandle)
{
	//init some of the member variables
	_appShutdown=false;

	//Just keeping this window handle around. In case I have to reinitialize.
	_windowHandle = windowHandle;
	
	//Putting the window handle into OIS-terms.
	OIS::ParamList pl;
	pl.insert(OIS::ParamList::value_type("WINDOW",Ogre::StringConverter::toString(windowHandle)));

	//Create the OIS input objects.
	_inputSystem = OIS::InputManager::createInputSystem(pl);
	_mouseObj = static_cast<OIS::Mouse*>(_inputSystem->createInputObject(OIS::OISMouse,true));
	_keyObj = static_cast<OIS::Keyboard*>(_inputSystem->createInputObject(OIS::OISKeyboard,true));

	_mouseObj->setEventCallback(this); //this class is a mouseListener
	_keyObj->setEventCallback(this); //this class is a keyboardListener

	//set up the keycode stuff
	_KC_map[OIS::KC_A] = 'a';
	_KC_map[OIS::KC_B] = 'b';
	_KC_map[OIS::KC_C] = 'c';
	_KC_map[OIS::KC_D] = 'd';
	_KC_map[OIS::KC_E] = 'e';
	_KC_map[OIS::KC_F] = 'f';
	_KC_map[OIS::KC_G] = 'g';
	_KC_map[OIS::KC_H] = 'h';
	_KC_map[OIS::KC_I] = 'i';
	_KC_map[OIS::KC_J] = 'j';
	_KC_map[OIS::KC_K] = 'k';
	_KC_map[OIS::KC_L] = 'l';
	_KC_map[OIS::KC_M] = 'm';
	_KC_map[OIS::KC_N] = 'n';
	_KC_map[OIS::KC_O] = 'o';
	_KC_map[OIS::KC_P] = 'p';
	_KC_map[OIS::KC_Q] = 'q';
	_KC_map[OIS::KC_R] = 'r';
	_KC_map[OIS::KC_S] = 's';
	_KC_map[OIS::KC_T] = 't';
	_KC_map[OIS::KC_U] = 'u';
	_KC_map[OIS::KC_V] = 'v';
	_KC_map[OIS::KC_W] = 'w';
	_KC_map[OIS::KC_X] = 'x';
	_KC_map[OIS::KC_Y] = 'y';
	_KC_map[OIS::KC_Z] = 'z';
}

//Cleaning up.
OISManager::~OISManager()
{
	if(_mouseObj)
		_inputSystem->destroyInputObject(_mouseObj);

	if(_keyObj)
		_inputSystem->destroyInputObject(_keyObj);

	OIS::InputManager::destroyInputSystem(_inputSystem);
}

void OISManager::setCaptureWindow(int width,int height)
{
	const OIS::MouseState &ms = _mouseObj->getMouseState();
	ms.width=width;
	ms.height=height;
	OIS::MouseState &mutms = const_cast<OIS::MouseState &>(_mouseObj->getMouseState());
	mutms.X.abs = width/2;
	mutms.Y.abs = height/2;
}

void OISManager::capture()
{
	_mouseObj->capture();
	_keyObj->capture();
	
	if(_lockMouse)
	{
		OIS::MouseState &mutMS = const_cast<OIS::MouseState &>(_mouseObj->getMouseState());
		mutMS.X.abs = mutMS.width/2;
		mutMS.Y.abs = mutMS.height/2;
		//mutMS.Z.abs = 0; doesn't work
	}

	_mouseMoveX = _mouseObj->getMouseState().X.rel;
	_mouseMoveY = _mouseObj->getMouseState().Y.rel;
	//_mouseMoveZ = _mouseObj->getMouseState().Z.abs; doesn't work
}

bool OISManager::isCFGKeyPressed(unsigned int key)
{
	return _keyDown[key];
}

void OISManager::setMouseLock(bool lock)
{
	_lockMouse = lock;
}

int OISManager::getMMX()
{
	return _mouseMoveX;
}

int OISManager::getMMY()
{
	return _mouseMoveY;
}

int OISManager::getMMZ()
{
	//return _mouseMoveZ;
	return 0; // doesn't work.
}

bool OISManager::isMBPressed(OIS::MouseButtonID id)
{
	switch(id)
	{
	case OIS::MB_Right:
		return _mouseRB;
		break;
	case OIS::MB_Middle:
		return _mouseMB;
		break;
	case OIS::MB_Left:
		return _mouseLB;
		break;
	default:
		return false;
		break;
	}
}

bool OISManager::keyPressed(const OIS::KeyEvent &evt)
{
	//Checking for escape-key press
	if(evt.key==OIS::KC_ESCAPE)
	{
		_appShutdown=true;
	}

	//Will eventually inject input into CEGUI/Bullet/etc.
	//injection into CEGUI...
	/*
	CEGUI::System::getSingleton().injectKeyDown(evt.key);
	CEGUI::System::getSingleton().injectChar(evt.text);
	*/

	char character = evt.text;
	std::string checkString;
	checkString = character;

	//check for specific control characters, if applicable for the KeyValues.
	//unfortunately can't figure out how to make it dynamic enough to handle all characters on keyboard
	switch(evt.key)
	{
	case OIS::KC_SPACE:
		checkString = "space";
		break;
	case OIS::KC_RCONTROL:
		checkString = "rcontrol";
		break;
	case OIS::KC_LCONTROL:
		checkString = "lcontrol";
		break;
	case OIS::KC_UP:
		checkString = "uparrow";
		break;
	case OIS::KC_DOWN:
		checkString = "downarrow";
		break;
	case OIS::KC_LEFT:
		checkString = "leftarrow";
		break;
	case OIS::KC_RIGHT:
		checkString = "rightarrow";
		break;
	case OIS::KC_RSHIFT:
		checkString = "rshift";
		break;
	case OIS::KC_LSHIFT:
		checkString = "lshift";
		break;
	case OIS::KC_RMENU:
		checkString = "ralt";
		break;
	case OIS::KC_LMENU:
		checkString = "lalt";
		break;
	};

	for(unsigned int i = FORWARD; i<MAXVALUE; ++i)
	{
		if(_keyValues[i] == checkString)
		{
			_keyDown[i] = true;	
		}
	}

	return true;
}

bool OISManager::keyReleased(const OIS::KeyEvent &evt)
{
	//Will eventually inject input into CEGUI/Bullet/etc.
	//injection into CEGUI...
	//CEGUI::System::getSingleton().injectKeyUp(evt.key);
	
	char character = getCharFromKeyCode(evt.key);
	std::string checkString;
	checkString = character;

	//check for specific control characters, if applicable for the KeyValues.
	//unfortunately can't figure out how to make it dynamic enough to handle all characters on keyboard
	switch(evt.key)
	{
	case OIS::KC_SPACE:
		checkString = "space";
		break;
	case OIS::KC_RCONTROL:
		checkString = "rcontrol";
		break;
	case OIS::KC_LCONTROL:
		checkString = "lcontrol";
		break;
	case OIS::KC_UP:
		checkString = "uparrow";
		break;
	case OIS::KC_DOWN:
		checkString = "downarrow";
		break;
	case OIS::KC_LEFT:
		checkString = "leftarrow";
		break;
	case OIS::KC_RIGHT:
		checkString = "rightarrow";
		break;
	case OIS::KC_RSHIFT:
		checkString = "rshift";
		break;
	case OIS::KC_LSHIFT:
		checkString = "lshift";
		break;
	case OIS::KC_RMENU:
		checkString = "ralt";
		break;
	case OIS::KC_LMENU:
		checkString = "lalt";
		break;
	};

	for(unsigned int i = FORWARD; i<MAXVALUE; ++i)
	{
		if(_keyValues[i] == checkString)
		{
			_keyDown[i] = false;
		}
	}

	return true;
}

bool OISManager::mouseMoved(const OIS::MouseEvent &evt)
{

	return true;
}

bool OISManager::mousePressed(const OIS::MouseEvent &evt,OIS::MouseButtonID id)
{
	switch(id)
	{
	case OIS::MB_Right:
		_mouseRB = true;
		break;
	case OIS::MB_Left:
		_mouseLB = true;
		break;
	case OIS::MB_Middle:
		_mouseMB = true;
		break;
	default:
		//nothing.
		break;
	};
	return true;
}

bool OISManager::mouseReleased(const OIS::MouseEvent &evt,OIS::MouseButtonID id)
{
	switch(id)
	{
	case OIS::MB_Right:
		_mouseRB = false;
		break;
	case OIS::MB_Left:
		_mouseLB = false;
		break;
	case OIS::MB_Middle:
		_mouseMB = false;
		break;
	default:
		//nothing.
		break;
	};
	return true;
}

void OISManager::setConfiguration(configuration_t* config)
{
	_config = config;
	_keyValues.clear();
	//follows the convention set in CONFIG_KEY_VALUES.
	//This can be added to, but CONFIG_KEY_VALUES must also be changed as well.
	_config->movement().forward()[0] = tolower(_config->movement().forward()[0]);
	_config->movement().backward()[0] = tolower(_config->movement().backward()[0]);
	_config->movement().right()[0] = tolower(_config->movement().right()[0]);
	_config->movement().left()[0] = tolower(_config->movement().left()[0]);
	_config->movement().jump()[0] = tolower(_config->movement().jump()[0]);
	_config->movement().sprint()[0] = tolower(_config->movement().sprint()[0]);
	_config->action().use()[0] = tolower(_config->action().use()[0]);
	_config->action().envwarnsys()[0] = tolower(_config->action().envwarnsys()[0]);
	_config->action().reload()[0] = tolower(_config->action().reload()[0]);
	_config->action().weapon1()[0] = tolower(_config->action().weapon1()[0]);
	_config->action().weapon2()[0] = tolower(_config->action().weapon2()[0]);
	_keyValues.push_back(_config->movement().forward()); //forward first
	_keyValues.push_back(_config->movement().backward());
	_keyValues.push_back(_config->movement().right());
	_keyValues.push_back(_config->movement().left());
	_keyValues.push_back(_config->movement().jump());
	_keyValues.push_back(_config->movement().sprint());
	_keyValues.push_back(_config->action().use());
	_keyValues.push_back(_config->action().envwarnsys());
	_keyValues.push_back(_config->action().reload()); 
	_keyValues.push_back(_config->action().weapon1());
	_keyValues.push_back(_config->action().weapon2());//weapon2 last.

	//just setting up this vector
	_keyDown.clear();
	for(unsigned int i = 0; i<MAXVALUE; ++i)
	{
		_keyDown.push_back(false);
	}
}

char OISManager::getCharFromKeyCode(unsigned int keyCode)
{
	return _KC_map[keyCode];
}

bool isControlCharacter(OIS::KeyCode keyCode)
{
	//gotta be a better way...
	return false;
}