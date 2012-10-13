#include "StdAfx.h"

#include "enemy_character.h"
#include "../LuaManager.h"
#include "../AI_include.h"

EnemyCharacter::EnemyCharacter(const std::string& name,const std::string& script,Ogre::SceneNode* node,CrowdManager* crowdMgr)
	: BaseEntity(false,LevelData::ENEMY),
	Character(node,crowdMgr,node->getPosition())
{
	_name = name;
	_scriptName = script;

	Ogre::Entity* ent = static_cast<Ogre::Entity*>(getMovableObject());

	//this will probably have to change
	node->scale(.1f,.1f,.1f);

	if(ent != nullptr)
	{
		std::cout << "EnemyCharacter \'" << name << "\' has an entity attached." << std::endl;
		Ogre::AnimationStateSet* animSet = ent->getAllAnimationStates();
		if(animSet != nullptr)
		{
			std::cout << " - has animations" << std::endl;
			_animHandler.setEntity(ent);
			_animHandler.init("Idle",true);
		}
		else
		{
			std::cout << " - doesn't have animations" << std::endl;
			_animHandler.setEntity(nullptr);
		}
	}

	_prevBhv = AI::BHV_IDLE;
	_prevAct = AI::ACT_IDLE;
	_isBhvFinished = true;
	_isActFinished = true;

	_bhvChange = false;
	_actChange = false;
}

void EnemyCharacter::update(float deltaTimeInMilliSecs)
{
	updatePosition(deltaTimeInMilliSecs/1000.0f);

	lua_State* lua = LuaManager::getSingleton().getLuaState();

	lua_pushstring(lua,_name.c_str());
	lua_setglobal(lua,"callingEntity");

	lua_getglobal(lua,this->_scriptName.c_str());
	lua_pushnumber(lua,deltaTimeInMilliSecs);
	lua_pushinteger(lua,_prevBhv);
	lua_pushinteger(lua,_prevAct);
	lua_pushinteger(lua,static_cast<int>(_isBhvFinished));
	lua_pushinteger(lua,static_cast<int>(_isActFinished));
	int err = lua_pcall(lua,5,1,0);
	if(err == 2)
	{
		std::cout << "Lua error!" << std::endl;
		if(lua_isstring(lua,1))
		{
			std::cout << lua_tostring(lua,1) << std::endl;
		}
	}

	int behavior = 0,action = 0;
	std::string bhvTarget,actTarget,changeWep;
	Ogre::Vector3 moveTarget,tmp,min,max,mtmp;

	int bhvChange = 0,actChange = 0;

	if(lua_istable(LuaManager::getSingleton().getLuaState(),1))
	{

		bhvChange = LuaManager::getIntegerFromLuaTable(lua,"bhvchange");
		_bhvChange = (bhvChange == 1) ? true : false;

		if(_bhvChange)
		{
			behavior = LuaManager::getIntegerFromLuaTable(lua,"behavior");
		}
		else
		{
			behavior = _prevBhv;
		}

		actChange = LuaManager::getIntegerFromLuaTable(lua,"actchange");
		_actChange = (actChange == 1) ? true : false;

		if(_actChange)
		{
			action = LuaManager::getIntegerFromLuaTable(lua,"action");
		}
		else
		{
			action = _prevAct;
		}

		switch(behavior)
		{
		case AI::BHV_IDLE:
			_behaviorIdle();
			break;
		case AI::BHV_MOVE:
			moveTarget = LuaManager::getVectorFromLuaTable(lua,"bhvtarget");
			if(moveTarget == Ogre::Vector3(-1000,-1000,-1000))
			{
				moveTarget = _destination;
			}

			_behaviorMove(moveTarget);
			break;
		case AI::BHV_WANDER:
			min = LuaManager::getVectorFromLuaTable(lua,"bhvmin");
			max = LuaManager::getVectorFromLuaTable(lua,"bhvmax");
			if(min == Ogre::Vector3::ZERO || max == Ogre::Vector3::ZERO)
			{
				min = _destination;
				max = _destination;
			}
			_behaviorWander(min,max);
			break;
		case AI::BHV_FOLLOW:
			bhvTarget = LuaManager::getStringFromLuaTable(lua,"bhvtarget");
			_behaviorFollow(bhvTarget);
			break;
		case AI::BHV_TALK:
			bhvTarget = LuaManager::getStringFromLuaTable(lua,"bhvtarget");
			_behaviorTalk(bhvTarget);
			break;
		}

	}
}

void EnemyCharacter::_behaviorIdle()
{
	//stop moving the character.
	stop();

	Ogre::Vector3 vel = getVelocity();
	float speed = vel.length();
	vel.y = 0;
	vel.normalise();

	Utility::rotateToTarget(_node,vel,true);

	_animHandler.blend("Idle",AnimationBlender::BlendWhileAnimating,.2f,true);

	_isBhvFinished = true;
}

void EnemyCharacter::_behaviorMove(const Ogre::Vector3& target)
{
	if(_destination.squaredDistance(target) >= 6)
	{
		updateDestination(target,false);
		_destination = target;
	}

	_isBhvFinished = (destinationReached()) ? true : false;

	Ogre::Vector3 velocity = getVelocity();
	float speed = velocity.length();
	velocity.y = 0;
	velocity.normalise();

	if(speed > .2f)
	{
		Utility::rotateToTarget(_node,target,true);

		if(_animHandler.getSource() != nullptr)
		{
			Ogre::AnimationState* target = _animHandler.getTarget();
			if(target == nullptr)
			{
				_animHandler.blend("Walk",AnimationBlender::BlendWhileAnimating,.2f,true);
			}
			else
			{
				if(target->getAnimationName() != "Walk")
				{
					_animHandler.blend("Walk",AnimationBlender::BlendWhileAnimating,.2f,true);
				}
			}
		}
	}
}

void EnemyCharacter::_behaviorWander(Ogre::Vector3& min,Ogre::Vector3& max)
{
	if(min == _destination || max == _destination)
	{
		_behaviorMove(_destination);
		return;
	}

	Utility::fixMinMax(min,max);
	Ogre::AxisAlignedBox box(min,max);

	Ogre::Vector3 point;
	point.x = Ogre::Math::RangeRandom(min.x,max.x);
	point.y = (min.y + max.y) / 2.0f;
	point.z = Ogre::Math::RangeRandom(min.z,max.z);

	if(box.intersects(point))
	{
		_behaviorMove(point);
	}
	else
	{
		_behaviorMove(_destination);
	}

	return;
}

void EnemyCharacter::_behaviorTalk(const std::string& targetName)
{

}

void EnemyCharacter::_behaviorFollow(const std::string& targetName)
{

}

void EnemyCharacter::_actionIdle()
{

}

void EnemyCharacter::_actionLookAt(const Ogre::Vector3& target)
{

}

void EnemyCharacter::_actionReload()
{

}

void EnemyCharacter::_actionShoot(const std::string& target)
{

}

void EnemyCharacter::_actionChangeWeapon(const std::string& newWep)
{

}