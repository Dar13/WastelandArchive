#include "StdAfx.h"

#ifndef _ENEMY_CHARACTER_H_
#define _ENEMY_CHARACTER_H_

#include "npc_character.h"
#include "../AnimationBlender.h"
#include "../GunData.h"

#define _PLAYER_INTERFACE_
#include "../Player.h"
#undef _PLAYER_INTERFACE_

class EnemyCharacter : public NPCCharacter
{
	EnemyCharacter(const std::string& name,const std::string& script,Ogre::SceneNode* node,CrowdManager* crowdMgr,DamageInterface* playerInterface = nullptr);

	void update(float deltaTimeInMilliSecs);
private:
	//Methods to implement actions specifically for Enemies
	void _actionReload();
	void _actionShoot(const std::string& target);
	void _actionChangeWeapon(const std::string& newWep);

	std::unique_ptr<cGunData> _weaponRifle;
	std::unique_ptr<cGunData> _weaponPistol;

	cGunData* _currentWeapon;

	DamageInterface* _damageInterface;
};

#endif