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

#ifndef _PAUSE_ONLY_

class MainMenu_FaderCallback : public ScreenFaderCallback
{
public:
	void fadeInCallback();
	void fadeOutCallback();

	void updateFade(double progress,double currentTime,int fadeOp);

	bool isFadeFinished() { return _finished; }

	void setupMusicFade(SoundManager* soundMgr,bool fadeDown = false);
	void setupGUIFade(CEGUI::Window* GUISheet,int fadeInTime,int fadeOutTime);

private:
	int _guiFadeIn,_guiFadeOut;
	bool _finished;
	bool _fadeMusicDown;
	SoundManager* _soundManager;
	CEGUI::Window* _guiSheet;
};

class MainMenu : public Menu
{
public:
	MainMenu();

	void Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

	int Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

	void Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

	void createOptionsMenu(InputManager* Input,GUIManager* Gui);

	bool _valueUpdate_sliders(const CEGUI::EventArgs&);
private:
	void _setupOptionEvents(InputManager* Input);
	void _saveOptionChanges();

	std::map<std::string,CEGUI::Window*> _guiSheetChildren;
	
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
	ScreenFaderUpdater _faderUpdater;

	//FMOD stuff
	std::vector<sSound> _sounds;
	std::vector<FMOD::Channel*> _channels;
};

#endif

class PauseMenu : public Menu
{
public:
	PauseMenu(int currentGameState);

	void Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

	int Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

	void Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound);

private:
	std::map<std::string,CEGUI::Window*> _guiSheetChildren;

	int _returnValue; //returns either the current game state, or State::END
};

#endif