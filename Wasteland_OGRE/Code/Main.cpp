#include "GameManager.h"
#include "StateManager.h"

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

	//testing out the new function I made for adding resources.
	std::vector<Ogre::String> loc,typ,name;
	loc.push_back("resource\\models");
	typ.push_back("FileSystem");
	name.push_back("Models");
	loc.push_back("resources");
	typ.push_back("FileSystem");
	name.push_back("General");

	ogre->addResources(loc,typ,name);

	unsigned long hWnd;
	ogre->getRenderWindow()->getCustomAttribute("WINDOW",&hWnd);

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
	delete ogre;

	return 0;
}