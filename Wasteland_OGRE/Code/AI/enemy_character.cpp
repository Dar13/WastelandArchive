#include "StdAfx.h"

#include "enemy_character.h"
#include "npc_character.h"
#include "../LuaManager.h"
#include "../AI_include.h"

EnemyCharacter::EnemyCharacter(const std::string& name,const std::string& script,Ogre::SceneNode* node,CrowdManager* crowdMgr,DamageInterface* damageInterface)
	: NPCCharacter(name,script,node,crowdMgr),
	  _damageInterface(damageInterface)
{
	_name = name;
	_scriptName = script;

	Ogre::Entity* ent = static_cast<Ogre::Entity*>(getMovableObject());

	//this will probably have to change
	node->scale(CHARACTER_SCALE_FACTOR,CHARACTER_SCALE_FACTOR,CHARACTER_SCALE_FACTOR);

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

	//set up the weapon
	_currentWeapon.node = nullptr;
	_currentWeapon.weapon = nullptr;

	_prevBhv = AI::BHV_IDLE;
	_prevAct = AI::ACT_IDLE;
	_isBhvFinished = true;
	_isActFinished = true;

	_bhvChange = false;
	_actChange = false;
}

void EnemyCharacter::addWeapons(GraphicsManager* graphics,SoundManager* sound)
{
	EquippableObject tmp;
	/*tmp = GameManager::createEquippable(_node->getCreator(),"resources//xml//weapon_g36c.xml",graphics,sound,true);
	_weaponRifle.node = tmp.node;
	_weaponRifle.weapon = static_cast<cGunData*>(tmp.equip);*/

	tmp = GameManager::createEquippable(_node->getCreator(),"resources//xml//weapon_m9se.xml",graphics,sound,true);
	_weaponPistol.node = tmp.node;
	_weaponPistol.weapon = static_cast<cGunData*>(tmp.equip);
}

void EnemyCharacter::update(float deltaTimeInMilliSecs)
{
	updatePosition(deltaTimeInMilliSecs/1000.0f);

	double dmg = _damageInterface->getEnemyDamage(this->_name);
	//subtract from health, kill off if below zero.

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

void EnemyCharacter::_actionReload()
{
	_currentWeapon.weapon->reload();
	_isActFinished = true;
}

void EnemyCharacter::_actionShoot(const std::string& target)
{
	//look at entity target
	LevelData::BaseEntity* entity = LuaManager::getSingleton().getEntity(target);
	//if the target isn't a NPC/Enemy, I don't want my enemy wasting his ammo.
	if(entity->getType() == LevelData::NPC || entity->getType() == LevelData::ENEMY)
	{
		Ogre::Vector3 targetPosition = static_cast<NPCCharacter*>(entity)->getPosition();

		Ogre::Vector3 sourcePosition = _node->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		sourcePosition.y = 0;

		Utility::rotateToTarget(_node,targetPosition,true);

		//after rotation, fire?
		_currentWeapon.weapon->fire();
		_damageInterface->registerShotAtPlayer(_currentWeapon.weapon->getGunshotData(),sourcePosition.squaredDistance(targetPosition));
	}
}

//newWep should be either 'Rifle','Pistol' or 'None'
void EnemyCharacter::_actionChangeWeapon(const std::string& newWep)
{
	if(_currentWeapon.node != nullptr)
	{
		if(_currentWeapon.node->getAttachedObject(0) == nullptr)
		{
			//re-add the gun entity back to the original node.
			Ogre::Entity* characterEntity = static_cast<Ogre::Entity*>(_movableObject);
			Ogre::MovableObject* attachedWeap = characterEntity->detachObjectFromBone(_currentWeapon.weapon->_getGunNameString());
			_currentWeapon.node->attachObject(attachedWeap);
		}
	}

	//switch the _currentWeapon
	if(newWep == "Rifle"){ _currentWeapon = _weaponRifle; }
	if(newWep == "Pistol") { _currentWeapon = _weaponPistol; }
	if(newWep == "None") { _currentWeapon.node = nullptr; _currentWeapon.weapon = nullptr; }

	if(_currentWeapon.node != nullptr)
	{
		Ogre::MovableObject* unattachedWeap = _currentWeapon.node->detachObject(static_cast<unsigned short>(0));
		static_cast<Ogre::Entity*>(_movableObject)->attachObjectToBone("FIRESPOT",unattachedWeap);
	}
	
}