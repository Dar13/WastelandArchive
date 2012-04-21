#include "StdAfx.h"

#ifndef _MENU_H_
#define _MENU_H_

#include "..\State.h"
#include "..\OISManager.h"
#include "..\OgreManager.h"
#include "..\GUIManager.h"
#include <vector>

class Menu : public State
{
protected:
	//!Handling the GUI sheet's child windows are the responsibility of the derived class.
	CEGUI::Window* _guiSheet;
};

class MainMenu : public Menu
{
public:
	MainMenu();

	void Setup(OISManager* Input,OgreManager* Graphics,GUIManager* Gui);

	int Run(OISManager* Input,OgreManager* Graphics,GUIManager* Gui);

	void Shutdown(OISManager* Input,OgreManager* Graphics,GUIManager* Gui);

	bool _shutdown(const CEGUI::EventArgs&);
	bool _start(const CEGUI::EventArgs&);
private:
	std::map<std::string,CEGUI::Window*> _guiSheetChildren;
	int _returnValue; //used because there might be two different possible game states that this class exits to.
};

#endif