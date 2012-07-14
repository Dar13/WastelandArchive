#include "StdAfx.h"
#include "interfaces\list.hxx"
#include "interfaces\weapon.hxx"
#include "Player.h"

#include "InputManager.h"
#include "Utility.h"

#include "GameManager.h"

#include "debug\print.h"

cGunData::cGunData(GUN_TYPE type,GUN_NAME name,int magazineSize,int numMags)
{
	_type = type;
	_name = name;
	_magazineSize = magazineSize;

	_ammoNotInMag = (numMags * _magazineSize) - _magazineSize;
	_currentMagazineAmmo = _magazineSize;

	_reloadNeeded = false;

	_isWeapon = true;
	_isEquipped = false;

	_playingAnim = 0;

	_moving = false;
	_firing = false;
	_reloading = false;
}

cGunData::cGunData(const baseEquippable& base,GUN_TYPE type,GUN_NAME name,int magazineSize,int numMags)
{
	_isWeapon = base.getIsWeapon();
	_isEquipped = base.getEquipped();
	
	_type = type;
	_name = name;
	_magazineSize = magazineSize;

	_ammoNotInMag = (numMags * _magazineSize) - _magazineSize;
	_currentMagazineAmmo = _magazineSize;

	_reloadNeeded = false;

	_playingAnim = 0;

	_moving = false;
	_firing = false;
	_reloading = false;
}

void cGunData::fire()
{
	if(_reloadNeeded)
	{
		reload();
	}
	else
	{
		_currentMagazineAmmo--;
		_firing = true;
		if(_currentMagazineAmmo == 0)
		{
			_reloadNeeded = true;
		}
	}
}

void cGunData::reload()
{
	if(_reloadNeeded)
	{
		//empty magazine, full reload
		_ammoNotInMag -= _magazineSize;
		_currentMagazineAmmo = _magazineSize;
		_reloading = true;
	}
	else
	{
		//not a full reload
		_ammoNotInMag -= (_magazineSize - _currentMagazineAmmo);
		_currentMagazineAmmo += (_magazineSize - _currentMagazineAmmo);
		_reloading = true;
	}

	_reloadNeeded = false;
}

bool cGunData::frameStarted(const Ogre::FrameEvent& evt)
{
	_animBlender.addTime(evt.timeSinceLastFrame);

	int sourceAnim = getAnimID(_animBlender.getSource()->getAnimationName());
	int targetAnim;
	
	//sometimes the target is 0.
	if(_animBlender.getTarget() != nullptr)
	{
		targetAnim = getAnimID(_animBlender.getTarget()->getAnimationName());
	}
	else
	{
		targetAnim = cGunData::NO_ANIM;
	}

	//Doesn't matter if the player is moving or not. Only if he's firing the weapon.
	if(_reloading && !_firing)
	{
		//if the source is reload then job is done.
		if(sourceAnim != cGunData::ANIM_RELOAD)
		{
			//if the blend is completely done.
			if(targetAnim != cGunData::NO_ANIM)
			{
				_animBlender.blend("reload",AnimationBlender::BlendSwitch,.2f,false);
				std::cout << "Reload deactivated" << std::endl;
			}

			//if the blend isn't quite done
			if(targetAnim != cGunData::ANIM_RELOAD)
			{
				_animBlender.blend("reload",AnimationBlender::BlendSwitch,.2f,false);
				std::cout << "Reload deactivated" << std::endl;
			}
		}
		else
		{
			if(_animBlender.complete)
			{
				_reloading = false;
			}
		}
	}

	//for moving animation.
	if(_moving && !_firing && !_reloading)
	{
		if(sourceAnim != cGunData::ANIM_MOVE)
		{
			if(targetAnim != cGunData::NO_ANIM)
			{
				_animBlender.blend("move",AnimationBlender::BlendSwitch,.2f,true);
			}
			
			if(targetAnim != cGunData::ANIM_MOVE)
			{
				_animBlender.blend("move",AnimationBlender::BlendSwitch,.2f,true);
			}
		}
		std::cout << "Moving" << std::endl;
	}

	//nothing else is going on, go to idle
	if(!_reloading && !_firing && !_moving)
	{
		if(sourceAnim != cGunData::ANIM_IDLE)
		{
			if(targetAnim != cGunData::NO_ANIM)
			{
				_animBlender.blend("idle",AnimationBlender::BlendWhileAnimating,.2f,true);
			}
			else if(targetAnim != cGunData::ANIM_IDLE)
			{
				_animBlender.blend("idle",AnimationBlender::BlendWhileAnimating,.2f,true);
			}
			else
			{
				//debug statement.
				std::cout << "idle not activated!" << "REL:" << _reloading << " MOV:" << _moving << " FIR:" << _firing << std::endl;
			}
		}
	}
	else
	{
		//for debug statements and such.
	}

	//for some of the other functions in the class.
	_playingAnim = getAnimID(_animBlender.getSource()->getAnimationName());

	return true;
}

bool cGunData::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//not needed right now I think.
	return true;
}

bool cGunData::frameEnded(const Ogre::FrameEvent& evt)
{
	//not needed right now.
	return true;
}

void cGunData::setSoundFrames(weapon_t* Weapon)
{
	for(auto itr = Weapon->soundFrames().soundframe().begin();
		itr != Weapon->soundFrames().soundframe().end();
		++itr)
	{
		sSoundFrame soundF;
		for(auto _itr = (*itr).frame().begin();
			_itr != (*itr).frame().end();
			++_itr)
		{
			soundF.frames.push_back(static_cast<int>(*_itr));
		}

		soundF.gunSound = getSoundID(itr->sound());

		_soundFrames.push_back(soundF);
	}
	std::cout << "Soundframes set for " << Weapon->name() << std::endl;
}

