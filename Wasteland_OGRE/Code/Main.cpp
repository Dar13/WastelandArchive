#include "StdAfx.h"

#include <new>

#include "GameManager.h"
#include "StateManager.h"
#include "debug\print.h"

#include <OgreWindowEventUtilities.h>

#if defined(WIN32)
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
#else
int main(int argc, char **argv[])
{
#endif
	/*
	OgreManager* ogre = NULL;
	OISManager* ois = NULL;
	GameManager* game = NULL;
	DebugPrint* realtimeDebugger = NULL;
	StateManager* wtld = NULL;
	*/

	const std::auto_ptr<OgreManager> ogre(new OgreManager());
	//ogre = new OgreManager();
	if(!ogre->Setup())
	{
		//not needed with auto_ptr
		//delete ogre;
		return 1;
	}

	//testing out the new function I made for adding resources.
	std::string resFile = "resource\\xml\\resource_list.xml";
	ogre->addResources(resFile);

	unsigned long hWnd;
	ogre->getRenderWindow()->getCustomAttribute("WINDOW",&hWnd);

	const std::auto_ptr<OISManager> ois(new OISManager(hWnd));
	//Setup the input handler(OIS)
	//ois = new OISManager(hWnd);
	resFile = "resource\\xml\\config.xml";
	ois->setConfiguration(configuration(resFile).release());

	//GameManager class doesn't exist anymore
	//const std::auto_ptr<GameManager> game(new GameManager());
	//Setup the Game manager
	//game = new GameManager();

	//debugger helper
	const std::auto_ptr<DebugPrint> realtimeDebugger(new DebugPrint());
	//realtimeDebugger = new DebugPrint();

	//Set up the state manager
	const std::auto_ptr<StateManager> wtld(new StateManager());
	//wtld = new StateManager();
	wtld->Setup(ois.get(),ogre.get());
	
		//run the app.
	wtld->Run();

	/*
	//aren't needed when using auto_ptr, are deleted automatically.
	//delete state manager
	delete wtld;

	delete realtimeDebugger;

	//delete interfaces for OIS and Ogre3D
	delete ois;
	delete ogre;
	*/
	/*
	catch(std::bad_alloc &ba)
	{
		OutputDebugString(ba.what());
		if(NULL != ogre)
		{
			delete ogre;
		}
		if(NULL != ois)
		{
			delete ois;
		}
		if(NULL != game)
		{
			delete game;
		}
		if(NULL != realtimeDebugger)
		{
			delete realtimeDebugger;
		}
		if(NULL != wtld)
		{
			delete wtld;
		}
	}
	*/
	

	return 0;
}