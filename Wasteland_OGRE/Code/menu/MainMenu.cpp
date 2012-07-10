#include "StdAfx.h"

#include "Menu.h"
#include "..\debug\console.h"
#include "..\GameManager.h"
#include <boost\lexical_cast.hpp>

MainMenu::MainMenu()
{
	_stateShutdown = false;
	_deltaTime = 0;
	_oldTime = 0;
	_goto_Options = false;
	_scene = nullptr;
	_camera = nullptr;
	_view = nullptr;
	_returnValue = ApplicationStates::END;
	_opt_guiSheet = nullptr;
	_camNode = nullptr;
	_cityNode = nullptr;
	_lightNode = nullptr;
	_testLight = nullptr;
}

void MainMenu::Setup(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	if(_stateShutdown)
		_stateShutdown = false;

	_scene = Graphics->getRoot()->createSceneManager(Ogre::ST_EXTERIOR_FAR,"menuMain");
	_camera = _scene->createCamera("menuCam");
	_camera->setAspectRatio(16.0f/9.0f);
	_view = Graphics->getRenderWindow()->addViewport(_camera);
	_view->setBackgroundColour(Ogre::ColourValue(0,0,0));

	std::auto_ptr<list_t> objList = list("resource\\xml\\lists\\intro_list.xml");
	for(list_t::file_const_iterator itr = objList.get()->file().begin(); itr != objList.get()->file().end(); ++itr)
	{
		std::string tmp = (*itr);
		std::auto_ptr<object_t> tmpObj = object(tmp.c_str());
		_cityNode = Graphics->createSceneNode(_scene,tmpObj.get(),NULL);
	}

	_cityNode->setFixedYawAxis(true,Ogre::Vector3::UNIT_Y);

	_camNode = _cityNode->createChildSceneNode("mmCamNode");
	_camNode->attachObject(_camera);
	_camNode->setPosition(Ogre::Vector3(750.0f,500.0f,0.0f));
	//_cityNode->setVisible(false,true);

	Ogre::Light* menuLight = _scene->createLight("mmLight");
	menuLight->setType(Ogre::Light::LT_POINT);
	Graphics->setLightRange(menuLight,1000.0f);
	menuLight->setSpecularColour(Ogre::ColourValue(0.0f,0.0f,1.0f,.5f));
	_lightNode = _camNode->createChildSceneNode("mmLightNode");
	_lightNode->attachObject(menuLight);
	
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
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.425f,0),CEGUI::UDim(.7f,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.15f,0),CEGUI::UDim(.1f,0)));
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MainMenu::_shutdown,this));
		window.second->setAlwaysOnTop(true);
	
		window.first = "main_Start_btn";
		window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
		window.second->setText("Start Game");
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.425f,0),CEGUI::UDim(.3f,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.15f,0),CEGUI::UDim(.1f,0)));
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MainMenu::_start,this));
		window.second->setAlwaysOnTop(true);

		window.first = "main_Options_btn";
		window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.45f,0),CEGUI::UDim(.5f,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.1f,0),CEGUI::UDim(.1f,0)));
		window.second->setText("Options");
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MainMenu::_options,this));
		window.second->setAlwaysOnTop(true);
		_guiSheet->addChildWindow(window.second);
		_guiSheetChildren.insert(window);
	}

	Gui->setCurrentGUISheet("main_Root");

	//load and set-up music.
	//DO NOT ADD MUSIC TO LOCAL SOUNDS VECTOR
	sSound sound;
	sound.name = "mainmenu_AtRest";
	sound.type = MUSIC;
	Sound->createSound(sound,"resource\\music\\at_rest.mp3");
	Sound->addMusicToPlaylist(sound);

	sound.name="mainmenu_Unpromised";
	sound.type=MUSIC;
	Sound->createSound(sound,"resource\\music\\unpromised.mp3");
	Sound->addMusicToPlaylist(sound);

	return;
}

