#include "StdAfx.h"

#include "Menu.h"
#include "..\debug\console.h"

#define _GM_NO_EXTRAS_
#include "..\GameManager.h"
#undef _GM_NO_EXTRAS_

#include <boost\lexical_cast.hpp>

#include <xercesc\dom\DOM.hpp>

MainMenu::MainMenu()
{
	_stateShutdown = false;
	_deltaTime = 0;
	_oldTime = 0;
	_goto_Options = false;
	_scene = nullptr;
	_camera = nullptr;
	_view = nullptr;
	_returnValue = State::END;
	_opt_guiSheet = nullptr;
	_camNode = nullptr;
	_cityNode = nullptr;
	_lightNode = nullptr;
	_testLight = nullptr;
	_fader = nullptr;
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

	std::cout << _scene->getAmbientLight();

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


	//this is all fucked up.
	Ogre::Light* menuLight = _scene->createLight("mmLight");
	menuLight->setType(Ogre::Light::LT_POINT);
	Graphics->setLightRange(menuLight,1500.0f);
	menuLight->setSpecularColour(Ogre::ColourValue(0.0f,0.0f,1.0f,1.0f));
	_lightNode = _scene->getRootSceneNode()->createChildSceneNode("mmLightNode");
	_lightNode->setPosition(0.0f,0,750.0f);
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
		window.second->setDestroyedByParent(true);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber([this] (const CEGUI::EventArgs& args) -> bool 
			{
				_stateShutdown = true;
				_returnValue = END;
				return true;	
			})
		);
		window.second->setAlwaysOnTop(true);
		window.second->setInheritsAlpha(true);
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);
	
		window.first = "main_Start_btn";
		window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
		window.second->setText("Start Game");
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.425f,0),CEGUI::UDim(.3f,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.15f,0),CEGUI::UDim(.1f,0)));
		window.second->setDestroyedByParent(true);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber([this] (const CEGUI::EventArgs& args) -> bool
			{
				_stateShutdown = true;
				_returnValue = State::GAME_ARENA;
				return true;
			})
		);
		window.second->setInheritsAlpha(true);
		window.second->setAlwaysOnTop(true);
		_guiSheetChildren.insert(window);
		_guiSheet->addChildWindow(window.second);

		window.first = "main_Options_btn";
		window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.45f,0),CEGUI::UDim(.5f,0)));
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.1f,0),CEGUI::UDim(.1f,0)));
		window.second->setDestroyedByParent(true);
		window.second->setText("Options");
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber([this] (const CEGUI::EventArgs& args) -> bool 
			{
				if(_goto_Options)
					_goto_Options = false;
				else
					_goto_Options = true;

				return true;
			})
		);
		window.second->setAlwaysOnTop(true);
		window.second->setInheritsAlpha(true);
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

	_fader = new ScreenFader("Overlays/FadeInOut","Overlays/FadeMaterial",&_faderCallback);
	_faderUpdater.setFader(_fader);

	Graphics->getRoot()->addFrameListener(&_faderUpdater);

	return;
}

int MainMenu::Run(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	Ogre::Real rotProg = 0.0f;
	//one rotation every minute
	Ogre::Real rotFactor = 1.0f / /*3600.0f*/ 1800.0f;
	bool isCamRotating = true;

	//no need for separate channels right now. SoundManager takes care of music for me.
	Sound->startMusic();
	//fade-in to menu
	_faderCallback.setupMusicFade(Sound);
	_faderCallback.setupGUIFade(_guiSheet,2,2);
	_fader->startFadeIn(4.0);
	Gui->setCurrentGUISheet("main_Root");
	
	//bool fadingIn = true;
	bool fadingOut = false;
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
		
		Sound->Update(Input->getConfiguration());

		Gui->Update(_deltaTime);
		//GameManager::RenderScene(Graphics,_view);
		GameManager::UpdateManagers(Graphics,NULL,_deltaTime);

		if(_goto_Options && !inOptions)
		{
			if(!Gui->doesGUISheetExist("options_Root"))
			{
				createOptionsMenu(Input,Gui);
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
				_saveOptionChanges();
				Gui->setCurrentGUISheet("main_Root");
				inOptions = false;
				isCamRotating = true;
				_cityNode->setVisible(true,true);
			}
		}

		if(_stateShutdown && !fadingOut)
		{
			fadingOut = true;
			_faderCallback.setupMusicFade(Sound,fadingOut);
			_fader->startFadeOut(4.0);
			_stateShutdown = false;
		}

		if(fadingOut)
		{
			if(_faderCallback.isFadeFinished())
			{
				_stateShutdown = true;
			}
		}

	}

	//update the configuration file
	Input->setConfiguration(configuration("resource//xml//config.xml").release());

	return _returnValue;
}

