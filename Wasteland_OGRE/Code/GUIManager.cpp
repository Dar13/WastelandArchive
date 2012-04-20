#include "StdAfx.h"

#include "GUIManager.h"


GUIManager::GUIManager(const std::string& cwd,Ogre::RenderTarget& window)
{
	_guiRenderer = 0;
	_guiSystem = 0;
	_guiWinManager = 0;
	_currentGUISheet = 0;

	Setup(cwd,window);
}

void GUIManager::Setup(const std::string& cwd,Ogre::RenderTarget& window)
{
	//since I'm drawing to the renderwindow anyways, don't need to provide a special rendertarget
	_guiRenderer = &CEGUI::OgreRenderer::bootstrapSystem(window);
	_guiSystem = CEGUI::System::getSingletonPtr();

	//setting the default resource group from ogre.
	CEGUI::Imageset::setDefaultResourceGroup("CEGUI");
	CEGUI::Font::setDefaultResourceGroup("CEGUI");
	CEGUI::Scheme::setDefaultResourceGroup("CEGUI");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("CEGUI");
	CEGUI::WindowManager::setDefaultResourceGroup("CEGUI");
	CEGUI::ScriptModule::setDefaultResourceGroup("CEGUI");
	CEGUI::XMLParser* parser = _guiSystem->getXMLParser();
	if(parser->isPropertyPresent("SchemaDefaultResourceGroup"))
		parser->setProperty("SchemaDefaultResourceGroup","CEGUI");

	//load look'n'feel here
	CEGUI::SchemeManager::getSingletonPtr()->create("SleekSpace/SleekSpace.scheme","CEGUI");

	_guiWinManager = CEGUI::WindowManager::getSingletonPtr();
}

bool GUIManager::Update(float timeElapsed_ms)
{
	_guiSystem->injectTimePulse(timeElapsed_ms);

	return true;
}

void GUIManager::setCurrentGUISheet(const std::string& sheetName)
{
	bool found = true;
	//check for duplicate calls.
	if(_currentGUISheet != NULL)
	{
		if(_currentGUISheet->getName() == sheetName)
			return;
	}
	if(sheetName == "none")
	{
		_currentGUISheet = NULL;
		_guiSystem->setGUISheet(0);
	}
	//else we're going to have to iterate through each element in the vector
	for(std::vector<CEGUI::Window*>::iterator itr = _guiSheets.begin(); itr != _guiSheets.end(); ++itr)
	{
		if( (*itr)->getName() == sheetName )
		{
			_currentGUISheet = (*itr);
			_guiSystem->setGUISheet(_currentGUISheet);
			found = true;
		}
	}
	if(!found)
	{
		_currentGUISheet = 0;
	}
	//returns despite whether or not the guisheet has been changed.
}

CEGUI::System* GUIManager::getSystem()
{
	return _guiSystem;
}

CEGUI::WindowManager* GUIManager::getWinManager()
{
	return _guiWinManager;
}

void GUIManager::addGUISheet(CEGUI::Window* guiSheet)
{
	//easy-peasy.
	_guiSheets.push_back(guiSheet);
}

void GUIManager::removeGUISheet(CEGUI::Window* guiSheet)
{
	std::vector<CEGUI::Window*>::iterator loc;
	//this way I don't need to implement a for-next loop.
	loc = std::find(_guiSheets.begin(),_guiSheets.end(),guiSheet);
	//std::find returns second argument if it fails. in this case, _guiSheets.end()
	if(loc != _guiSheets.end())
	{
		//requires an iterator describing a location for some reason(even though it invalidates the iterator).
		_guiSheets.erase(loc);
	}
}

GUIManager::~GUIManager()
{
	//this way I don't have to worry about cleaning it up manually.
	_guiSystem->destroy();
}