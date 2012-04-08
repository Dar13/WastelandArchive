#include "StdAfx.h"



#ifndef _ENVWARNSYS_H_
#define _ENVWARNSYS_H_

//Forward Declaration
struct OgreTransform;

class EWSManager
{
public:
	EWSManager(Ogre::SceneManager* scene);
	~EWSManager();

	void Setup(Ogre::SceneManager* scene);
	//Passing in the current information needed to be represented.
	//Would be called by the appState(maybe) or the GameManager(probably).
	void Update(int health,int newTime,bool isPlacing,const OgreTransform& playerTransform);//, void* ammoInfo);

	void Place(const Ogre::Vector3& rayCastPosition,const Ogre::Vector3& rayCastNormal,const OgreTransform& playerTransform);

protected:
	void Fill(Ogre::ColourValue& color);
	void Box(Ogre::Rect& rect, Ogre::ColourValue& color);
	void Line(Ogre::Vector2 start, Ogre::Vector2 end, Ogre::ColourValue& color);
	void Circle(Ogre::Vector2 center,int radius, Ogre::ColourValue& color);
	
	void SetPixel(Ogre::uint8* data, Ogre::ColourValue& color);
	int getOffset(int x, int y, int rowPitch);

private:
	Ogre::TexturePtr _ewsTexture;
	Ogre::MaterialPtr _material;
	Ogre::HardwarePixelBufferSharedPtr _pixelBuffer;
	//Ogre::PixelBox& _pixelBox;
	Ogre::SceneNode* _ewsNode;
	Ogre::Entity* _ewsEntity;

	bool _placed;
	int placeToggle;
	bool _pointToPlayer;

	int oldTime;
	
	//eventually make a struct to hold all this data
	int _health;
};

#endif