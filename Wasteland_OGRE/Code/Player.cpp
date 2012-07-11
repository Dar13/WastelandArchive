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
	cGunData(type,name,magazineSize,numMags);
}

void cGunData::fire()
{
	if(_currentMagazineAmmo == 0 && _fireAnimEnded)
	{
		//don't even bother shooting, there's no point.
		reload();
		return;
	}
	else
	{
		_currentMagazineAmmo--;
	}
}

void cGunData::reload()
{
	if(_reloadNeeded)
	{
		//empty magazine, full reload
		_ammoNotInMag -= _magazineSize;
		_currentMagazineAmmo = _magazineSize;
	}
	else
	{
		//not a full reload
		_ammoNotInMag -= (_magazineSize - _currentMagazineAmmo);
		_currentMagazineAmmo += (_magazineSize - _currentMagazineAmmo);
	}

	_reloadNeeded = false;
}

bool cGunData::frameStarted(const Ogre::FrameEvent& evt)
{

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
			soundF.gunSound = FIRE;
		}
		if( itr->sound() == "DRYFIRE")
		{
			found = true;
			soundF.gunSound = DRYFIRE;
		}
		if( itr->sound() == "RELOAD")
		{
			found = true;
			soundF.gunSound = RELOAD;
		}
		if( itr->sound() == "PUTAWAY")
		{
			found = true;
			soundF.gunSound = PUTAWAY;
		}
		if(itr->sound() == "ALTRELOAD")
		{
			found = true;
			soundF.gunSound = ALTRELOAD;
		}
		if(itr->sound() == "ALTFIRE")
		{
			found = true;
			soundF.gunSound = ALTFIRE;
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
	Ogre::AnimationStateSet* set = entity->getAllAnimationStates();
	_currentAnimation = set->getAnimationState("idle");
}

Player::Player()
{
	_firingWeapon = false;
	_reloadingWeapon = false;
}

Player::~Player()
{
	//nothing yet
}

void Player::Setup(std::string file)
{
	//load list of weapon xml files
	/*
	list_t* wepList = list(file).release();
	for(list_t::file_iterator itr = wepList->file().begin(); itr != wepList->file().end(); ++itr)
	{
		weapon_t* wep = weapon(*itr).release();
		
		delete wep;
	}
	delete wepList;
	*/
}

bool Player::Update(InputManager* input,PhysicsManager* physics,EWSManager* ews,const OgreTransform& transform)
{
	if(input->isCFGKeyPressed(ENVWARNSYS))
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

	if(input->isCFGKeyPressed(CONFIG_KEY_VALUES::RELOAD))
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

void Player::Clean(bool reuse)
{
	return;
}