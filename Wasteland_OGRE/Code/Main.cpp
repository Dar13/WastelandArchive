#include "StdAfx.h"

#include <new>

#include <direct.h>

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

	//Smart pointer holding ogre manager pointer.
	const std::unique_ptr<OgreManager> ogre(new OgreManager());
	//Setup ogre. If this fails, crash to desktop. Probably should pop-up a message box.
	if(!ogre->Setup())
	{
#if defined(WIN32)
		MessageBoxA(NULL,"The Object-Oriented Graphics Rendering Engine(OGRE) has failed to initialize properly.","ERROR",MB_OK);
#else
		//not sure what to put here.
#endif
		return 1;
	}

	//adds all the resource groups defined by the files contained in this file.
	std::string resFile = "resource\\xml\\resource_list.xml";
	ogre->addResources(resFile);

	//gets the window handle from ogre.
	unsigned long hWnd;
	ogre->getRenderWindow()->getCustomAttribute("WINDOW",&hWnd);

	//Setup the input handler(OIS)
	const std::unique_ptr<OISManager> ois(new OISManager(hWnd));
	resFile = "resource\\xml\\config.xml"; //re-using string variable.
	ois->setConfiguration(configuration(resFile).release());

	char buf[MAX_PATH];
	_getcwd(buf,MAX_PATH);
	resFile.assign(buf);
	const std::unique_ptr<GUIManager> gui(new GUIManager(resFile,*ogre->getRenderWindow()));

	//true console debugger
	const std::unique_ptr<VirtualConsole> console(new VirtualConsole("Debugger",0));

	//Set up the state manager
	const std::unique_ptr<StateManager> wtld(new StateManager());
	wtld->Setup(ois.get(),ogre.get(),gui.get());
	
	//run the app.
	wtld->Run();

	return 0;
}