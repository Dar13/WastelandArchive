#include "StdAfx.h"

#ifndef _NPC_CHARACTER_H_
#define _NPC_CHARACTER_H_

#include "../LevelData.h"
#include "../Character.h"
//#include "../AI_include.h"

class NPCCharacter : public LevelData::BaseEntity,Character
{
public:
	NPCCharacter(const std::string& name,const std::string& script,Ogre::SceneNode* node,CrowdManager* crowdMgr);

	void update(float deltaTime);

private:
	
};

#endif