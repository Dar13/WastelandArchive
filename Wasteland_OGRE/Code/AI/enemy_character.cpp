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

void EnemyCharacter::update(float deltaTime)
{

}

void EnemyCharacter::_behaviorIdle()
{

}

void EnemyCharacter::_behaviorMove(const Ogre::Vector3& target)
{

}

void EnemyCharacter::_behaviorWander(Ogre::Vector3& min,Ogre::Vector3& max)
{

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