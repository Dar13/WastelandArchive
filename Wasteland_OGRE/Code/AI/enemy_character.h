#include "StdAfx.h"

#ifndef _ENEMY_CHARACTER_H_
#define _ENEMY_CHARACTER_H_

#include "npc_character.h"
#include "../AnimationBlender.h"

class EnemyCharacter : public NPCCharacter
{
	EnemyCharacter(const std::string& name,const std::string& script,Ogre::SceneNode* node,CrowdManager* crowdMgr);

	void update(float deltaTimeInMilliSecs);
private:
	//Methods to implement actions specifically for Enemies
	void _actionReload();
	void _actionShoot(const std::string& target);
	void _actionChangeWeapon(const std::string& newWep);
};

#endif