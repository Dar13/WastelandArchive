#include "StdAfx.h"

#include "OISManager.h"

#include <OgreStringConverter.h>

template<> OISManager* Ogre::Singleton<OISManager>::ms_Singleton=0;

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
}

void OISManager::capture()
{
	_mouseObj->capture();
	_keyObj->capture();
}

bool OISManager::keyPressed(const OIS::KeyEvent &evt)
{
	//Checking for escape-key press
	if(evt.key==OIS::KC_ESCAPE)
	{
		_appShutdown=true;
	}

	//Will eventually inject input into CEGUI/Bullet/etc.

	return true;
}

bool OISManager::keyReleased(const OIS::KeyEvent &evt)
{
	//Will eventually inject input into CEGUI/Bullet/etc.

	return true;
}

bool OISManager::mouseMoved(const OIS::MouseEvent &evt)
{

	return true;
}

bool OISManager::mousePressed(const OIS::MouseEvent &evt,OIS::MouseButtonID id)
{
	return true;
}

bool OISManager::mouseReleased(const OIS::MouseEvent &evt,OIS::MouseButtonID id)
{
	return true;
}