#include "StdAfx.h"

#include "AIManager.h"

//Destructor
AIManager::~AIManager()
{
	std::for_each(_npcs.begin(),_npcs.end(),[](NPCCharacter* npc) {
		delete npc;
		npc = nullptr;
	});
	_npcs.clear();
}

void AIManager::loadNPCs(std::string fileName,CrowdManager* Crowd, Ogre::SceneManager* Scene,float maxSpeed)
{
	auto npcList = list(fileName.c_str());
	for(auto itr = npcList->file().begin(); itr != npcList->file().end(); ++itr)
	{
		characterobject_t* obj = characterObject(*itr).release();
		Ogre::SceneNode* node = GameManager::createCharacterObject(Scene,obj);

		NPCCharacter* npc = new NPCCharacter(obj->name(),obj->scriptName(),node,Crowd);
		npc->setMaxSpeed(maxSpeed);
		_npcs.push_back(npc);
		LuaManager::getSingleton().addEntity(npc->getName(),npc);

		delete obj;
	}
}

void AIManager::update(float deltaTimeInMs)
{
	for(auto itr = _npcs.begin(); itr != _npcs.end(); ++itr)
	{
		(*itr)->update(deltaTimeInMs);
	}
}