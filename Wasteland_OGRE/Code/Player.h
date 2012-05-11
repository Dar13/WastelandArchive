#include "StdAfx.h"

#include "interfaces\weapon.hxx"

#include "EWS.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "GraphicsManager.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player
{
public:
	Player();
	~Player();

	void Setup(std::string file);
	bool Update(InputManager* input,PhysicsManager* physics,EWSManager* ews,const OgreTransform& transform);
	void Clean(bool reuse = false);

	void placeEWS(EWSManager* ews,PhysicsManager* physics,const OgreTransform& transform);

private:
	bool _firingWeapon;
	bool _reloadingWeapon;
};

#endif