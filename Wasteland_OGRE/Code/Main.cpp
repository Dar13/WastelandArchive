#include "StdAfx.h"

#include <new>

#include "GameManager.h"
#include "StateManager.h"
#include "debug\print.h"
#include "debug\console.h"

#include <OgreWindowEventUtilities.h>

#if defined(WIN32)
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
#else
int main(int argc, char **argv[])
{
#endif

	const std::unique_ptr<OgreManager> ogre(new OgreManager());
	if(!ogre->Setup())
	{
		return 1;
	}

	//testing out the new function I made for adding resources.
	std::string resFile = "resource\\xml\\resource_list.xml";
	ogre->addResources(resFile);

	unsigned long hWnd;
	ogre->getRenderWindow()->getCustomAttribute("WINDOW",&hWnd);

	//Setup the input handler(OIS)
	const std::unique_ptr<OISManager> ois(new OISManager(hWnd));
	resFile = "resource\\xml\\config.xml";
	ois->setConfiguration(configuration(resFile).release());

	//true console debugger
	const std::unique_ptr<VirtualConsole> console(new VirtualConsole("Debugger",0));

	//Set up the state manager
	const std::unique_ptr<StateManager> wtld(new StateManager());
	wtld->Setup(ois.get(),ogre.get());
	
	//run the app.
	wtld->Run();

	return 0;
}