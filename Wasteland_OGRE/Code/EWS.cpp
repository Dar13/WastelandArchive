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
	//check for if the texture is made.
	if(_ewsTexture.isNull())
	{
		_ewsTexture = Ogre::TextureManager::getSingleton().createManual("EWSTexture",
																		"EWS",
																		Ogre::TEX_TYPE_2D,
																		512,512,
																		0,
																		Ogre::PF_BYTE_RGBA,
																		Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	}
	else
	{
		Fill(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));
	}
	//get pixel buffer
	_pixelBuffer = _ewsTexture->getBuffer();

	//fill in data
	Fill(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));

	//create material
	if(_material.isNull())
	{
		_material = Ogre::MaterialManager::getSingletonPtr()->create("EWSDynTexture",
																 "EWS");
		_material->getTechnique(0)->getPass(0)->createTextureUnitState("dynamicTexture");
		_material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	}

	//setting up the scene nodes and entities.
	_ewsNode = scene->getRootSceneNode()->createChildSceneNode("EWSNode");
	_ewsEntity = scene->createEntity("EWSEntity","ewsPlain.mesh");
	_ewsNode->attachObject(_ewsEntity);
	_ewsEntity->setMaterial(_material);

}

void EWSManager::Update(int health)
{
	return;
}

void EWSManager::Reset()
{
	//reset the textures/etc.
	_ewsNode->getCreator()->destroyEntity(_ewsEntity);
	_ewsNode->getParentSceneNode()->removeAndDestroyChild(_ewsNode->getName());
	
	return;
}

void EWSManager::Fill(Ogre::ColourValue& color)
{
	_pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox& pixBox = _pixelBuffer->getCurrentLock();

	int red = (int)(color.r * 255);
	int green = (int)(color.g * 255);
	int blue = (int)(color.b * 255);
	int alpha = (int)(color.a * 255);

	Ogre::uint8* pData = static_cast<Ogre::uint8*>(pixBox.data);
	for(unsigned int i = 0; i < _ewsTexture->getSrcWidth(); ++i)
	{
		for(unsigned int j = 0; j < _ewsTexture->getSrcHeight(); ++j)
		{
			*pData++ = red;
			*pData++ = green;
			*pData++ = blue;
			*pData++ = alpha;
		}
	}

	_pixelBuffer->unlock();
}