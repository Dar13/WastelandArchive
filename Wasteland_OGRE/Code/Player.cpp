#include "StdAfx.h"
#include "interfaces\list.hxx"
#include "interfaces\weapon.hxx"
#include "Player.h"

#include "OISManager.h"

template<> Player* Ogre::Singleton<Player>::ms_Singleton = 0;

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
	list_t* wepList = list(file).release();
	for(list_t::file_iterator itr = wepList->file().begin(); itr != wepList->file().end(); ++itr)
	{
		weapon_t* wep = weapon(*itr).release();
		
		delete wep;
	}
	delete wepList;
}

bool Player::Update()
{
	if(!_reloadingWeapon)
	{
		if(OISManager::getSingleton().isMBPressed(OIS::MB_Left))
		{
			//shoot gun
			_firingWeapon = true;
		}
		else
		{
			_firingWeapon = false;
		}

		if(OISManager::getSingleton().isMBPressed(OIS::MB_Right) && !_firingWeapon)
		{
			//zoom gun
		}

		if(OISManager::getSingleton().isCFGKeyPressed(RELOAD) && !_firingWeapon)
		{
			//reload gun
			_reloadingWeapon = true;
		}
	}
	else
	{
		//reload gun action.

	}

	return true;
}

void Player::Clean(bool reuse)
{
	return;
}