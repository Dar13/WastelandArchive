#include "StdAfx.h"

#ifndef _ENEMY_CHARACTER_H_
#define _ENEMY_CHARACTER_H_

#include "../LevelData.h"
#include "../Character.h"
#include "../AnimationBlender.h"

class EnemyCharacter : public LevelData::BaseEntity,public Character
{
	EnemyCharacter(const std::string& name,const std::string& script,Ogre::SceneNode* node,CrowdManager* crowdMgr);

	void update(float deltaTime);
private:
	//Methods to implement behaviors
	void _behaviorMove(const Ogre::Vector3& target);
	void _behaviorIdle();
	void _behaviorWander(Ogre::Vector3 min,Ogre::Vector3& max);
	void _behaviorTalk(const std::string& targetName);
	void _behaviorFollow(const std::string& targetName);

	//Methods to implement actions for Enemies
	//not yet

	int _prevBhv;
	int _prevAct;
	bool _isBhvFinished;
	bool _isActFinished;

	bool _bhvChange,_actChange;

	AnimationBlender _animHandler;
};

#endif