#include "OgreManager.h"
#include "OISManager.h"

//CEGUI includes
#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>

//Ogre includes
#include <Ogre.h>
#include <OgreWindowEventUtilities.h>

#if defined(WIN32)
#include <Windows.h>

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

	unsigned long hWnd;
	ogre->getRenderWindow()->getCustomAttribute("WINDOW",&hWnd);

	OISManager* ois = new OISManager(hWnd);

	//Main loop
	bool noErrors=true;
	while(!ois->escapePressed() && noErrors)
	{
		ois->capture();

		noErrors=ogre->Render();
	}

	delete ois;
	delete ogre;

	return 0;
}