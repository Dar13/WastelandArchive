#include "StdAfx.h"

#include "interfaces\weapon.hxx"

#include "EWS.h"
#include "OISManager.h"
#include "BulletManager.h"
#include "OgreManager.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player
{
public:
	Player();
	~Player();

	void Setup(std::string file);
	bool Update(OISManager* input,BulletManager* physics,EWSManager* ews,const OgreTransform& transform);
	void Clean(bool reuse = false);

	void placeEWS(EWSManager* ews,BulletManager* physics,const OgreTransform& transform);

private:
	bool _firingWeapon;
	bool _reloadingWeapon;
};

#endif