#include "StdAfx.h"

#include "interfaces\weapon.hxx"

#ifndef _PLAYER_H_
#define _PLAYER_H_

struct SWeapon
{
	weapon_t* info;
	Ogre::SceneNode* node;
};

class Player : public Ogre::Singleton<Player>
{
public:
	Player();
	~Player();

	void Setup(std::string file);
	bool Update();
	void Clean(bool reuse = false);

private:
	bool _firingWeapon;
	bool _reloadingWeapon;
	SWeapon _currentWeapon;
	SWeapon _holsteredWeapon;

	Player(const Player&);
	Player& operator=(Player&);
};

#endif