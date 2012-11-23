#include "StdAfx.h"
#include "AIManager.h"
#include "LuaManager.h"

void AIManager::loadNPCsFromFile(std::string file,Ogre::SceneManager* scene,GraphicsManager* Graphics,CrowdManager* crowd)
{
	auto npcList = list(file.c_str());
	for(auto itr = npcList->file().begin(); itr != npcList->file().end(); ++itr)
	{
		characterobject_t* obj = characterObject(*itr).release();
		Ogre::SceneNode* node = GameManager::createCharacterObject(scene,obj,Graphics);

		NPCCharacter* npc = new NPCCharacter(obj->name(),obj->scriptName(),node,crowd);
		npc->setMaxSpeed(.9f);
		_NPCs.push_back(npc);
		LuaManager::getSingleton().addEntity(npc->getName(),npc);

		delete obj;
	}
}