void MainMenu::Shutdown(InputManager* Input,GraphicsManager* Graphics,GUIManager* Gui,SoundManager* Sound)
{
	//undo whatever is done in the setup.
	Gui->setCurrentGUISheet("none");
	Gui->removeGUISheet(_guiSheet);
	Gui->removeGUISheet(_opt_guiSheet);
	Gui->getWinManager()->destroyWindow(_guiSheet);
	Gui->getWinManager()->destroyWindow(_opt_guiSheet);
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

	_guiSheetChildren.clear();
	_opt_guiSheetChildren.clear();

	delete _fader;
	Graphics->getRoot()->removeFrameListener(&_faderUpdater);

	return;
}

void MainMenu::createOptionsMenu(InputManager* Input,GUIManager* Gui)
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
	window.second->setDestroyedByParent(true);
	_opt_guiSheet->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	
	//graphic options...
	window.first = "opt_Config_Graphic_wnd";
	window.second = Gui->getWinManager()->createWindow("DefaultWindow",window.first);
	window.second->setText("Graphics");
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.0f,0)));
	window.second->setID(1);
	window.second->setDestroyedByParent(true);
	tabCntrl->addTab(window.second);
	_opt_guiSheetChildren.insert(window);

	//resolution list
	window.first = "opt_Config_Graphic_Res_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setText("Resolutions:");
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f,5),CEGUI::UDim(0.05f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.2f,0),CEGUI::UDim(.05f,0)));
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Graphic_wnd"]->addChildWindow(window.second);

	window.first = "opt_Config_Graphic_Res_list";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Combobox",window.first);
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Graphic_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	CEGUI::Combobox* resCombobox = static_cast<CEGUI::Combobox*>(window.second);
	resCombobox->setReadOnly(true);
	resCombobox->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f,5),CEGUI::UDim(.1f,0)));
	resCombobox->setSize(CEGUI::UVector2(CEGUI::UDim(.2f,0),CEGUI::UDim(.2f,0)));

	std::string resList[5] = {"1920x1080","1600x1050","1366x768","1280x1024","1024x768"};
	for(int i = 0; i < 5; i++)
	{
		CEGUI::ListboxTextItem* cbItem = new CEGUI::ListboxTextItem(resList[i]);
		cbItem->setSelectionBrushImage("TaharezLook","ComboboxSelectionBrush");
		if(Input->getConfiguration()->graphics().resolution() == resList[i])
		{
			cbItem->setSelected(true);
		}
		else
		{
			cbItem->setSelected(false);
		}
		cbItem->setAutoDeleted(true);
		resCombobox->addItem(cbItem);
	}

	//fullscreen
	window.first = "opt_Config_Graphic_fscrn_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.2f,0),CEGUI::UDim(.05f,0)));
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25f,0),CEGUI::UDim(.05f,0)));
	window.second->setText("Fullscreen");
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Graphic_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Graphic_fscrn_val";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.2f,0),CEGUI::UDim(.05f,0)));
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.25f,0),CEGUI::UDim(.1f,0)));
	window.second->setDestroyedByParent(true);
	window.second->setText(Input->getConfiguration()->graphics().fullscreen());
	window.second->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber([this] (const CEGUI::EventArgs& args) -> bool 
		{
			const CEGUI::WindowEventArgs& realArgs = static_cast<const CEGUI::WindowEventArgs&>(args);
			if(realArgs.window->getText() == "True")
			{
				realArgs.window->setText("False");
			}
			else
			{
				realArgs.window->setText("True");
			}

			return true;
		})
	);
	_opt_guiSheetChildren["opt_Config_Graphic_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	//other graphics options...

	//audio options...
	window.first = "opt_Config_Audio_wnd";
	window.second = Gui->getWinManager()->createWindow("DefaultWindow",window.first);
	window.second->setText("Audio");
	window.second->setID(2);
	window.second->setDestroyedByParent(true);
	tabCntrl->addTab(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_wnd";
	window.second = Gui->getWinManager()->createWindow("DefaultWindow",window.first);
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.5f,0),CEGUI::UDim(.5f,0)));
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f,20),CEGUI::UDim(.0f,0)));
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setText("Volumes");
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.1f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f,0),CEGUI::UDim(0.1f,0)));
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_chr_sldr";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Slider",window.first);
	CEGUI::Slider* sldr = static_cast<CEGUI::Slider*>(window.second);
	sldr->setPosition(CEGUI::UVector2(CEGUI::UDim(0.14f,0),CEGUI::UDim(0.2f,0)));
	sldr->setSize(CEGUI::UVector2(CEGUI::UDim(0.04f,0),CEGUI::UDim(0.4f,0)));
	sldr->setCurrentValue(0.5f);
	window.second = static_cast<CEGUI::Slider*>(sldr);
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	window.second->subscribeEvent(CEGUI::Slider::EventValueChanged,CEGUI::Event::Subscriber(&MainMenu::_valueUpdate_sliders,this));

	window.first = "opt_Config_Audio_vol_chr_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f,0),CEGUI::UDim(0.6f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(0.225f,0),CEGUI::UDim(.1f,0)));
	window.second->setText("Character Volume");
	window.second->setFont("DejaVuSans-6");
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_chr_val";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.19f,0),CEGUI::UDim(0.33f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.075f,0),CEGUI::UDim(0.1f,0)));
	window.second->setFont("DejaVuSans-6");
	window.second->setText("50");
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_msc_sldr";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Slider",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.495f,0),CEGUI::UDim(.2f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.04f,0),CEGUI::UDim(.4f,0)));
	sldr = static_cast<CEGUI::Slider*>(window.second);
	sldr->setCurrentValue(.5f);
	window.second = static_cast<CEGUI::Window*>(sldr);
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	window.second->subscribeEvent(CEGUI::Slider::EventValueChanged,CEGUI::Event::Subscriber(&MainMenu::_valueUpdate_sliders,this));

	window.first = "opt_Config_Audio_vol_msc_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.4f,0),CEGUI::UDim(0.6f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.225f,0),CEGUI::UDim(.1f,0)));
	window.second->setText("Music Volume");
	window.second->setFont("DejaVuSans-6");
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_msc_val";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.55f,0),CEGUI::UDim(.33f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.075f,0),CEGUI::UDim(.1f,0)));
	window.second->setFont("DejaVuSans-6");
	window.second->setText("50");
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_sfx_sldr";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Slider",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.7925f,0),CEGUI::UDim(.2f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.04f,0),CEGUI::UDim(.4f,0)));
	sldr = static_cast<CEGUI::Slider*>(window.second);
	sldr->setCurrentValue(.5f);
	window.second = static_cast<CEGUI::Window*>(sldr);
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	window.second->subscribeEvent(CEGUI::Slider::EventValueChanged,CEGUI::Event::Subscriber(&MainMenu::_valueUpdate_sliders,this));

	window.first = "opt_Config_Audio_vol_sfx_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.7f,0),CEGUI::UDim(0.6f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.225f,0),CEGUI::UDim(.1f,0)));
	window.second->setText("SFX Volume");
	window.second->setFont("DejaVuSans-6");
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Audio_vol_sfx_val";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.85f,0),CEGUI::UDim(.33f,0)));
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.075f,0),CEGUI::UDim(.1f,0)));
	window.second->setText("50");
	window.second->setFont("DejaVuSans-6");
	window.second->setDestroyedByParent(true);
	_opt_guiSheetChildren["opt_Config_Audio_vol_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	//Control options tab
	window.first = "opt_Config_Controls_wnd";
	window.second = Gui->getWinManager()->createWindow("DefaultWindow",window.first);
	window.second->setText("Controls");
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.0f,0)));
	window.second->setID(3);
	window.second->setDestroyedByParent(true);
	tabCntrl->addTab(window.second);
	_opt_guiSheetChildren.insert(window);

	auto controlClicked = [this] (const CEGUI::EventArgs& args) -> bool
	{
		const CEGUI::WindowEventArgs& realArgs = static_cast<const CEGUI::WindowEventArgs&>(args);
		realArgs.window->setText("Press a new key");
		return true;
	};

	float xOffset = .0f;
	float yOffset = .1f;
	std::string desc[11] = {"fwd","bwd","rgt","lft","jmp","spr","rld","ews","use","wep1","wep2"};
	std::string fullDesc[11] = {"Forward","Backward","Right","Left","Jump","Sprint","Reload","EWS","Use","Weapon1","Weapon2"};
	for(int i = 0; i < 11; ++i)
	{
		if(i > 5) { xOffset = .4f; yOffset = .1f + (i - 6)*.05f; }
		else { yOffset = .1f + (i*.05f); }
		window.first = "opt_Config_Controls_"+desc[i]+"_txt";
		window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.1f,0),CEGUI::UDim(.05f,0)));
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(xOffset,0),CEGUI::UDim(yOffset,0)));
		window.second->setText(fullDesc[i]);
		window.second->setFont("DejaVuSans-6");
		_opt_guiSheetChildren["opt_Config_Controls_wnd"]->addChildWindow(window.second);
		_opt_guiSheetChildren.insert(window);
	}

	window.first = "opt_Config_Controls_move_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.225f,0),CEGUI::UDim(.05f,0)));
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f,0),CEGUI::UDim(.05f,0)));
	window.second->setText("Movement");
	_opt_guiSheetChildren["opt_Config_Controls_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	window.first = "opt_Config_Controls_act_txt";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/StaticText",window.first);
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.225f,0),CEGUI::UDim(.05f,0)));
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(.4f,0),CEGUI::UDim(.05f,0)));
	window.second->setText("Actions");
	_opt_guiSheetChildren["opt_Config_Controls_wnd"]->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);

	xOffset = .1f;
	yOffset = .1f;
	for(int i = 0; i < 11; ++i)
	{
		if(i < InputManager::USE){ xOffset = .1f; yOffset = .1f + i*.05f;}
		else{ xOffset = .5f; yOffset = .1f + (i - 6)*0.05f;}
		window.first = "opt_Config_Controls_"+desc[i]+"_val";
		window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
		window.second->setSize(CEGUI::UVector2(CEGUI::UDim(.125f,0),CEGUI::UDim(.05f,0)));
		window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(xOffset,0),CEGUI::UDim(yOffset,0)));
		window.second->setFont("DejaVuSans-6");
		_opt_guiSheetChildren["opt_Config_Controls_wnd"]->addChildWindow(window.second);
		_opt_guiSheetChildren.insert(window);
		window.second->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(controlClicked));

		switch(i)
		{
		case InputManager::FORWARD:
			window.second->setText(Input->getConfiguration()->movement().forward());
			break;
		case InputManager::BACKWARD:
			window.second->setText(Input->getConfiguration()->movement().backward());
			break;
		case InputManager::RIGHT:
			window.second->setText(Input->getConfiguration()->movement().right());
			break;
		case InputManager::LEFT:
			window.second->setText(Input->getConfiguration()->movement().left());
			break;
		case InputManager::JUMP:
			window.second->setText(Input->getConfiguration()->movement().jump());
			break;
		case InputManager::SPRINT:
			window.second->setText(Input->getConfiguration()->movement().sprint());
			break;
		case InputManager::RELOAD:
			window.second->setText(Input->getConfiguration()->action().reload());
			break;
		case InputManager::ENVWARNSYS:
			window.second->setText(Input->getConfiguration()->action().envwarnsys());
			break;
		case InputManager::USE:
			window.second->setText(Input->getConfiguration()->action().use());
			break;
		case InputManager::WEAPON1:
			window.second->setText(Input->getConfiguration()->action().weapon1());
			break;
		case InputManager::WEAPON2:
			window.second->setText(Input->getConfiguration()->action().weapon2());
			break;
		}
	}

	//option menu exit button
	window.first = "opt_Exit_btn";
	window.second = Gui->getWinManager()->createWindow("TaharezLook/Button",window.first);
	window.second->setText("Exit to Main Menu");
	window.second->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f,0),CEGUI::UDim(0.1f,0)));
	window.second->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8f,0),CEGUI::UDim(0.9f,0)));
	window.second->setAlwaysOnTop(true);
	window.second->setDestroyedByParent(true);
	_opt_guiSheet->addChildWindow(window.second);
	_opt_guiSheetChildren.insert(window);
	window.second->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber([this] (const CEGUI::EventArgs& args) -> bool 
		{
			if(_goto_Options)
				_goto_Options = false;
			else
				_goto_Options = true;

			return true;
		})
	);


	_setupOptionEvents(Input);
}

