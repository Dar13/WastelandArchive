#include "StdAfx.h"

#include "Utility.h"

#ifndef _ENVWARNSYS_H_
#define _ENVWARNSYS_H_

//Forward Declaration
struct OgreTransform;

/*! \brief This class manages the entirety of the Environmental Warning System, an innovative look at Heads-Up Displays

Creates and updates the necessary scene nodes, textures, and information required for the Environmental Warning System.
*/
class EWSManager
{
public:
	EWSManager(Ogre::SceneManager* scene);
	~EWSManager();

	//! Sets up the EWS manager
	/*!
		\param scene The current SceneManager in use by the current application state.
	*/
	void Setup(Ogre::SceneManager* scene);

	//Passing in the current information needed to be represented.
	//Would be called by the appState(maybe) or the GameManager(probably).
	//! Tells the EWS manager to update the system.
	/*!
		\param newTime Gives the elapsed time to the EWS manager, to control the update rate.
		\param isPlacing Tells the EWS whether or not it should place the system.
		\param playerData Gives health and ammo information to the EWS manager
		\param playerTransform Supplies the player's position and orientation to the EW system.
	*/
	void Update(int newTime,bool isPlacing,const sPlayerData& playerData,const OgreTransform& playerTransform);//, void* ammoInfo);

	//! Places the EWS system at the specified position and normal.
	/*!
		\param rayCastPosition End of raycast done by the physics engine.
		\param rayCastNormal Normal of the raycast done by the physics engine.
		\param playerTransform Player's position and orientation
	*/
	void Place(const Ogre::Vector3& rayCastPosition,const Ogre::Vector3& rayCastNormal,const OgreTransform& playerTransform);

protected:
	void Fill(Ogre::ColourValue& color);
	void Box(Ogre::Rect rect, Ogre::ColourValue& color);
	void Line(Ogre::Vector2 start, Ogre::Vector2 end, Ogre::ColourValue& color);
	void Circle(Ogre::Vector2 center,int radius, Ogre::ColourValue& color);
	
	void SetPixel(Ogre::uint8* data, Ogre::ColourValue& color);
	int getOffset(int x, int y, int rowPitch);

private:
	Ogre::TexturePtr _ewsTexture;
	Ogre::MaterialPtr _material;
	Ogre::HardwarePixelBufferSharedPtr _pixelBuffer;
	Ogre::SceneNode* _ewsNode;
	Ogre::Entity* _ewsEntity;

	Ogre::BillboardSet* _ewsBillboardSet;
	Ogre::Billboard* _ewsBillboard;

	bool _placed;
	int placeToggle;
	bool _pointToPlayer;

	int oldTime;
	
	//Struct to hold player data
	sPlayerData _playerData;
};

#endif