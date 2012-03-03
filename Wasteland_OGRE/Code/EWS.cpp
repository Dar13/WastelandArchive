#include "StdAfx.h"
#include "EWS.h"

template<> EWSManager* Ogre::Singleton<EWSManager>::ms_Singleton = 0;

EWSManager::EWSManager()
{
	//setting up and initializing the pointers and stuff.
	_ewsTexture.setNull();
}

void EWSManager::Setup(Ogre::SceneManager* scene)
{
	_ewsTexture = Ogre::TextureManager::getSingleton().createManual("EWSTexture",
																	"EWS",
																	Ogre::TEX_TYPE_2D,
																	512,512,
																	0,
																	Ogre::PF_BYTE_RGBA,
																	Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	//get pixel buffer
	Ogre::HardwarePixelBufferSharedPtr pixelBuf = _ewsTexture->getBuffer();
	//lock it up
	pixelBuf->lock(Ogre::HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox& pixelBox = pixelBuf->getCurrentLock();

	Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

	//fill in data
	for(size_t i = 0; i < 512; ++i)
	{
		for(size_t j = 0; j < 512; ++j)
		{
			*pDest++ = 0; //Blue
			*pDest++ = 255; //Green
			*pDest++ = 255; //Red
			*pDest++ = 200; //Alpha
		}
	}

	pixelBuf->unlock();

	//create material
	_material = Ogre::MaterialManager::getSingletonPtr()->create("EWSDynTexture",
																 "EWS");
	_material->getTechnique(0)->getPass(0)->createTextureUnitState("dynamicTexture");
	_material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
}

void EWSManager::Update(int health)
{
	return;
}

void EWSManager::Reset()
{
	//reset all materials/textures/etc.
	
	return;
}