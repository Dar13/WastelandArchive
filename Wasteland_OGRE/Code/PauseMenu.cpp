#include "StdAfx.h"

#include "menu\Menu.h"

#include "GameManager.h"

PauseMenu::PauseMenu(int currentGameState)
	: _returnValue(currentGameState)
{
}

void PauseMenu::Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	_stateShutdown = false;

	//Input->setMouseLock(false);

	if(!Gui->doesGUISheetExist("pause_Root"))
	{
		Gui->getSystem()->setDefaultMouseCursor("TaharezLook","MouseArrow");
		//CEGUI::MouseCursor::getSingleton().show();

		_guiSheet = Gui->getWinManager()->createWindow("DefaultWindow","pause_Root");
		CEGUI::WindowManager* winMgr = Gui->getWinManager();

		Gui->addGUISheet(_guiSheet);
		std::pair<std::string,CEGUI::Window*> window;

		window.first = "pause_Resume_btn";
		window.second = winMgr->createWindow("TaharezLook/Button",window.first);
		window.second->setText("Resume Game");
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.425f,0),CEGUI::UDim(.3f,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.15f,0),CEGUI::UDim(.1f,0)));
		window.second->setDestroyedByParent(true);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber([this] (const CEGUI::EventArgs& args) -> bool {
				_stateShutdown = true;
				return true;
			})
		);
		window.second->setAlwaysOnTop(true);
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);

		window.first = "pause_End_btn";
		window.second = winMgr->createWindow("TaharezLook/Button",window.first);
		window.second->setText("End Game");
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.425f,0),CEGUI::UDim(.5f,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.15f,0),CEGUI::UDim(.1f,0)));
		window.second->setDestroyedByParent(true);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber([this] (const CEGUI::EventArgs& args) -> bool {
				_stateShutdown = true;
				_returnValue = State::END;
				return true;
			})
		);
		window.second->setAlwaysOnTop(true);
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);
	}

	return;
}

int PauseMenu::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	Gui->setCurrentGUISheet("pause_Root");
	CEGUI::MouseCursor::getSingleton().show();

	bool locked = Input->getMouseLock();
	Input->setMouseLock(false);

	while(!_stateShutdown)
	{
		Input->Update(false);

		_deltaTime = Graphics->getTimer()->getMilliseconds() - _oldTime;
		_oldTime = static_cast<float>(Graphics->getTimer()->getMilliseconds());

		Gui->Update(_deltaTime);

		if(!GameManager::UpdateManagers(Graphics,NULL,_deltaTime))
		{
			_stateShutdown = true;
			_returnValue = State::END;
		}
	}

	Gui->setCurrentGUISheet("none");

	Input->setMouseLock(locked);
	
	CEGUI::MouseCursor::getSingleton().hide();

	//since the state can be used multiple times...
	_stateShutdown = false;

	return _returnValue;
}

void PauseMenu::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	Gui->setCurrentGUISheet("none");
	Gui->removeGUISheet(_guiSheet);
	Gui->getWinManager()->destroyWindow(_guiSheet);
	
	Input->setMouseLock(true);

	_guiSheetChildren.clear();

	return;
}