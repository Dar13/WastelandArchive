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
	Fill(Ogre::ColourValue(0.5f,1.0f,1.0f,1.0f));
	
	//Line(Ogre::Vector2(0,256),Ogre::Vector2(512,1),Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));

	//create material
	if(_material.isNull())
	{
		_material = Ogre::MaterialManager::getSingletonPtr()->create("EWSDynTexture",
																     "EWS");
		_material->getTechnique(0)->getPass(0)->createTextureUnitState("EWSTexture");
		_material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	}

	//setting up the scene nodes and entities.
	_ewsNode = scene->getRootSceneNode()->createChildSceneNode("EWSNode");
	_ewsEntity = scene->createEntity("EWSEntity","ewsPlain.mesh");
	_ewsNode->attachObject(_ewsEntity);
	_ewsNode->setPosition(0.0f,2.0f,0.0f);
	_ewsEntity->setMaterial(_material);

	Ogre::Rect testRect;
	testRect.left = 10;
	testRect.right = 200;
	testRect.top = 10;
	testRect.bottom = 200;
	Box(testRect,Ogre::ColourValue(.5f,1.0f,0.0f,1.0f));
}

void EWSManager::Update(int health)
{
	Ogre::Rect r;
	r.bottom = 150;
	r.top = r.bottom - health;
	r.left = 50;
	r.right = 150;
	Box(r,Ogre::ColourValue(1.0f,0.1f,0.1f,1.0f));
	return;
}

void EWSManager::Reset()
{
	//reset the textures/etc.
	if(_ewsNode->getCreator()->hasEntity(_ewsEntity->getName()))
	{
		_ewsNode->getCreator()->destroyEntity(_ewsEntity);
	}
	if(_ewsNode->isInSceneGraph())
	{
		_ewsNode->getParentSceneNode()->removeAndDestroyChild(_ewsNode->getName());
	}

	_ewsEntity = NULL;
	_ewsNode = NULL;
	
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

void EWSManager::Box(Ogre::Rect &rect, Ogre::ColourValue& color)
{
	_pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox& pixBox = _pixelBuffer->getCurrentLock();

	Ogre::uint8* pData = static_cast<Ogre::uint8*>(pixBox.data);
	for(unsigned int ix = rect.left; ix <= rect.right; ++ix)
	{
		for(unsigned int iy = rect.top; iy <= rect.bottom; ++iy)
		{
			unsigned int offset = getOffset(ix,iy,pixBox.rowPitch);
			SetPixel(pData + offset,color);
		}
	}
	

	_pixelBuffer->unlock();
}

void EWSManager::Line(Ogre::Vector2 start, Ogre::Vector2 end, Ogre::ColourValue& color)
{
	_pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox& pixbox = _pixelBuffer->getCurrentLock();

	Ogre::uint8* pData = static_cast<Ogre::uint8*>(pixbox.data);
	//unsigned int startOffset = (start.x * 4) + (start.y * 4 * pixbox.rowPitch);
	//unsigned int endOffset = (end.x * 4) + (end.y * 4 * pixbox.rowPitch);

	for(unsigned int ix = start.x; ix <= end.x; ++ix)
	{
		unsigned int offset = getOffset(ix,iy,pixbox.rowPitch);
		SetPixel(pData + offset,color);
	}

	_pixelBuffer->unlock();

	return;
}

void EWSManager::SetPixel(Ogre::uint8* data,Ogre::ColourValue& color)
{
	//assumes data pointer is set to the correct setting.
	*data++ = (Ogre::uint8)color.r*255;
	*data++ = (Ogre::uint8)color.g*255;
	*data++ = (Ogre::uint8)color.b*255;
	*data++ = (Ogre::uint8)color.a*255;
	 
	return;
}

int EWSManager::getOffset(int x,int y, int rowPitch)
{
	return (x * 4) + ( y * 4 * rowPitch );
}