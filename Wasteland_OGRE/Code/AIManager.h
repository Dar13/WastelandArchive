#include "StdAfx.h"

#ifndef _AI_MANAGER_H_
#define _AI_MANAGER_H_

#include "AI\npc_character.h"
#include "CrowdManager.h"
#include "LuaManager.h"

#include "interfaces\characterobject.hxx"

class AIManager
{
public:
	~AIManager();
	void loadNPCs(std::string fileName,CrowdManager* Crowd,Ogre::SceneManager* Scene,float maxSpeed = .9f);

	void update(float deltaTime);
private:
	std::vector<NPCCharacter*> _npcs;
};

#endif