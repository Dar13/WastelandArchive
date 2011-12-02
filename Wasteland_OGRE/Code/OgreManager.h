#ifndef _OGRE_MANAGER_H_
#define _OGRE_MANAGER_H_

#include <Ogre.h>

#include <vector>
#include <map>

/*! \brief This class manages the entirety of Ogre3D(OOP Graphics Rendering Engine).

Exposes the various Ogre classes(Root,Time,RenderWindow,etc.) to the application states, and performs Ogre-specific tasks
such as get mesh information or making a scene node.
*/

class OgreManager : public Ogre::Singleton<OgreManager>
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
		\param location A string vector holding all locations of the resource locations.
		\param type A string vector holding all types of resource locations(FileSystem, Zip, etc.)
		\param name A string vector holding all the names of the resource groups formed by the location/type combinations.
	*/
	bool addResources(std::vector<Ogre::String> &location,std::vector<Ogre::String> &type,std::vector<Ogre::String> &name);

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
		\param options std::map<string,string> that will hold all applicable data concerning the construction of the Entity/SceneNode.
		<ul>
			<li>"name" Name for both the Entity and the SceneNode.</li>
			<li>"filename" FileName for the Entity to load from(if needed for the type of Entity/SceneNode).</li>
			<li>"type" The type of entity to be created.</li>
			<li>"resgroup" Resource Group that holds the FileName.</li>
		</ul>
		\param position The initial position of the SceneNode. Defaults to (0,0,0).
		\param rotation The initial rotation of the SceneNode. Defaults to Ogre::Quaternion::ZERO.
	*/
	Ogre::SceneNode* createSceneNode(Ogre::SceneManager* scene, std::map<std::string,std::string> &options,Ogre::Vector3 &position = Ogre::Vector3::ZERO,Ogre::Quaternion &rotation = Ogre::Quaternion::ZERO);
	
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
	

private:
	//Hidden methods
	OgreManager(const OgreManager&);
	OgreManager& operator=(const OgreManager&);

	//Private helper functions
	//! Calculates the attenuation for the passed-in Ogre::Light.
	void setLightRange(Ogre::Light* l, Ogre::Real range);
	//! Converts a hexadecimal number into an Ogre::ColourValue.
	Ogre::ColourValue getColorFromHex(int hexColor, float alpha = 1.0f);

	//Ogre pointers
	Ogre::Root* _Root;
	Ogre::RenderWindow* _Window;
	unsigned long _windowHandle;
	Ogre::Log* _Log;
	Ogre::Timer* _Timer;
};

#endif