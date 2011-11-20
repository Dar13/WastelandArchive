#ifndef _OIS_MANAGER_H_
#define _OIS_MANAGER_H_

#include <OIS.h>
#include <OgreSingleton.h>

class OISManager : public Ogre::Singleton<OISManager>,OIS::MouseListener, OIS::KeyListener
{
public:
	OISManager(unsigned long windowHandle);
	~OISManager();

	//Some setup function(s).
	void setCaptureWindow(int width,int height);

	//Capture the input
	void capture();

	//OIS handler functions
	bool keyPressed(const OIS::KeyEvent &evt);
	bool keyReleased(const OIS::KeyEvent &evt);
	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt,OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt,OIS::MouseButtonID id);

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