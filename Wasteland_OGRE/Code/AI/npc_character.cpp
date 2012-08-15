#include "StdAfx.h"

#include ".\npc_character.h"

#include "../LuaManager.h"

#include "../AI_include.h"

NPCCharacter::NPCCharacter(const std::string& name,const std::string& script,Ogre::SceneNode* node,CrowdManager* crowdMgr)
	: BaseEntity(false,LevelData::NPC),
	  Character(node,crowdMgr,node->getPosition())
{
	_name = name;
	_scriptName = script;
	_activated = false; // just ensuring that it's de-activated

}

void NPCCharacter::update(float deltaTime)
{
	//call the lua function
	LuaManager::getSingleton().prepFunction(_scriptName);
	LuaManager::getSingleton().pushFunctionArg(deltaTime);
	LuaManager::getSingleton().callFunction(1,1);
	
	if(lua_istable(LuaManager::getSingleton().getLuaState(),1))
	{
		lua_State* lua = LuaManager::getSingleton().getLuaState();
		//get behavior
		lua_pushstring(lua,"behavior");
		lua_gettable(lua,1);
		int behavior = lua_tonumber(lua,-1);
		lua_pop(lua,1);

		//get action
		lua_pushstring(lua,"action");
		lua_gettable(lua,1);
		int action = lua_tonumber(lua,-1);
		lua_pop(lua,1);

		std::string bhvTarget,actTarget;
		Ogre::Vector3 moveTarget;

		switch(behavior)
		{
		case AI::BHV_TALK:
			lua_pushstring(lua,"bhvtarget");
			lua_gettable(lua,1);
			bhvTarget = lua_tostring(lua,-1);
			lua_pop(lua,1);
			break;
		case AI::BHV_MOVE:
			lua_pushstring(lua,"bhvtarget");
			lua_gettable(lua,1);
			if(lua_istable(lua,-1))
			{
				
			}
			break;
		case AI::BHV_FOLLOW:

			break;
		}

		switch(action)
		{
		case AI::ACT_SHOOT:

			break;
		case AI::ACT_CHANGEWEP:

			break;
		}
	}
}