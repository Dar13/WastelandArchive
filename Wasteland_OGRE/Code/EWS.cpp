#include "StdAfx.h"
#include "EWS.h"

#include "debug\console.h"
#include "Utility.h"
#include "GraphicsManager.h"

#include "Player.h"

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
	//_ewsTexture = _material->getTechnique(0)->getPass(0)->getTextureUnitState(2)->_getTexturePtr();
	_ewsTexture = _material->getTechnique(0)->getPass(0)->getTextureUnitState(1)->_getTexturePtr();
	
	//get pixel buffer
	_pixelBuffer = _ewsTexture->getBuffer();

	//setting up the scene nodes and entities.
	_ewsNode = scene->getRootSceneNode()->createChildSceneNode("EWSNode");
	_ewsEntity = scene->createEntity("EWSEntity","ewsPlain.mesh");
	_ewsNode->attachObject(_ewsEntity);
	_ewsNode->setPosition(0.0f,2.0f,0.0f);
	_ewsEntity->setMaterial(_material);

	//_placed = true;
	_placed = false;
	_pointToPlayer = false;

	oldTime = 0;

}

void EWSManager::Update(int newTime,bool isPlacing,const sPlayerData& playerData,const OgreTransform& playerTransform)
{
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

	if(_placed)
	{
		if(_pointToPlayer)
		{
			_ewsNode->lookAt(Ogre::Vector3(playerTransform.position.x,_ewsNode->getPosition().y,playerTransform.position.z),Ogre::Node::TS_WORLD);
		}

		//assumes timeElapsed is in ms
		if((newTime - oldTime) > 500 && _playerData != playerData ) //playerInfo != _playerInfo)
		{
			_playerData = playerData;
			//redraw the background.
			Box(Ogre::Rect(15,15,497,497),Ogre::ColourValue(189.0f/255.0f,189.0f/255.0f,189.0f/255.0f,0.5f));

			//draw health information
			Box(Ogre::Rect(100,playerData.health + 100,150,400),Ogre::ColourValue(0.0f,.5f,0.0f,1.0f));

			//draw ammo information
			if(playerData.ammoInMag < 0)
			{
				//ammo not applicable. Draw image accordingly.
			}
			else
			{
				//ammo is applicable. Draw accordingly
				//ammo in magazine
				float magAmmoPcnt = static_cast<float>(_playerData.ammoInMag) / static_cast<float>(_playerData.magSize);
				float height = magAmmoPcnt * 300;
				Box(Ogre::Rect(300,400 - (height / 2),350,400),Ogre::ColourValue(.6f,.01f,.01f,1.0f));
				
				//number of mags left
				//write number on texture
				//integrate this function:
				//http://www.ogre3d.org/tikiwiki/tiki-index.php?page=HowTo%3A+Write+text+on+texture
			}

			oldTime = newTime;
		}
		_material->setDepthWriteEnabled(false);
		_material->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		_material->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));
	}
	else
	{
		return;
	}
	
	return;
}

void EWSManager::Place(const Ogre::Vector3& rayCastPosition,const Ogre::Vector3& rayCastNormal,const OgreTransform& playerTransform)
{
	if(_placed && placeToggle == 2 )
	{
		//if placed already, set to unplaced and make EWS system invisible.
		_placed = false;
		_ewsNode->setVisible(false);
		placeToggle = 1;
	}
	if(!_placed)
	{
		if(placeToggle == 0)
		{
			//to avoid intersection with source node
			if(rayCastNormal != Ogre::Vector3::NEGATIVE_UNIT_Y)
			{
				_ewsNode->setPosition(rayCastPosition + rayCastNormal);
			}
			else
			{
				Ogre::Vector3 v = rayCastNormal;
				v.y--;
				_ewsNode->setPosition(rayCastPosition + v);
			}
			//check to see if it's intersecting with something
			
			//to avoid rotation build-up. Now the node rotates cleanly.
			_ewsNode->resetOrientation();;
			if(rayCastNormal == Ogre::Vector3::UNIT_Y || rayCastNormal == Ogre::Vector3::NEGATIVE_UNIT_Y)
			{
				_pointToPlayer = true;
				if(rayCastNormal == Ogre::Vector3::NEGATIVE_UNIT_Y)
				{
					_ewsNode->lookAt(Ogre::Vector3(playerTransform.position.x,
												   rayCastPosition.y + rayCastNormal.y - 1,
												   playerTransform.position.z),
									 Ogre::Node::TS_WORLD);
				}
				else
					_ewsNode->lookAt(Ogre::Vector3(playerTransform.position.x,
												   rayCastPosition.y + rayCastNormal.y,
												   playerTransform.position.z),
									 Ogre::Node::TS_WORLD);
			}
			else
			{
				_pointToPlayer = false;
				_ewsNode->lookAt(rayCastNormal*2,Ogre::Node::TS_LOCAL);
			}
			_placed = true;
			_ewsNode->setVisible(true);
			placeToggle = 1;
			/*VirtualConsole::getSingleton().put("---\n");
			VirtualConsole::getSingleton().put(Utility::vector3_toStr(rayCastNormal));
			VirtualConsole::getSingleton().put(Utility::vector3_toStr(rayCastPosition));
			VirtualConsole::getSingleton().put(Utility::vector3_toStr(rayCastPosition + rayCastNormal));
			VirtualConsole::getSingleton().put(Utility::vector3_toStr(playerTransform.position));*/
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