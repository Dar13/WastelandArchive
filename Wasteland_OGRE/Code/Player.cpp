#include "StdAfx.h"
#include "interfaces\list.hxx"
#include "interfaces\weapon.hxx"
#include "Player.h"

#include "InputManager.h"
#include "Utility.h"

#include "debug\print.h"

cGunData::cGunData(GUN_TYPE type,GUN_NAME name,int magazineSize,int numMags)
{
	_type = type;
	_name = name;
	_magazineSize = magazineSize;

	_ammoNotInMag = (numMags * _magazineSize) - _magazineSize;
	_currentMagazineAmmo = _magazineSize;

	_reloadNeeded = false;
}

void cGunData::fire()
{
	_currentMagazineAmmo--;
	if(_currentMagazineAmmo == 0)
	{
		_reloadNeeded = true;
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