void cGunData::setAnimationFrames(Ogre::Entity* entity)
{
	_animBlender.setEntity(entity);
	_animBlender.init("idle",true);
	std::cout << "AnimationFrames set for " << entity->getName() << std::endl;
}

Player::Player()
{
	_firingWeapon = false;
	_reloadingWeapon = false;

	_health = 100;
}

Player::~Player()
{
	//clean up equippables
	for(auto itr = _equippables.begin(); itr != _equippables.end(); ++itr)
	{
		if(itr->equip->getIsWeapon())
		{
			delete static_cast<cGunData*>(itr->equip);
		}
		else
		{
			delete itr->equip;
		}
	}
}

void Player::Setup(const std::string& file,GraphicsManager* graphics,Ogre::SceneNode* equipNode)
{
	if(equipNode != nullptr)
	{
		_equipNode = equipNode;
	}

	if(_equippables.size() == 1 && _equipNode != nullptr)
	{
		//equip the one weapon/thing
		equipObject(_equippables[_curEquippable]);
	}

	for(auto itr = _equippables.begin(); itr != _equippables.end(); ++itr)
	{
		if(itr->equip->getIsWeapon())
		{
			graphics->getRoot()->addFrameListener(static_cast<cGunData*>(itr->equip));
		}
	}
}

bool Player::Update(InputManager* input,PhysicsManager* physics,EWSManager* ews,const OgreTransform& transform)
{
	if(input->isCFGKeyPressed(InputManager::ENVWARNSYS))
	{
		placeEWS(ews,physics,transform);
	}

	cGunData* gun = nullptr;

	if(_equippables[_curEquippable].equip->getIsWeapon())
	{
		gun = static_cast<cGunData*>(_equippables[_curEquippable].equip);
	}

	

	if(gun != nullptr)
	{	
		if(input->isMBPressed(OIS::MB_Left))
		{
			//shoot gun
			gun->fire();
		}

		if(input->isCFGKeyPressed(InputManager::RELOAD))
		{
			gun->reload();
		}

		if(input->isCFGKeyPressed(InputManager::FORWARD))
		{
			gun->setMoving(true);
		}
		else
		{
			if(gun->isMoving())
			{
				gun->setMoving(false);
			}
		}
	}

	return true;
}

void Player::placeEWS(EWSManager* ews,PhysicsManager* physics,const OgreTransform& transform)
{
	btVector3 rayPosition,rayNormal;
	btVector3 rayStart = Utility::convert_OgreVector3(transform.position);
	btVector3 rayEnd;
	Ogre::Vector3 dir = transform.position + (transform.direction * 1000.0f);
	rayEnd = Utility::convert_OgreVector3(dir);
	if(physics->RaycastWorld_Closest(rayStart,rayEnd,rayPosition,rayNormal))
	{
		ews->Place(Utility::convert_btVector3(rayPosition),Utility::convert_btVector3(rayNormal),transform);
	}
}

void Player::equipObject(const EquippableObject& obj)
{
	obj.node->getParentSceneNode()->removeChild(obj.node);
	_equipNode->addChild(obj.node);
	obj.node->setPosition(1.0f,.25f,-.5f);
	std::cout << "Weapon equipped." << std::endl;
}

void Player::addEquippableObject(const EquippableObject& object)
{
	_equippables.push_back(object);
	_curEquippable = 0; //there's only one...
	std::cout << "Weapon added to equippables." << std::endl;
}

sPlayerData Player::getPlayerData()
{
	if(_equippables[_curEquippable].equip->getIsWeapon())
	{
		return sPlayerData(_health,
						   static_cast<cGunData*>(_equippables[_curEquippable].equip)->getMagAmmo(),
						   static_cast<cGunData*>(_equippables[_curEquippable].equip)->getNumOfMags());
	}
	else
	{
		//-1 denotes ammo is not applicable
		return sPlayerData(_health,-1,-1);
	}
}

void Player::Clean(bool reuse)
{
	return;
}

//-------------------------------------

int getAnimID(const std::string& name)
{
	if(name == "idle")
	{
		return cGunData::ANIM_IDLE;
	}

	if(name == "startfire")
	{
		return cGunData::ANIM_STARTFIRE;
	}

	if(name == "endfire")
	{
		return cGunData::ANIM_MOVE;
	}

	if(name == "move")
	{
		return cGunData::ANIM_MOVE;
	}

	if(name == "autofire")
	{
		return cGunData::ANIM_AUTOFIRE;
	}

	if(name == "reload")
	{
		return cGunData::ANIM_RELOAD;
	}

	if(name == "select")
	{
		return cGunData::ANIM_SELECT;
	}

	if(name == "putaway")
	{
		return cGunData::ANIM_PUTAWAY;
	}

	return cGunData::NO_ANIM;
}

int getSoundID(const std::string& sound)
{
	if(sound == "DRYFIRE")
	{
		return cGunData::SND_DRYFIRE;
	}
	if(sound == "FIRE")
	{
		return cGunData::SND_FIRE;
	}
	if(sound == "ALTFIRE")
	{
		return cGunData::SND_ALTFIRE;
	}
	if(sound == "ALTRELOAD")
	{
		return cGunData::SND_ALTRELOAD;
	}
	if(sound == "RELOAD")
	{
		return cGunData::SND_RELOAD;
	}
	if(sound == "PUTAWAY")
	{
		return cGunData::SND_PUTAWAY;
	}

	return cGunData::NO_SOUND;
}