int MainMenu::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	Ogre::Real rotProg = 0.0f;
	//one rotation every minute
	Ogre::Real rotFactor = 1.0f / 3600.0f;
	bool isCamRotating = true;

	//no need for separate channels right now. SoundManager takes care of music for me.
	Sound->startMusic();
	
	bool inOptions = false;
	while(!_stateShutdown)
	{
		Input->Update(false);

		_deltaTime = Graphics->getTimer()->getMilliseconds() - _oldTime;
		_oldTime = static_cast<float>(Graphics->getTimer()->getMilliseconds());
		
		//look at origin...
		Ogre::Vector3 localY = _camNode->getOrientation() * Ogre::Vector3::UNIT_Y;
		Ogre::Quaternion quat = localY.getRotationTo(Ogre::Vector3::UNIT_Y);
		_camNode->rotate(quat,Ogre::Node::TS_PARENT);
		_camNode->lookAt(Ogre::Vector3::ZERO,Ogre::Node::TS_WORLD);

		//now i need to rotate about the origin...
		if(isCamRotating)
		{
			rotProg += rotFactor;
			if(rotProg > 1)
			{
				rotProg = rotFactor;
			}
			//use polar coordinates to get new position.
			Ogre::Vector3 positionRotation;
			positionRotation.x = 750.0f * Ogre::Math::Cos((rotProg * Ogre::Math::TWO_PI));
			positionRotation.y = 500.0f;
			positionRotation.z = 750.0f * Ogre::Math::Sin((rotProg * Ogre::Math::TWO_PI));
			_camNode->setPosition(positionRotation);
		}
		
		Sound->Update();

		Gui->Update(_deltaTime);
		GameManager::UpdateManagers(Graphics,NULL,_deltaTime);

		if(_goto_Options && !inOptions)
		{
			if(!Gui->doesGUISheetExist("options_Root"))
			{
				createOptionsMenu(Gui);
			}
			Gui->setCurrentGUISheet("options_Root");
			CEGUI::TabControl* tControl = static_cast<CEGUI::TabControl*>(_opt_guiSheetChildren["opt_Config_tabcntrl"]);
			tControl->setSelectedTab(1);
			inOptions = true;
			isCamRotating = false;
			_cityNode->setVisible(false,true);
		}
		else
		{
			if(_goto_Options == false && inOptions)
			{
				Gui->setCurrentGUISheet("main_Root");
				inOptions = false;
				isCamRotating = true;
				_cityNode->setVisible(true,true);
			}
		}
	}
	//trying to get the screen to clear out.
	GameManager::UpdateManagers(Graphics,NULL,_deltaTime);

	return _returnValue;
}

