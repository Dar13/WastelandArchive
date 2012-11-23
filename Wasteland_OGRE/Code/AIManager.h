#include "StdAfx.h"

#ifndef _AI_MANAGER_H_
#define _AI_MANAGER_H_

#include "AI\enemy_character.h"
#include "AI\npc_character.h"

class AIManager
{
public:
	void loadNPCsFromFile(std::string file,Ogre::SceneManager* scene,GraphicsManager* Graphics,CrowdManager* crowd);
	void loadEnemiesFromFile(std::string& file,Ogre::SceneManager* scene,GraphicsManager* Graphics,CrowdManager* crowd);

	~AIManager()
	{
		std::for_each(_NPCs.begin(),_NPCs.end(),[] (NPCCharacter* npc) {
			delete npc;
		});

		std::for_each(_Enemies.begin(),_Enemies.end(),[] (EnemyCharacter* enem) {
			delete enem;
		});
	}

private:
	std::vector<NPCCharacter*> _NPCs;
	std::vector<EnemyCharacter*> _Enemies;
};

#endif