void MainMenu::_setupOptionEvents(InputManager* Input)
{
	auto func = [this,Input] (const CEGUI::EventArgs& args) -> bool
	{
		const CEGUI::KeyEventArgs& realArgs = static_cast<const CEGUI::KeyEventArgs&>(args);
		std::string str;
		str = Input->__getKeyboard()->getAsString(static_cast<OIS::KeyCode>(realArgs.scancode));
		

		//need to check for left and right variations of some keys.
		switch(realArgs.scancode)
		{
		case CEGUI::Key::LeftAlt:
		case CEGUI::Key::LeftBracket:
		case CEGUI::Key::LeftControl:
		case CEGUI::Key::LeftShift:
		case CEGUI::Key::LeftWindows:
			str.insert(str.begin(),'L');
			break;
		}
			
		realArgs.window->setText(str);
		return true;
	};
	_opt_guiSheetChildren["opt_Config_Controls_fwd_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_bwd_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_rgt_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_lft_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_spr_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_jmp_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_rld_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_ews_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_use_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_wep1_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));

	_opt_guiSheetChildren["opt_Config_Controls_wep2_val"]->subscribeEvent(CEGUI::PushButton::EventKeyDown,
		CEGUI::Event::Subscriber(func));
}

void MainMenu::_saveOptionChanges()
{
	//get the information first
	std::vector<std::string> actionValues,movementValues;
	int characterValue,musicValue,soundValue;

	CEGUI::Slider* slider = static_cast<CEGUI::Slider*>(_opt_guiSheetChildren["opt_Config_Audio_vol_sfx_sldr"]);
	soundValue = static_cast<int>(slider->getCurrentValue() * 100);

	slider = static_cast<CEGUI::Slider*>(_opt_guiSheetChildren["opt_Config_Audio_vol_chr_sldr"]);
	characterValue = static_cast<int>(slider->getCurrentValue() * 100);

	slider = static_cast<CEGUI::Slider*>(_opt_guiSheetChildren["opt_Config_Audio_vol_msc_sldr"]);
	musicValue = static_cast<int>(slider->getCurrentValue() * 100);

	std::string desc[11] = {"fwd","bwd","rgt","lft","jmp","spr","rld","ews","use","wep1","wep2"};
	std::wstring fullDesc[11] = {L"forward",L"backward",L"right",L"left",L"sprint",L"jump",L"reload",L"envwarnsys",L"use",L"weapon1",L"weapon2"};
	for(int i = 0; i < 11; ++i)
	{
		if(i < 6)
		{
			movementValues.push_back(_opt_guiSheetChildren["opt_Config_Controls_"+desc[i]+"_val"]->getText().c_str());
		}
		else
		{
			actionValues.push_back(_opt_guiSheetChildren["opt_Config_Controls_"+desc[i]+"_val"]->getText().c_str());
		}
	}

	//serialize what we have
	xercesc::XMLPlatformUtils::Initialize();

	xercesc::DOMImplementation* domImpl = nullptr;
	xercesc::DOMImplementation* serImpl = xercesc::DOMImplementationRegistry::getDOMImplementation(L"LS");
	domImpl = xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode("core"));

	xercesc::DOMLSSerializer* serializer = serImpl->createLSSerializer();
	//xercesc::DOMLSOutput* serOutput = serImpl->createLSOutput();
	//serOutput->set

	serializer->getDomConfig()->setParameter(L"format-pretty-print",true);
	serializer->setNewLine(L"\n");

	xercesc::DOMDocument* doc = domImpl->createDocument(0,L"configuration",0);
	doc->setXmlStandalone(false);
	doc->setXmlVersion(L"1.0");

	//setting up the DOM
	xercesc::DOMElement* root = static_cast<xercesc::DOMElement*>(doc->getFirstChild());
	root->setAttribute(L"xmlns:xsi",L"http://www.w3.org/2001/XMLSchema-instance");
	root->setAttribute(L"xsi:noNamespaceSchemaLocation",L"schemas/configuration.xsd");

	//graphics stuff
	xercesc::DOMElement* graphics = doc->createElement(L"graphics");
	root->appendChild(graphics);
	
	//std::wstring graphicsOpts[2] = {L"resolution",L"fullscreen"};

	CEGUI::Combobox* list = static_cast<CEGUI::Combobox*>(_opt_guiSheetChildren["opt_Config_Graphic_Res_list"]);
	CEGUI::ListboxItem* item = list->getSelectedItem();
	xercesc::DOMElement* element;

	//can't combine these two options into one nice for-loop like I did with the movement/action values
	//because the way of getting the values are radically different.
	//I can at least order them properly though.
	element = doc->createElement(L"resolution");
	graphics->appendChild(element);
	std::string tmp = item->getText().c_str();
	element->appendChild(doc->createTextNode(Utility::stringToWString(tmp).c_str()));

	element = doc->createElement(L"fullscreen");
	graphics->appendChild(element);
	tmp = _opt_guiSheetChildren["opt_Config_Graphic_fscrn_val"]->getText().c_str();
	std::transform(tmp.begin(),tmp.end(),tmp.begin(),tolower);
	element->appendChild(doc->createTextNode(Utility::stringToWString(tmp).c_str()));

	//action stuff
	xercesc::DOMElement* actionElement = doc->createElement(L"action");
	root->appendChild(actionElement);

	for(int i = 0; i < 5; ++i)
	{
		xercesc::DOMElement* element = doc->createElement(fullDesc[i+6].c_str());
		actionElement->appendChild(element);
		std::wstring tmp = Utility::stringToWString(actionValues[i]);
		element->appendChild(doc->createTextNode(tmp.c_str()));
	}

	//Movement stuff.
	xercesc::DOMElement* moveElement = doc->createElement(L"movement");
	root->appendChild(moveElement);

	for(int i = 0; i < 6; ++i)
	{
		xercesc::DOMElement* element = doc->createElement(fullDesc[i].c_str());
		moveElement->appendChild(element);
		std::wstring tmp = Utility::stringToWString(movementValues[i]);
		element->appendChild(doc->createTextNode(tmp.c_str()));
	}

	//Volume stuff
	xercesc::DOMElement* volumeElement = doc->createElement(L"volume");
	root->appendChild(volumeElement);

	xercesc::DOMElement* vol = doc->createElement(L"character");
	volumeElement->appendChild(vol);
	vol->appendChild(doc->createTextNode(boost::lexical_cast<std::wstring,int>(characterValue).c_str()));

	vol = doc->createElement(L"music");
	volumeElement->appendChild(vol);
	vol->appendChild(doc->createTextNode(boost::lexical_cast<std::wstring,int>(musicValue).c_str()));

	vol = doc->createElement(L"soundfx");
	volumeElement->appendChild(vol);
	vol->appendChild(doc->createTextNode(boost::lexical_cast<std::wstring,int>(soundValue).c_str()));
	
	//XMLCh* output = serializer->writeToString(doc);

	serializer->writeToURI(doc,L"resource//xml//config.xml");

	doc->release();
	serializer->release();
	xercesc::XMLPlatformUtils::Terminate();
}

