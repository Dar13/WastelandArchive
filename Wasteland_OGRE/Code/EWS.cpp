#include "StdAfx.h"
#include "EWS.h"

#include "debug\print.h"

EWSManager::EWSManager(Ogre::SceneManager* scene)
{
	//setting up and initializing the pointers and stuff.
	_ewsTexture.setNull();
	Setup(scene);
}

EWSManager::~EWSManager()
{
	//fill overlay texture with transparent black
	if(_ewsTexture.isNull()==false)
	{
		Fill(Ogre::ColourValue(0,0,0,0));
	}
}

void EWSManager::Setup(Ogre::SceneManager* scene)
{
	//check for if the texture is made.
	Ogre::ResourcePtr test;
	test = Ogre::MaterialManager::getSingleton().getByName("ews_base");
	if(!test.isNull())
	{
		_material = test;
	}
	_ewsTexture = _material->getTechnique(0)->getPass(0)->getTextureUnitState(2)->_getTexturePtr();
	
	//get pixel buffer
	_pixelBuffer = _ewsTexture->getBuffer();

	//setting up the scene nodes and entities.
	_ewsNode = scene->getRootSceneNode()->createChildSceneNode("EWSNode");
	_ewsEntity = scene->createEntity("EWSEntity","ewsPlain.mesh");
	_ewsNode->attachObject(_ewsEntity);
	_ewsNode->setPosition(0.0f,2.0f,0.0f);
	_ewsEntity->setMaterial(_material);

	_placed = true;
	oldTime = 0;

}

void EWSManager::Update(int health,int newTime,bool isPlacing)
{
	currentTime = newTime;
	if(!isPlacing)
	{
		if(_placed)
		{
			placeToggle = 2;
		}
		if(!_placed)
		{
			placeToggle = 0;
		}
	}

	if(!_placed)
	{
		return;
	}
	//assumes timeElapsed is in ms
	if((newTime - oldTime) > 500 && health != _health ) //playerInfo != _playerInfo)
	{
		//draw health information
		Box(Ogre::Rect(250,health-100,300,250),Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));
		//Circle(Ogre::Vector2(256,200),100,Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));

		//draw ammo information
		//NOT DONE YET!
		oldTime = newTime;
		
	}
	_material->setDepthWriteEnabled(false);
	_material->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	_material->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));
	
	return;
}

void EWSManager::Place(Ogre::Vector3& rayCastPosition,Ogre::Vector3& rayCastNormal)
{
	if(_placed && placeToggle == 2 )
	{
		//??
		_placed = false;
		_ewsNode->setVisible(false);
		placeToggle = 1;
	}
	if(!_placed)
	{
		if(placeToggle == 0)
		{
			_ewsNode->setPosition(rayCastPosition + rayCastNormal);
			_ewsNode->lookAt(rayCastPosition + (rayCastNormal),Ogre::SceneNode::TS_WORLD);
			_placed = true;
			_ewsNode->setVisible(true);
			placeToggle = 1;
		}
	}
}

void EWSManager::Fill(Ogre::ColourValue& color)
{
	_pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox& pixBox = _pixelBuffer->getCurrentLock();

	int red = static_cast<int>(color.r * 255);
	int green = static_cast<int>(color.g * 255);
	int blue = static_cast<int>(color.b * 255);
	int alpha = static_cast<int>(color.a * 255);

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
	for(int ix = rect.left; ix <= rect.right; ++ix)
	{
		for(int iy = rect.top; iy <= rect.bottom; ++iy)
		{
			if((ix >= 0 && ix < 512) && (iy >= 0 && iy < 512))
			{
				unsigned int offset = getOffset(ix,iy,pixBox.rowPitch);
				SetPixel(pData + offset,color);
			}
		}
	}
	

	_pixelBuffer->unlock();
}

void EWSManager::Line(Ogre::Vector2 start, Ogre::Vector2 end, Ogre::ColourValue& color)
{
	_pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox& pixbox = _pixelBuffer->getCurrentLock();

	Ogre::uint8* pData = static_cast<Ogre::uint8*>(pixbox.data);

	for(int ix = (int)start.x; ix <= (int)end.x; ++ix)
	{
		unsigned int offset = getOffset(ix,(int)start.x,pixbox.rowPitch);
		SetPixel(pData + offset,color);
	}

	_pixelBuffer->unlock();

	return;
}

//VERY LOW PERFORMANCE!! Needs to be optimized before used in EWS system.
void EWSManager::Circle(Ogre::Vector2 center,int radius, Ogre::ColourValue& color)
{
	int w,h;
	w = _ewsTexture->getSrcWidth();
	h = _ewsTexture->getSrcHeight();
	_pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox& pixBox = _pixelBuffer->getCurrentLock();

	Ogre::uint8* pData = static_cast<Ogre::uint8*>(pixBox.data);

	int dots = static_cast<int>(radius * (Ogre::Math::TWO_PI*4));
	float deg2dot = 360 / (dots * 1.0f);

	for(int f = 0; f <= radius; ++f)
	{
		for(int d = 0; d <= dots; ++d)
		{
			int u = static_cast<int>(sin(d * deg2dot) * (radius - f));
			int v = static_cast<int>(cos(d * deg2dot) * (radius - f));
			if(((center.x + u) >= 0) && ((center.x + u) < w))
			{
				if( ((center.y + v) >= 0) && ((center.y + v) < h) )
				{
					int offset = getOffset(static_cast<int>(center.x + u),static_cast<int>(center.y + v),pixBox.rowPitch);
					SetPixel(pData + offset,color);
				}
			}
		}
	}

	_pixelBuffer->unlock();
}

inline void EWSManager::SetPixel(Ogre::uint8* data,Ogre::ColourValue& color)
{
	
	Ogre::uint8 pixData[4];
	pixData[0] = static_cast<Ogre::uint8>(color.a * 255); //alpha
	pixData[1] = static_cast<Ogre::uint8>(color.r * 255); //red
	pixData[2] = static_cast<Ogre::uint8>(color.g * 255); //green
	pixData[3] = static_cast<Ogre::uint8>(color.b * 255); //blue

	int format = _pixelBuffer->getFormat();
	//assumes data pointer is set to the correct setting.
	if(format == Ogre::PF_R8G8B8A8)
	{
		*data++ = pixData[1]; //red
		*data++ = pixData[2]; //green
		*data++ = pixData[3]; //blue
		*data++ = pixData[0]; //alpha
	}
	if(format == Ogre::PF_A8R8G8B8)
	{
		//BGRA???!!!! WHAT THE FUCK!!!
		//alpha at the end???
		*data++ = pixData[3]; //blue
		*data++ = pixData[2]; //green
		*data++ = pixData[1]; //red
		*data++ = pixData[0]; //alpha
	}
	 
	return;
}

int EWSManager::getOffset(int x,int y, int rowPitch)
{
	return (x * 4) + ( y * 4 * rowPitch );
}