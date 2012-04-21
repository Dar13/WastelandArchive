#include "StdAfx.h"

#ifndef _GUIMANAGER_H_
#define _GUIMANAGER_H_

class GUIManager
{
public:
	GUIManager(const std::string& cwd,Ogre::RenderTarget& window);
	~GUIManager();
	void Setup(const std::string& cwd,Ogre::RenderTarget& window);

	bool Update(float timeElapsed_ms);

	void Shutdown();

	void setLookNFeel(const std::string& schemeFileName);

	bool doesGUISheetExist(const std::string& sheetName);
	void setCurrentGUISheet(const std::string& sheetName);
	void addGUISheet(CEGUI::Window* guiSheet);
	void removeGUISheet(CEGUI::Window* guiSheet);

	CEGUI::System* getSystem();
	CEGUI::WindowManager* getWinManager();
private:
	CEGUI::System* _guiSystem;
	CEGUI::OgreRenderer* _guiRenderer;
	CEGUI::WindowManager* _guiWinManager;

	//AKA root windows.
	std::vector<CEGUI::Window*> _guiSheets;
	CEGUI::Window* _currentGUISheet;

};

#endif