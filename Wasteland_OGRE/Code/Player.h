#include "StdAfx.h"

#include "interfaces\weapon.hxx"

#include "EWS.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "GraphicsManager.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

enum GUN_TYPE
{
	NONE = 0,
	PISTOL,
	ASSAULT,
	SMG,
	SHOTGUN,
	HEAVY
};

enum GUN_NAME
{
	NONE = 0,
	M9,
	G36C
};

class cGunData
{
public:
	cGunData(GUN_TYPE type,GUN_NAME name,int magazineSize,int tAmmo);

	void fireGun(); //fires one round

	void setMagazineSize(const int& magSize);

private:
	//Gun description
	GUN_TYPE _type;
	GUN_NAME _name;
	int _magazineSize; //shouldn't be changed except in constructor

	int _totalAmmo;
	int _currentMagazineAmmo;
};

class Player
{
public:
	Player();
	~Player();

	void Setup(std::string file);
	bool Update(InputManager* input,PhysicsManager* physics,EWSManager* ews,const OgreTransform& transform);
	void Clean(bool reuse = false);

	void placeEWS(EWSManager* ews,PhysicsManager* physics,const OgreTransform& transform);

	int getHealth();

private:
	bool _firingWeapon;
	bool _reloadingWeapon;

	int _health;


};

#endif