#ifndef _OGRE_MANAGER_H_
#define _OGRE_MANAGER_H_

#include <Ogre.h>

#include "XMLReader.h"

#include <vector>
#include <map>

/*! \brief This class manages the entirety of Ogre3D(OOP Graphics Rendering Engine).

Exposes the various Ogre classes(Root,Time,RenderWindow,etc.) to the application states, and performs Ogre-specific tasks
such as get mesh information or making a scene node.
*/

class OgreManager : public Ogre::Singleton<OgreManager>,Ogre::FrameListener
{
public:
	//! Initializes all pointers to NULL.
	OgreManager();
	//! Calls the Shutdown method, guarantees clean shutdown of Ogre.
	~OgreManager(){Shutdown();}

	//! Sets up the class to handle Ogre things.
	bool Setup();
	//! Updates Ogre, while rendering one frame.
	bool Render();
	//! Shutsdown Ogre as cleanly as possible.
	void Shutdown();

	//! Adds resource locations stored in vectors to Ogre.
	//! Means that an external file can hold all resource locations.
	/*!
		\param filename Is a path to the relevant xml file that holds all the resource locations.
	*/
	bool addResources(std::string& filename);

	//! Gives access to the Ogre::Root pointer.
	Ogre::Root* getRoot(){return _Root;}
	//! Returns the current Ogre::RenderWindow*.
	Ogre::RenderWindow* getRenderWindow(){return _Window;}
	//! Returns the current window handle (hWnd).
	unsigned long getWindowHandle(){return _windowHandle;}
	//! Returns the current Logger for Ogre.
	Ogre::Log* getLog(){return _Log;}
	//! Returns the current Timer.
	Ogre::Timer* getTimer(){return _Timer;}

	//! Creates an Ogre SceneNode and Entity based on passed in parameters
	//! /returns Ogre::SceneNode*
	/*!
		\param scene The SceneManager that will create the SceneNode.
		\param objectInfo Class that holds all loaded information from the applicable xml file.
	*/
	Ogre::SceneNode* createSceneNode(Ogre::SceneManager* scene, object_t* objectInfo);
	
	//! Retrieves all vertex and index data from a mesh.
	//! Useful for generating complex Bullet rigid bodies.
	/*!
		\param meshptr Contains MeshPtr, wrapper to the real Mesh pointer.
		\param vertex_count Will be filled with the count of vertices in the mesh.
		\param vertices Will be filled with the individual vertices of the mesh.
		\param index_count Will be filled with count of all indices in the mesh.
		\param indices Will be filled with the order of the vertices that form the triangles of the mesh. The first 3 indices correspond to the three vertices in the returned vertices array to make the first triangle in the mesh.
		\param position **IDK**
		\param orient **IDK**
		\param scale **IDK**
	*/
	void getMeshInformation(const Ogre::MeshPtr* const meshptr,
							size_t &vertex_count,
							Ogre::Vector3* &vertices,
							size_t &index_count,
							unsigned long* &indices,
							const Ogre::Vector3 &position = Ogre::Vector3::ZERO,
							const Ogre::Quaternion &orient = Ogre::Quaternion::IDENTITY,
							const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE);
	
	void addAnimationState(Ogre::AnimationState* anim);
	void clearAnimationStates();

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

private:
	//Hidden methods
	OgreManager(const OgreManager&);
	OgreManager& operator=(const OgreManager&);

	//Private helper functions
	//! Calculates the attenuation for the passed-in Ogre::Light.
	void setLightRange(Ogre::Light* l, Ogre::Real range);
	//! Converts a hexadecimal number into an Ogre::ColourValue.
	Ogre::ColourValue getColorFromHex(int hexColor, float alpha = 1.0f);

	std::vector<Ogre::AnimationState*> _animations;

	//Ogre pointers
	Ogre::Root* _Root;
	Ogre::RenderWindow* _Window;
	unsigned long _windowHandle;
	Ogre::Log* _Log;
	Ogre::Timer* _Timer;
};

#endif