void MainMenu::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	//undo whatever is done in the setup.
	Gui->setCurrentGUISheet("none");
	CEGUI::MouseCursor::getSingleton().hide();

	Graphics->getRenderWindow()->removeAllViewports();

	_scene->destroyCamera(_camera);

	_scene->clearScene();

	Graphics->getRoot()->destroySceneManager(_scene);

	for(std::vector<FMOD::Channel*>::iterator itr = _channels.begin(); itr != _channels.end(); ++itr)
	{
		//basically kills the channel in the eyes of FMOD.
		(*itr)->stop();
	}

	for(std::vector<sSound>::iterator itr = _sounds.begin(); itr != _sounds.end(); ++itr)
	{
		//need to release this memory.
		(*itr).sound->release();
		(*itr).sound = 0;
	}

	Sound->stopMusic(true);

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
	tabCntrl->setTabHeight(CEGUI::UDim(0.1f,0));
	tabCntrl->setTabTextPadding(CEGUI::UDim(.05f,0));
	tabCntrl->setEnabled(true);
	tabCntrl->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f,0),CEGUI::UDim(1.0f,0)));
	tabCntrl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.0f,0)));
	tabCntrl->setAlwaysOnTop(false);
	window.second = static_cast<CEGUI::Window*>(tabCntrl);
	_opt_guiSheet->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	//graphic options...
	window.first = "opt_Config_Graphic_wnd";
	window.second = Gui->getWinManager()->createWindow("DefaultWindow",window.first);
	window.second->setText("Graphics");
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.0f,0)));
	window.second->setID(1);
	tabCntrl->addTab(window.second);
	_opt_guiSheetChildren.insert(window);

	//resolution list
	window.first = "opt_Config_Graphic_Res_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setText("Resolutions:");
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f,5),CEGUI::UDim(0.05f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.2f,0),CEGUI::UDim(.05f,0)));
	_opt_guiSheetChildren["opt_Config_Graphic_wnd"]->addChildWindow(window.second);

	window.first = "opt_Config_Graphic_Res_list";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Combobox",window.first);
	_opt_guiSheetChildren["opt_Config_Graphic_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	CEGUI::Combobox* resCombobox = static_cast<CEGUI::Combobox*>(window.second);
	resCombobox->setReadOnly(true);
	resCombobox->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f,5),CEGUI::UDim(.1f,0)));
	resCombobox->setSize(CEGUI::UVector2(CEGUI::UDim(.2f,0),CEGUI::UDim(.2f,0)));

	CEGUI::ListboxTextItem* cbItem = new CEGUI::ListboxTextItem("1920x1024",10);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	cbItem->setSelected(true);
	resCombobox->addItem(cbItem);

	cbItem = new CEGUI::ListboxTextItem("1600x1050",20);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	resCombobox->addItem(cbItem);

	cbItem = new CEGUI::ListboxTextItem("1366x768",30);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	resCombobox->addItem(cbItem);
	
	cbItem = new CEGUI::ListboxTextItem("1280x1024",40);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	resCombobox->addItem(cbItem);

	cbItem = new CEGUI::ListboxTextItem("1024x768",50);
	cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
	resCombobox->addItem(cbItem);

	//other graphics options...

	//audio options...
	window.first = "opt_Config_Audio_wnd";
	window.second = Gui->getWinManager()->createWindow("DefaultWindow",window.first);
	window.second->setText("Audio");
	window.second->setID(2);
	tabCntrl->addTab(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_wnd";
	window.second = Gui->getWinManager()->createWindow("DefaultWindow",window.first);
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.5f,0),CEGUI::UDim(.5f,0)));
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f,20),CEGUI::UDim(.0f,0)));
	_opt_guiSheetChildren["opt_Config_Audio_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setText("Volumes");
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.1f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f,0),CEGUI::UDim(0.1f,0)));
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_chr_sldr";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Slider",window.first);
	CEGUI::Slider* sldr = static_cast<CEGUI::Slider*>(window.second);
	sldr->setPosition(CEGUI::UVector2(CEGUI::UDim(0.14f,0),CEGUI::UDim(0.2f,0)));
	sldr->setSize(CEGUI::UVector2(CEGUI::UDim(0.04f,0),CEGUI::UDim(0.4f,0)));
	sldr->setCurrentValue(0.5f);
	window.second = static_cast<CEGUI::Slider*>(sldr);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	window.second->subscribeEvent(CEGUI::Slider::EventValueChanged,CEGUI::Event::Subscriber(&MainMenu::_valueUpdate_sliders,this));

	window.first = "opt_Config_Audio_vol_chr_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f,0),CEGUI::UDim(0.6f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(0.225f,0),CEGUI::UDim(.1f,0)));
	window.second->setText("Character Volume");
	window.second->setFont("DejaVuSans-6");
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_chr_val";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.19f,0),CEGUI::UDim(0.33f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.075f,0),CEGUI::UDim(0.1f,0)));
	window.second->setFont("DejaVuSans-6");
	window.second->setText("50");
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_msc_sldr";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Slider",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.495f,0),CEGUI::UDim(.2f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.04f,0),CEGUI::UDim(.4f,0)));
	sldr = static_cast<CEGUI::Slider*>(window.second);
	sldr->setCurrentValue(.5f);
	window.second = static_cast<CEGUI::Window*>(sldr);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	window.second->subscribeEvent(CEGUI::Slider::EventValueChanged,CEGUI::Event::Subscriber(&MainMenu::_valueUpdate_sliders,this));

	window.first = "opt_Config_Audio_vol_msc_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.4f,0),CEGUI::UDim(0.6f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.225f,0),CEGUI::UDim(.1f,0)));
	window.second->setText("Music Volume");
	window.second->setFont("DejaVuSans-6");
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_msc_val";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.55f,0),CEGUI::UDim(.33f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.075f,0),CEGUI::UDim(.1f,0)));
	window.second->setFont("DejaVuSans-6");
	window.second->setText("50");
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_sfx_sldr";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Slider",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.7925f,0),CEGUI::UDim(.2f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.04f,0),CEGUI::UDim(.4f,0)));
	sldr = static_cast<CEGUI::Slider*>(window.second);
	sldr->setCurrentValue(.5f);
	window.second = static_cast<CEGUI::Window*>(sldr);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	window.second->subscribeEvent(CEGUI::Slider::EventValueChanged,CEGUI::Event::Subscriber(&MainMenu::_valueUpdate_sliders,this));

	window.first = "opt_Config_Audio_vol_sfx_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.7f,0),CEGUI::UDim(0.6f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.225f,0),CEGUI::UDim(.1f,0)));
	window.second->setText("SFX Volume");
	window.second->setFont("DejaVuSans-6");
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_sfx_val";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.85f,0),CEGUI::UDim(.33f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.075f,0),CEGUI::UDim(.1f,0)));
	window.second->setText("50");
	window.second->setFont("DejaVuSans-6");
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
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

bool MainMenu::_valueUpdate_sliders(const CEGUI::EventArgs& arg)
{
	//convert to a more comprehensive event argument class.
	const CEGUI::WindowEventArgs& realArgs = static_cast<const CEGUI::WindowEventArgs&>(arg);
	//get the name of the window that created this event.
	std::string n = realArgs.window->getName().c_str();

	//Get current value. This could get ugly if the window isn't a slider.
	std::string v = "";
	v = boost::lexical_cast<std::string,int>(static_cast<int>(((CEGUI::Slider*)realArgs.window)->getCurrentValue() * 100));
	
	//check for specific sliders and setting their corresponding value text.
	if(n == "opt_Config_Audio_vol_chr_sldr")
	{
		_opt_guiSheetChildren["opt_Config_Audio_vol_chr_val"]->setText(v);
	}
	if(n == "opt_Config_Audio_vol_msc_sldr")
	{
		_opt_guiSheetChildren["opt_Config_Audio_vol_msc_val"]->setText(v);
	}
	if(n == "opt_Config_Audio_vol_sfx_sldr")
	{
		_opt_guiSheetChildren["opt_Config_Audio_vol_sfx_val"]->setText(v);
	}

	return true;
}