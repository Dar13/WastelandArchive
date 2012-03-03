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

private:
	Ogre::TexturePtr _ewsTexture;
	Ogre::MaterialPtr _material;
	Ogre::HardwarePixelBufferSharedPtr _pixelBuffer;
	Ogre::PixelBox _pixelBox;

	//Ogre::Singleton stuff, have to make this inaccessible.
	EWSManager(const EWSManager&);
	EWSManager& operator=(EWSManager&);
};

#endif