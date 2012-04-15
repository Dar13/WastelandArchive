#include "StdAfx.h"
#include "interfaces\list.hxx"
#include "interfaces\weapon.hxx"
#include "Player.h"

#include "OISManager.h"
#include "Utility.h"

#include "debug\print.h"

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

bool Player::Update(OISManager* input,BulletManager* physics,EWSManager* ews,const OgreTransform& transform)
{
	if(input->isCFGKeyPressed(ENVWARNSYS))
	{
		placeEWS(ews,physics,transform);
	}
	return true;
}

void Player::placeEWS(EWSManager* ews,BulletManager* physics,const OgreTransform& transform)
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