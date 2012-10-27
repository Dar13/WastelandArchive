#include "StdAfx.h"

#include "InputManager.h"

#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>

#include <OgreStringConverter.h>

bool isControlCharacter(OIS::KeyCode keyCode);

InputManager::InputManager(unsigned long windowHandle)
	: _mouseRB(false),
	  _mouseMB(false),
	  _mouseLB(false),
	  _config(nullptr),
	  _lockMouse(false),
	  _mouseMoveX(0),
	  _mouseMoveY(0),
	  _mouseMoveZ(0),
	  _appShutdown(false),
	  _windowHandle(windowHandle) /*Keeping this window handle around in case I have to reinitialize.*/
{
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
	_KC_map["a"] = OIS::KC_A;
	_KC_map["b"] = OIS::KC_B;
	_KC_map["c"] = OIS::KC_C;
	_KC_map["d"] = OIS::KC_D;
	_KC_map["e"] = OIS::KC_E;
	_KC_map["f"] = OIS::KC_F;
	_KC_map["g"] = OIS::KC_G;
	_KC_map["h"] = OIS::KC_H;
	_KC_map["i"] = OIS::KC_I;
	_KC_map["j"] = OIS::KC_J;
	_KC_map["k"] = OIS::KC_K;
	_KC_map["l"] = OIS::KC_L;
	_KC_map["m"] = OIS::KC_M;
	_KC_map["n"] = OIS::KC_N;
	_KC_map["o"] = OIS::KC_O;
	_KC_map["p"] = OIS::KC_P;
	_KC_map["q"] = OIS::KC_Q;
	_KC_map["r"] = OIS::KC_R;
	_KC_map["s"] = OIS::KC_S;
	_KC_map["t"] = OIS::KC_T;
	_KC_map["u"] = OIS::KC_U;
	_KC_map["v"] = OIS::KC_V;
	_KC_map["w"] = OIS::KC_W;
	_KC_map["x"] = OIS::KC_X;
	_KC_map["y"] = OIS::KC_Y;
	_KC_map["z"] = OIS::KC_Z;
	_KC_map["lshift"] = OIS::KC_LSHIFT;
	_KC_map["right shift"] = OIS::KC_RSHIFT;
	_KC_map["lalt"] = OIS::KC_LMENU;
	_KC_map["right alt"] = OIS::KC_RMENU;
	_KC_map["lctrl"] = OIS::KC_LCONTROL;
	_KC_map["right ctrl"] = OIS::KC_RCONTROL;
	_KC_map["enter"] = OIS::KC_RETURN;
	_KC_map["tab"] = OIS::KC_TAB;
	_KC_map["space"] = OIS::KC_SPACE;
	for(int i = OIS::KC_1; i < OIS::KC_0+1; ++i)
	{
		if(i != OIS::KC_0)
		{
			_KC_map[Ogre::StringConverter::toString(i-1)] = i;
		}
		else
		{
			_KC_map["0"] = OIS::KC_0;
		}
	}
}

//Cleaning up.
InputManager::~InputManager()
{
	if(_mouseObj)
		_inputSystem->destroyInputObject(_mouseObj);

	if(_keyObj)
		_inputSystem->destroyInputObject(_keyObj);

	OIS::InputManager::destroyInputSystem(_inputSystem);

	delete _config;
}

void InputManager::setCaptureWindow(int width,int height)
{
	const OIS::MouseState &ms = _mouseObj->getMouseState();
	ms.width=width;
	ms.height=height;
	OIS::MouseState &mutms = const_cast<OIS::MouseState &>(_mouseObj->getMouseState());
	mutms.X.abs = width/2;
	mutms.Y.abs = height/2;
}

void InputManager::setMousePosition(int& x,int& y)
{
	OIS::MouseState &m = const_cast<OIS::MouseState&>(_mouseObj->getMouseState());
	m.X.abs = x;
	m.Y.abs = y;
}

bool InputManager::Update(bool checkEscapeKey)
{
	//get input
	try
	{
		capture();
	}
	catch(OIS::Exception& o)
	{
		OutputDebugStringA(o.what());
		OutputDebugStringA(o.eFile);
		char buf[100];
		itoa(o.eType,buf,10);
		OutputDebugStringA(buf);
	}

	//check if exit char is pressed.
	if(checkEscapeKey)
		return ((_appShutdown) ? false : true);

	//return false by default.
	//return false;
	//to keep in line with other managers' update methods, return true by default.
	return true;
}

