#include "StdAfx.h"

#include "interfaces\weapon.hxx"

#include "EWS.h"
#include "OISManager.h"
#include "BulletManager.h"
#include "OgreManager.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

struct SWeapon
{
	SWeapon(weapon_t* p = NULL,Ogre::SceneNode* n = NULL) : info(p),node(n) {}
	weapon_t* info;
	Ogre::SceneNode* node;

	static const SWeapon EMPTY;
};

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
	SWeapon _currentWeapon;
	SWeapon _holsteredWeapon;
};

#endif