#include "StdAfx.h"

#include "GameManager.h"
#include "StateManager.h"

#include <OgreWindowEventUtilities.h>

#include "XMLReader.h"

#if defined(WIN32)
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
#else
int main(int argc, char **argv[])
{
#endif
	OgreManager* ogre = new OgreManager();
	if(!ogre->Setup())
	{
		delete ogre;
		return 1;
	}

	//testing out the new function I made for adding resources.
	std::string resFile = "resource\\xml\\resource_list.xml";

	ogre->addResources(resFile);

	unsigned long hWnd;
	ogre->getRenderWindow()->getCustomAttribute("WINDOW",&hWnd);

	//Setup the XMLReader singleton
	XMLReader* xml = new XMLReader();

	//Setup the input handler(OIS)
	OISManager* ois = new OISManager(hWnd);

	//Setup Bullet manager
	BulletManager* bullet = new BulletManager();
	bullet->Setup();

	//Setup the Game manager
	GameManager* game = new GameManager();

	//Set up the state manager
	StateManager* wtld = new StateManager();
	wtld->Setup();
	
	//run the app.
	wtld->Run();

	//delete state manager
	delete wtld;

	//delete interfaces for OIS and Ogre3D
	delete ois;
	delete xml;
	delete ogre;
	

	return 0;
}