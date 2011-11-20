#ifndef _OGRE_MANAGER_H_
#define _OGRE_MANAGER_H_

#include <Ogre.h>

#include <vector>
#include <map>

class OgreManager : public Ogre::Singleton<OgreManager>
{
public:
	OgreManager();
	~OgreManager(){Shutdown();}

	//Generic class state functions
	bool Setup();
	bool Render();
	void Shutdown();

	//Ogre Functions
	//Adds resource locations held in the 
	void addResources(std::vector<Ogre::String> &resourceLocations);

	//Getting the Ogre pointers.
	Ogre::Root* getRoot(){return _Root;}
	Ogre::RenderWindow* getRenderWindow(){return _Window;}
	unsigned long getWindowHandle(){return _windowHandle;}
	Ogre::Log* getLog(){return _Log;}
	Ogre::SceneManager* getSceneManager(){return _Scene;}
	Ogre::Timer* getTimer(){return _Timer;}
	

private:
	//Hidden methods
	OgreManager(const OgreManager&);
	OgreManager& operator=(const OgreManager&);

	//Ogre pointers
	Ogre::Root* _Root;
	Ogre::RenderWindow* _Window;
	unsigned long _windowHandle;
	Ogre::Log* _Log;
	Ogre::SceneManager* _Scene;
	Ogre::Timer* _Timer;
};

#endif