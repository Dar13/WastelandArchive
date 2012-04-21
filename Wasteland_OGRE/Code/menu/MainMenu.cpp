#include "StdAfx.h"

#include "Menu.h"
#include "..\debug\console.h"
#include "..\GameManager.h"

MainMenu::MainMenu()
{
	_stateShutdown = false;
	_deltaTime = 0;
	_oldTime = 0;
}

void MainMenu::Setup(OISManager* Input,OgreManager* Graphics,GUIManager* Gui)
{
	if(_stateShutdown)
		_stateShutdown = false;
	
	if(!Gui->doesGUISheetExist("main_Root"))
	{
		_guiSheet = Gui->getWinManager()->createWindow("DefaultWindow","main_Root");
		Gui->addGUISheet(_guiSheet);
		std::pair<std::string,CEGUI::Window*> window;
		window.first = "main_Exit_btn";
		window.second = Gui->getWinManager()->createWindow("SleekSpace/Button",window.first);
		window.second->setText("Exit Game");
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.5,0),CEGUI::UDim(.5,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.1,0),CEGUI::UDim(.1,0)));
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MainMenu::_shutdown,this));
	
		window.first = "main_Start_btn";
		window.second = Gui->getWinManager()->createWindow("SleekSpace/Button",window.first);
		window.second->setText("Start Game");
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.5,0),CEGUI::UDim(.4,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.1,0),CEGUI::UDim(.1,0)));
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MainMenu::_start,this));
	}

	Gui->setCurrentGUISheet("main_Root");

	return;
}

int MainMenu::Run(OISManager* Input,OgreManager* Graphics,GUIManager* Gui)
{
	while(!_stateShutdown)
	{
		Input->Update(false);

		_deltaTime = Graphics->getTimer()->getMilliseconds() - _oldTime;
		_oldTime = static_cast<float>(Graphics->getTimer()->getMilliseconds());

		Gui->Update(_deltaTime);
		GameManager::UpdateManagers(Graphics,NULL,_deltaTime);
	}

	return _returnValue;
}

void MainMenu::Shutdown(OISManager* Input,OgreManager* Graphics,GUIManager* Gui)
{
	//not really sure what to do here. Delete the GUI sheet from the manager?
	//Do I really need to?
	Gui->setCurrentGUISheet("none");
	return;
}

bool MainMenu::_shutdown(const CEGUI::EventArgs& arg)
{
	_stateShutdown = true;
	_returnValue = END;

	return true;
}

bool MainMenu::_start(const CEGUI::EventArgs& arg)
{
	_stateShutdown = true;
	_returnValue = GAME_ARENA;

	return true;
}