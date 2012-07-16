#include "StdAfx.h"

#ifndef _MENU_H_
#define _MENU_H_

#include "..\State.h"
#include "..\InputManager.h"
#include "..\GraphicsManager.h"
#include "..\GUIManager.h"
#include <vector>

class Menu : public State
{
protected:
	//!Handling the GUI sheet's child windows are the responsibility of the derived class.
	CEGUI::Window* _guiSheet;
};

class MainMenu_FaderCallback : public ScreenFaderCallback
{
public:
	void fadeInCallback();
	void fadeOutCallback();

	void updateFade(double progress);

	bool isFadeFinished() { return _finished; }

	void setupMusicFade(SoundManager* soundMgr);

private:
	bool _finished;
	SoundManager* _soundManager;
};

class MainMenu : public Menu
{
public:
	MainMenu();

	void Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

	int Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

	void Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

	void createOptionsMenu(GUIManager* Gui);

	bool _shutdown(const CEGUI::EventArgs&);
	bool _start(const CEGUI::EventArgs&);
	bool _options(const CEGUI::EventArgs&);

	bool _valueUpdate_sliders(const CEGUI::EventArgs&);
private:
	std::map<std::string,CEGUI::Window*> _guiSheetChildren;
	int _returnValue; //used because there might be two different possible game states that this class exits to.
	
	CEGUI::Window* _opt_guiSheet;
	std::map<std::string,CEGUI::Window*> _opt_guiSheetChildren;
	bool _goto_Options;

	//Ogre stuff
	Ogre::SceneManager* _scene;
	Ogre::Camera* _camera;
	Ogre::Viewport* _view;
	Ogre::SceneNode* _camNode,*_cityNode;
	Ogre::SceneNode* _lightNode;
	//! test light
	Ogre::SceneNode* _testLight;
	
	//Adding a screen fader
	ScreenFader* _fader;
	MainMenu_FaderCallback _faderCallback;

	//FMOD stuff
	std::vector<sSound> _sounds;
	std::vector<FMOD::Channel*> _channels;
};

#endif