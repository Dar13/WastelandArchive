#include "StdAfx.h"

//#include <vld.h>

#include <new>

#include <direct.h>

#include "GameManager.h"
#include "StateManager.h"
#include "debug\print.h"
#include "debug\console.h"
#include "LuaManager.h"

#include <OgreWindowEventUtilities.h>

#if defined(WIN32)
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
#else
int main(int argc, char **argv[])
{
#endif

	srand(static_cast<int>(time(0)));
	srand(static_cast<int>((rand() % RAND_MAX) * time(0)));

	//Smart pointer holding ogre manager pointer.
	const std::unique_ptr<GraphicsManager> ogre(new GraphicsManager());
	configuration_t* config = configuration("resource\\xml\\config.xml").release();
	//Setup ogre. If this fails, crash to desktop. Probably should pop-up a message box.
	if(!ogre->Setup(config))
	{
#if defined(WIN32)
		MessageBoxA(NULL,"The Object-Oriented Graphics Rendering Engine(OGRE) has failed to initialize properly.","ERROR",MB_OK);
#else
		//not sure what to put here.
		//Linux is much harder to have a message box generically like Windows.
#endif
		return 1; //error code 1
	}

	srand(ogre->getTimer()->getMilliseconds());

	//adds all the resource groups defined by the files contained in this file.
	std::string resFile = "resource\\xml\\lists\\resource_list.xml";
	ogre->addResources(resFile);

	//gets the window handle from ogre.
	unsigned long hWnd;
	HWND realhWnd;
	ogre->getRenderWindow()->getCustomAttribute("WINDOW",&hWnd);
	ogre->getRenderWindow()->getCustomAttribute("WINDOW",&realhWnd);

	//Setup the input handler(OIS)
	const std::unique_ptr<InputManager> ois(new InputManager(hWnd));
	ois->setConfiguration(config);

	const std::unique_ptr<SoundManager> sound(new SoundManager());
	if(!sound->Setup())
	{
#if defined(WIN32)
		MessageBoxA(NULL,"FMOD didn't initialize properly.","ERROR",MB_OK);
#else
		//other platforms?
#endif
		return 2; //error code 2
	}

	char buf[MAX_PATH];
	_getcwd(buf,MAX_PATH);
	resFile.assign(buf);
	const std::unique_ptr<GUIManager> gui(new GUIManager(resFile,*ogre->getRenderWindow()));

	//true console debugger
	std::unique_ptr<Console> console(new Console("Debug Console"));
	console->positionConsole(10,2000);
	SetFocus(realhWnd);
	SwitchToThisWindow(realhWnd,TRUE);

	//setup lua manager
	const std::unique_ptr<LuaManager> lua(new LuaManager());
	lua->Setup("resource\\xml\\lists\\lua_list.xml");

	//Set up the state manager
	const std::unique_ptr<StateManager> wtld(new StateManager());
	wtld->Setup(ois.get(),ogre.get(),gui.get(),sound.get());
	
	//run the app.
	wtld->Run();

	return 0;
}