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
	bool addResources(std::vector<Ogre::String> &location,std::vector<Ogre::String> &type,std::vector<Ogre::String> &name);

	//Getting the Ogre pointers.
	Ogre::Root* getRoot(){return _Root;}
	Ogre::RenderWindow* getRenderWindow(){return _Window;}
	unsigned long getWindowHandle(){return _windowHandle;}
	Ogre::Log* getLog(){return _Log;}
	Ogre::Timer* getTimer(){return _Timer;}
	
	//Utility functions, specific to Ogre.
	void getMeshInformation(const Ogre::MeshPtr* const meshptr,
							size_t &vertex_count,
							Ogre::Vector3* &vertices,
							size_t &index_count,
							unsigned long* &indices,
							const Ogre::Vector3 &position = Ogre::Vector3::ZERO,
							const Ogre::Quaternion &orient = Ogre::Quaternion::IDENTITY,
							const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE);
	

private:
	//Hidden methods
	OgreManager(const OgreManager&);
	OgreManager& operator=(const OgreManager&);

	//Ogre pointers
	Ogre::Root* _Root;
	Ogre::RenderWindow* _Window;
	unsigned long _windowHandle;
	Ogre::Log* _Log;
	Ogre::Timer* _Timer;
};

#endif