#include "StdAfx.h"

#ifndef _NPC_CHARACTER_H_
#define _NPC_CHARACTER_H_

#include "../LevelData.h"
#include "../Character.h"
#include "../AnimationBlender.h"
//#include "../AI_include.h"

#define CHARACTER_SCALE_FACTOR .0275f

class NPCCharacter : public LevelData::BaseEntity,public Character
{
public:
	NPCCharacter(const std::string& name,const std::string& script,Ogre::SceneNode* node,CrowdManager* crowdMgr);

	void update(float deltaTimeInMilliSecs);

protected:
	//Methods to implement behaviors specific to NPCs.
	void _behaviorMove(const Ogre::Vector3& target);
	void _behaviorIdle();
	void _behaviorWander(Ogre::Vector3& min,Ogre::Vector3& max);
	void _behaviorTalk(const std::string& targetName);
	void _behaviorFollow(const std::string& targetName);

	//Methods to implement actions specific to NPCs.
	void _actionIdle();
	void _actionLook(const Ogre::Vector3& target);

	int _prevBhv;
	int _prevAct;
	bool _isBhvFinished;
	bool _isActFinished;

	bool _bhvChange,_actChange;

	AnimationBlender _animHandler;
};

#endif