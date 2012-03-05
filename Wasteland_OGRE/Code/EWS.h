#include "StdAfx.h"

#ifndef _ENVWARNSYS_H_
#define _ENVWARNSYS_H_

class EWSManager : public Ogre::Singleton<EWSManager>
{
public:
	EWSManager();

	void Setup(Ogre::SceneManager* scene);
	//Passing in the current information needed to be represented.
	//Would be called by the appState(maybe) or the GameManager(probably).
	void Update(int health);//, void* ammoInfo);

	//Have to wait for the class to be destroyed for it to be totally cleaned.
	void Reset();

protected:
	void Fill(Ogre::ColourValue& color);
	void Box(Ogre::Rect& rect, Ogre::ColourValue& color);
	void Line(Ogre::Vector2 start, Ogre::Vector2 end, Ogre::ColourValue& color);
	void SetPixel(Ogre::uint8* data, Ogre::ColourValue& color);
	int getOffset(int x, int y, int rowPitch);

private:
	Ogre::TexturePtr _ewsTexture;
	Ogre::MaterialPtr _material;
	Ogre::HardwarePixelBufferSharedPtr _pixelBuffer;
	//Ogre::PixelBox& _pixelBox;
	Ogre::SceneNode* _ewsNode;
	Ogre::Entity* _ewsEntity;

	//Ogre::Singleton stuff, have to make this inaccessible.
	EWSManager(const EWSManager&);
	EWSManager& operator=(EWSManager&);
};

#endif