#include "StdAfx.h"

#include "Menu.h"
#include "..\debug\console.h"
#include "..\GameManager.h"

MainMenu::MainMenu()
{
	_stateShutdown = false;
	_deltaTime = 0;
	_oldTime = 0;
	_goto_Options = false;
}

void MainMenu::Setup(OISManager* Input,OgreManager* Graphics,GUIManager* Gui)
{
	if(_stateShutdown)
		_stateShutdown = false;

	_scene = Graphics->getRoot()->createSceneManager(Ogre::ST_EXTERIOR_FAR,"menuMain");
	_camera = _scene->createCamera("menuCam");
	_camera->setAspectRatio(16.0f/9.0f);
	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));
	
	if(!Gui->doesGUISheetExist("main_Root"))
	{
		Gui->getSystem()->setDefaultMouseCursor("TaharezLook","MouseArrow");
		CEGUI::MouseCursor::getSingleton().show();

		_guiSheet = Gui->getWinManager()->createWindow("DefaultWindow","main_Root");
		Gui->addGUISheet(_guiSheet);
		std::pair<std::string,CEGUI::Window*> window;

		window.first = "main_Exit_btn";
		window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
		window.second->setText("Exit Game");
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.5,0),CEGUI::UDim(.7,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.1,0),CEGUI::UDim(.1,0)));
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MainMenu::_shutdown,this));
		window.second->setAlwaysOnTop(true);
	
		window.first = "main_Start_btn";
		window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
		window.second->setText("Start Game");
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.5,0),CEGUI::UDim(.3,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.1,0),CEGUI::UDim(.1,0)));
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MainMenu::_start,this));
		window.second->setAlwaysOnTop(true);

		window.first = "main_Options_btn";
		window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.5f,0),CEGUI::UDim(.5f,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.1f,0),CEGUI::UDim(.1f,0)));
		window.second->setText("Options");
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MainMenu::_options,this));
		window.second->setAlwaysOnTop(true);
		_guiSheet->addChildWindow(window.second);
		_guiSheetChildren.insert(window);
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

		if(_goto_Options)
		{
			if(!Gui->doesGUISheetExist("options_Root"))
			{
				createOptionsMenu(Gui);
			}
			Gui->setCurrentGUISheet("options_Root");
		}
		else
			Gui->setCurrentGUISheet("main_Root");
	}
	//trying to get the screen to clear out.
	GameManager::UpdateManagers(Graphics,NULL,_deltaTime);

	return _returnValue;
}

void MainMenu::Shutdown(OISManager* Input,OgreManager* Graphics,GUIManager* Gui)
{
	//undo whatever is done in the setup.
	Gui->setCurrentGUISheet("none");
	CEGUI::MouseCursor::getSingleton().hide();

	Graphics->getRenderWindow()->removeAllViewports();

	_scene->destroyCamera(_camera);

	_scene->clearScene();

	Graphics->getRoot()->destroySceneManager(_scene);

	return;
}

void MainMenu::createOptionsMenu(GUIManager* Gui)
{
	CEGUI::Window* _opt_guiSheet = Gui->getWinManager()->createWindow("DefaultWindow","options_Root");
	Gui->addGUISheet(_opt_guiSheet);
	Gui->setCurrentGUISheet("options_Root");

	std::pair<std::string,CEGUI::Window*> window;

	//tab control...
	window.first = "opt_Config_tabcntrl";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/TabControl",window.first);
	CEGUI::TabControl* tabCntrl = static_cast<CEGUI::TabControl*>(window.second);
	tabCntrl->setTabHeight(CEGUI::UDim(0.1,0));
	tabCntrl->setEnabled(true);
	tabCntrl->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f,0),CEGUI::UDim(1.0f,0)));
	tabCntrl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0,0),CEGUI::UDim(0.0,0)));
	tabCntrl->setAlwaysOnTop(false);
	window.second = static_cast<CEGUI::Window*>(tabCntrl);
	_opt_guiSheet->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	//graphic options...
	window.first = "opt_Config_Graphic_wnd";
	window.second = Gui->getWinManager()->createWindow("DefaultWindow",window.first);
	window.second->setText("Graphic Options");
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.0f,0)));
	tabCntrl->addTab(window.second);
	_opt_guiSheetChildren.insert(window);

	//resolution list
	window.first = "opt_Config_Graphic_Res_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setText("Resolutions:");
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.0,5),CEGUI::UDim(0.05,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.2,0),CEGUI::UDim(.05,0)));
	_opt_guiSheetChildren["opt_Config_Graphic_wnd"]->addChildWindow(window.second);

	window.first = "opt_Config_Graphic_Res_list";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Combobox",window.first);
	_opt_guiSheetChildren["opt_Config_Graphic_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	CEGUI::Combobox* resCombobox = static_cast<CEGUI::Combobox*>(window.second);
	resCombobox->setReadOnly(true);
	resCombobox->setPosition(CEGUI::UVector2(CEGUI::UDim(.0,5),CEGUI::UDim(.1,0)));
	resCombobox->setSize(CEGUI::UVector2(CEGUI::UDim(.2,0),CEGUI::UDim(.2,0)));

	CEGUI::ListboxTextItem* cbItem = new CEGUI::ListboxTextItem("1920x1024",1);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	cbItem->setSelected(true);
	resCombobox->addItem(cbItem);

	cbItem = new CEGUI::ListboxTextItem("1600x1050",2);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	resCombobox->addItem(cbItem);

	cbItem = new CEGUI::ListboxTextItem("1366x768",3);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	resCombobox->addItem(cbItem);
	
	cbItem = new CEGUI::ListboxTextItem("1280x1024",4);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	resCombobox->addItem(cbItem);

	cbItem = new CEGUI::ListboxTextItem("1024x768",5);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	resCombobox->addItem(cbItem);

	//other graphics options...

	//audio options...
	window.first = "opt_Config_Audio_wnd";
	window.second = Gui->getWinManager()->createWindow("DefaultWindow",window.first);
	window.second->setText("Audio Options");
	tabCntrl->addTab(window.second);
	_opt_guiSheetChildren.insert(window);

	//option menu exit button
	window.first = "opt_Exit_btn";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
	window.second->setText("Exit to Main Menu");
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f,0),CEGUI::UDim(0.1f,0)));
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8f,0),CEGUI::UDim(0.9f,0)));
	window.second->setAlwaysOnTop(true);
	_opt_guiSheet->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MainMenu::_options,this));

}

bool MainMenu::_shutdown(const CEGUI::EventArgs& arg)
{
	_stateShutdown = true;
	_returnValue = END;

	return true;
}

bool MainMenu::_options(const CEGUI::EventArgs& arg)
{
	if(_goto_Options)
		_goto_Options = false;
	else
		_goto_Options = true;

	return true;
}

bool MainMenu::_start(const CEGUI::EventArgs& arg)
{
	_stateShutdown = true;
	_returnValue = GAME_ARENA;

	return true;
}