//TODO: Decide whether or not to split this up into three different lambda functions.
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

//-------------------------------------------------------------------------------

void MainMenu_FaderCallback::fadeInCallback()
{
	_finished = true;
	_soundManager->setMusicFade(false);
	if(_guiSheet)
	{
		_guiSheet->setAlpha(1.0f);
	}
}

void MainMenu_FaderCallback::fadeOutCallback()
{
	_finished = true;
	_soundManager->setMusicFade(false);
	if(_guiSheet)
	{
		_guiSheet->setAlpha(0.0f);
	}
}

void MainMenu_FaderCallback::setupMusicFade(SoundManager* soundMgr,bool fadeDown)
{
	_finished = false;
	_soundManager = soundMgr;
	_soundManager->setMusicFade(true);
	_fadeMusicDown = fadeDown;
}

void MainMenu_FaderCallback::setupGUIFade(CEGUI::Window* GUISheet,int fadeInTime,int fadeOutTime)
{
	_guiSheet = GUISheet;
	_guiFadeIn = fadeInTime;
	_guiFadeOut = fadeOutTime;
	_guiSheet->setAlpha(0.0f); //to prevent some popping-in during the GUI Fadeout time.
}

void MainMenu_FaderCallback::updateFade(double progress,double currentTime,int fadeOp)
{
	_finished = false;
	float vol = _soundManager->getDefaultMusicVolume();
	_soundManager->setMusicFadeVolume(vol * static_cast<float>( 1.0 - progress ));

	//also update the alpha of the GUI(based on its own timer)
	double totalDuration = currentTime / progress;
	double time = (fadeOp == 1) ? _guiFadeIn : _guiFadeOut;
	if(currentTime < (totalDuration - time))
	{
		double curGui = (totalDuration - time) - currentTime;
		_guiSheet->setAlpha(static_cast<float>(curGui / time));
	}
}