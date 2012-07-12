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

	_fireAnimEnded = false;
	_reloadAnimEnded = false;
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

	_fireAnimEnded = false;
	_reloadAnimEnded = false;
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

	//update animation blender
	int playingAnim = getAnimID(_animBlender.getSource()->getAnimationName());
	if(_firing)
	{
		if(playingAnim != cGunData::ANIM_RELOAD || playingAnim != cGunData::ANIM_SELECT)
		{
			_animBlender.blend("startfire",AnimationBlender::BlendWhileAnimating,1.0,false);
		}
		if(playingAnim == cGunData::ANIM_STARTFIRE && _animBlender.getTarget() == nullptr)
		{
			_animBlender.blend("endfire",AnimationBlender::BlendWhileAnimating,1.0,false);
		}
		if(playingAnim == cGunData::ANIM_ENDFIRE && _animBlender.getTarget() == nullptr)
		{
			_firing = false;
		}
	}

	if(_reloading)
	{

	}

	if(!_reloading && !_firing && !_moving)
	{
		_animBlender.blend("idle",AnimationBlender::BlendWhileAnimating,1.0,true);
	}

	return true;
}

bool cGunData::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

	return true;
}

bool cGunData::frameEnded(const Ogre::FrameEvent& evt)
{

	return true;
}

int cGunData::getGunType()
{
	return _type;
}

int cGunData::getGunName()
{
	return _name;
}

//!Might be inaccurate.
int cGunData::getNumofMags()
{
	return (_ammoNotInMag + _currentMagazineAmmo) / _magazineSize;
}

int cGunData::getMagAmmo()
{
	return _currentMagazineAmmo;
}

bool cGunData::isReloadNeeded()
{
	return _reloadNeeded;
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
			++itr)
		{
			soundF.frames.push_back(static_cast<int>(*_itr));
		}

		bool found = false;
		if( itr->sound() == "FIRE")
		{
			found = true;
			soundF.gunSound = SND_FIRE;
		}
		if( itr->sound() == "DRYFIRE")
		{
			found = true;
			soundF.gunSound = SND_DRYFIRE;
		}
		if( itr->sound() == "RELOAD")
		{
			found = true;
			soundF.gunSound = SND_RELOAD;
		}
		if( itr->sound() == "PUTAWAY")
		{
			found = true;
			soundF.gunSound = SND_PUTAWAY;
		}
		if(itr->sound() == "ALTRELOAD")
		{
			found = true;
			soundF.gunSound = SND_ALTRELOAD;
		}
		if(itr->sound() == "ALTFIRE")
		{
			found = true;
			soundF.gunSound = SND_ALTFIRE;
		}
		if(!found)
		{
			soundF.gunSound = NO_SOUND;
		}

		_soundFrames.push_back(soundF);
	}
}

void cGunData::setAnimationFrames(Ogre::Entity* entity)
{
	_animBlender.setEntity(entity);
	_animBlender.init("idle",true);
}

Player::Player()
{
	_firingWeapon = false;
	_reloadingWeapon = false;

	_health = 100;
}

Player::~Player()
{
	//nothing yet
}

void Player::Setup(const std::string& file)
{
	if(_equippables.size() == 1)
	{
		//equip the one weapon/thing
		equipObject(*_currentEquippable);
	}
}

bool Player::Update(InputManager* input,PhysicsManager* physics,EWSManager* ews,const OgreTransform& transform)
{
	if(input->isCFGKeyPressed(InputManager::ENVWARNSYS))
	{
		placeEWS(ews,physics,transform);
	}

	if(input->isMBPressed(OIS::MB_Left))
	{
		//shoot gun
		if(_currentEquippable->equip->getIsWeapon())
		{
			cGunData* gun = static_cast<cGunData*>(_currentEquippable->equip.get());
			gun->fire();
		}
	}

	if(input->isCFGKeyPressed(InputManager::RELOAD))
	{
		if(_currentEquippable->equip->getIsWeapon())
		{
			cGunData* gun = static_cast<cGunData*>(_currentEquippable->equip.get());
			gun->reload();
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
}

void Player::addEquippableObject(const EquippableObject& object)
{
	_equippables.push_back(object);
	_currentEquippable = _equippables.begin(); //there's only one...
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