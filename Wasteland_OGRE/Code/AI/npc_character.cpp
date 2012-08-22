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

	_prevBhv = 1;
	_prevAct = 1;
	_isBhvFinished = true;
	_isActFinished = true;

	_bhvChange = false;
	_actChange = false;

}

/*! \brief Updates the NPCCharacter

Internally updates the inherited LevelData::BaseEntity and the inherited Character.

*/
void NPCCharacter::update(float deltaTime)
{
	//before anything else, update the position
	updatePosition(deltaTime);

	//call the lua function
	LuaManager::getSingleton().prepFunction(_scriptName);
	LuaManager::getSingleton().pushFunctionArg(deltaTime + .1f);
	LuaManager::getSingleton().pushFunctionArg(_prevBhv);
	LuaManager::getSingleton().pushFunctionArg(_prevAct);
	LuaManager::getSingleton().pushFunctionArg(static_cast<int>(_isBhvFinished));
	LuaManager::getSingleton().pushFunctionArg(static_cast<int>(_isActFinished));
	printDebug(LuaManager::getSingleton().getLuaState());
	LuaManager::getSingleton().callFunction(5,1);

	LuaManager::getSingleton().callFunction("test",0);

	lua_State* lua = LuaManager::getSingleton().getLuaState();

	lua_pushstring(lua,_name.c_str());
	lua_setglobal(lua,"callingEntity");

	int behavior,action;
	std::string bhvTarget,actTarget,changeWep;
	Ogre::Vector3 moveTarget,shootTarget;

	int bhvChange,actChange;
	
	if(lua_istable(LuaManager::getSingleton().getLuaState(),1))
	{
		//get behavior
		lua_pushstring(lua,"behavior");
		lua_gettable(lua,1);
		behavior = lua_tointeger(lua,-1);
		lua_pop(lua,1);

		lua_pushstring(lua,"bhvchange");
		lua_gettable(lua,1);
		bhvChange = lua_tointeger(lua,-1);
		lua_pop(lua,1);
		_bhvChange = (bhvChange == 1) ? true : false;

		//get action
		lua_pushstring(lua,"action");
		lua_gettable(lua,1);
		action = lua_tointeger(lua,-1);
		lua_pop(lua,1);

		lua_pushstring(lua,"actchange");
		lua_gettable(lua,1);
		actChange = lua_tointeger(lua,-1);
		lua_pop(lua,1);
		_actChange = (actChange == 1) ? true : false;

		switch(behavior)
		{
		case AI::BHV_IDLE:
			_behaviorIdle();
			break;
		case AI::BHV_WANDER:
			_behaviorWander();
			break;
		case AI::BHV_TALK:
			lua_pushstring(lua,"bhvtarget");
			lua_gettable(lua,1);
			bhvTarget = lua_tostring(lua,-1);
			lua_pop(lua,1);
			_behaviorTalk(bhvTarget);
			break;
		case AI::BHV_MOVE:
			lua_pushstring(lua,"bhvtarget");
			lua_gettable(lua,1);
			if(lua_istable(lua,-1))
			{
				//this works.
				for(int i = 0; i < 3; i++)
				{
					lua_pushnumber(lua,i+1);
					lua_gettable(lua,-2);
					moveTarget[i] = static_cast<float>(lua_tonumber(lua,-1));
					lua_pop(lua,1);
				}

				lua_pop(lua,1);
			}

			_behaviorMove(moveTarget);
			break;
		case AI::BHV_FOLLOW:
			lua_pushstring(lua,"bhvtarget");
			lua_gettable(lua,1);
			bhvTarget = lua_tostring(lua,-1);
			lua_pop(lua,1);
			_behaviorFollow(bhvTarget);
			break;
		}

		switch(action)
		{
		case AI::ACT_SHOOT:
			lua_pushstring(lua,"acttarget");
			lua_gettable(lua,1);
			if(lua_istable(lua,-1))
			{
				for(int i = 0; i < 3; i++)
				{
					lua_pushnumber(lua,i+1);
					lua_gettable(lua,-2);
					shootTarget[i] = static_cast<float>(lua_tonumber(lua,-1));
					lua_pop(lua,1);
				}
			}
			lua_pop(lua,1);
			break;
		case AI::ACT_CHANGEWEP:
			lua_pushstring(lua,"weapon");
			lua_gettable(lua,1);
			if(lua_isstring(lua,-1))
			{
				changeWep = lua_tostring(lua,-1);
			}
			lua_pop(lua,1);
			break;
		}
	}

	_prevBhv = behavior;
	_prevAct = action;


	//lastly, update the animations
	//_anim.update(deltaTime);
	
}

void NPCCharacter::_behaviorIdle()
{
	stop();
	//still want to point the character in the direction last traveled.
	Ogre::Vector3 vel = getVelocity();
	float speed = vel.length();
	Ogre::Vector3 src = _node->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	src.y = 0;

	vel.y = 0;
	vel.normalise();
	_node->rotate(src.getRotationTo(vel));

	//transition to idle animation
}

void NPCCharacter::_behaviorMove(const Ogre::Vector3& target)
{
	//Check for duplicate move calls and update lua function call with that info
	if(_destination.squaredDistance(target) <= 9)
	{
		if(destinationReached())
		{
			_isBhvFinished = true;
			_behaviorIdle();
		}
	}
	updateDestination(target,false);

	//point the character in the direction it's traveling
	Ogre::Vector3 vel = getVelocity();
	float speed = vel.length();
	vel.y = 0;
	vel.normalise();
	if(speed > .2f)
	{
		//moving sufficiently fast, change to moving animation and point character
		Ogre::Vector3 src = _node->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		src.y = 0;

		_node->rotate(src.getRotationTo(vel));

		//change animation if needed.
	}
}

void NPCCharacter::_behaviorWander()
{
}

void NPCCharacter::_behaviorTalk(const std::string& targetName)
{

}

void NPCCharacter::_behaviorFollow(const std::string& targetName)
{

}