void InputManager::capture()
{
	_mouseObj->capture();
	_keyObj->capture();
	
	if(_lockMouse)
	{
		//variable name is mutMS because I can change the variables(aka mutable MouseState).
		OIS::MouseState &mutMS = const_cast<OIS::MouseState &>(_mouseObj->getMouseState());
		mutMS.X.abs = mutMS.width/2;
		mutMS.Y.abs = mutMS.height/2;
		//mutMS.Z.abs = 0; doesn't work
	}

	_mouseMoveX = _mouseObj->getMouseState().X.rel;
	_mouseMoveY = _mouseObj->getMouseState().Y.rel;
	//_mouseMoveZ = _mouseObj->getMouseState().Z.abs; doesn't work
}

bool InputManager::isCFGKeyPressed(unsigned int key)
{
	return _keyDown[key];
}

void InputManager::setMouseLock(bool lock)
{
	_lockMouse = lock;
}

int InputManager::getMouseMoveX()
{
	return _mouseMoveX;
}

int InputManager::getMouseMoveY()
{
	return _mouseMoveY;
}

int InputManager::getMouseMoveZ()
{
	//return _mouseMoveZ;
	return 0; // doesn't work.
}

bool InputManager::isMBPressed(OIS::MouseButtonID id)
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

bool InputManager::keyPressed(const OIS::KeyEvent &evt)
{
	//Checking for escape-key press
	if(evt.key==OIS::KC_ESCAPE)
	{
		_appShutdown=true;
	}

	//injection into CEGUI...
	CEGUI::System::getSingleton().injectKeyDown(evt.key);
	CEGUI::System::getSingleton().injectChar(evt.text);

	for(unsigned int i = FORWARD; i<MAXVALUE; ++i)
	{
		if(evt.key == _nKeyValues[i])
		{
			_keyDown[i] = true;
		}
	}

	return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &evt)
{
	_appShutdown = (evt.key == OIS::KC_ESCAPE) ? false : _appShutdown;

	//Will eventually inject input into CEGUI/Bullet/etc.
	//injection into CEGUI...
	CEGUI::System::getSingleton().injectKeyUp(evt.key);

	for(unsigned int i = FORWARD; i<MAXVALUE; ++i)
	{
		if(evt.key == _nKeyValues[i])
		{
			_keyDown[i] = false;
		}

	}

	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingletonPtr()->injectMouseMove(static_cast<float>(evt.state.X.rel),static_cast<float>(evt.state.Y.rel));
	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &evt,OIS::MouseButtonID id)
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

	//injection into CEGUI
	CEGUI::System::getSingletonPtr()->injectMouseButtonDown(static_cast<CEGUI::MouseButton>(id));

	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &evt,OIS::MouseButtonID id)
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

	//CEGUI injection
	CEGUI::System::getSingletonPtr()->injectMouseButtonUp(static_cast<CEGUI::MouseButton>(id));

	return true;
}

void InputManager::setConfiguration(configuration_t* config)
{
	//delete the previously held configuration
	delete _config;

	_config = config;
	_nKeyValues.clear();

	//convert the values to KC_values
	std::vector<std::string> values;
	configuration_t::movement_type* movement = &_config->movement();
	configuration_t::action_type* action = &_config->action();
	values.push_back(movement->forward());
	values.push_back(movement->backward());
	values.push_back(movement->right());
	values.push_back(movement->left());
	values.push_back(movement->jump());
	values.push_back(movement->sprint());
	values.push_back(action->use());
	values.push_back(action->envwarnsys());
	values.push_back(action->reload());
	values.push_back(action->weapon1());
	values.push_back(action->weapon2());
	
	for(int i = 0; i < MAXVALUE; ++i)
	{
		std::transform(values[i].begin(),values[i].end(),values[i].begin(),tolower);
		if(_KC_map.find(values[i]) != _KC_map.end())
		{
			_nKeyValues.push_back(_KC_map.at(values[i]));
		}
	}

	//just setting up this vector
	_keyDown.clear();
	for(unsigned int i = 0; i<MAXVALUE; ++i)
	{
		_keyDown.push_back(false);
	}
}