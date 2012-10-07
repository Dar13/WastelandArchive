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

	//check for animations
	Ogre::Entity* ent = static_cast<Ogre::Entity*>(node->getAttachedObject(0));
	
	node->scale(.1f,.1f,.1f);
	if(ent != nullptr)
	{
		std::cout << "NPCCharacter \'" << name << "\' has an entity attached." << std::endl;
		Ogre::AnimationStateSet* animSet = ent->getAllAnimationStates();
		if(animSet != nullptr)
		{
			std::cout << "  - has animations" << std::endl;
			_animHandler.setEntity(ent);
			_animHandler.init("Idle",true);
		}
		else
		{
			std::cout << " - doesn't have animations" << std::endl;
			_animHandler.setEntity(NULL);
		}
	}

	_prevBhv = 0;
	_prevAct = 0;
	_isBhvFinished = true;
	_isActFinished = true;

	_bhvChange = false;
	_actChange = false;
}

/*! \brief Updates the NPCCharacter

Internally updates the inherited LevelData::BaseEntity and the inherited Character.

*/
void NPCCharacter::update(float deltaTimeInMilliSecs)
{
	//before anything else, update the position
	updatePosition(deltaTimeInMilliSecs/1000.0f);

	lua_State* lua = LuaManager::getSingleton().getLuaState();
	
	lua_pushstring(lua,_name.c_str());
	lua_setglobal(lua,"callingEntity");

	lua_getglobal(lua,this->_scriptName.c_str());
	lua_pushnumber(lua,16.666);
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
	Ogre::Vector3 moveTarget,shootTarget;
	Ogre::Vector3 tmp;

	int bhvChange = 0,actChange = 0;

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
		
		Ogre::Vector3 min,max,mtmp;

		switch(behavior)
		{
		case AI::BHV_IDLE:
			_behaviorIdle();
			break;
		case AI::BHV_WANDER:
			lua_pushstring(lua,"bhvmin");
			
			lua_gettable(lua,1);
			if(lua_istable(lua,-1))
			{
				//this works.
				for(int i = 0; i < 3; i++)
				{
					lua_pushnumber(lua,i+1);
					lua_gettable(lua,-2);
					mtmp[i] = static_cast<float>(lua_tonumber(lua,-1));
					lua_pop(lua,1);
					
				}
			}
			lua_pop(lua,1);
			if(mtmp != Ogre::Vector3(-1000,-1000,-1000))
			{
				min = mtmp;
			} else { min = _destination; }

			lua_pushstring(lua,"bhvmax");
			lua_gettable(lua,1);
			if(lua_istable(lua,-1))
			{
				for(int i = 0; i < 3; ++i)
				{
					lua_pushnumber(lua,i+1);
					lua_gettable(lua,-2);
					mtmp[i] = static_cast<float>(lua_tonumber(lua,-1));
					lua_pop(lua,1);
				}
			}
			lua_pop(lua,1);

			if(mtmp != Ogre::Vector3(-1000,-1000,-1000))
			{
				max = mtmp;
			} else { max = _destination; }

			_behaviorWander(min,max);
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
					tmp[i] = static_cast<float>(lua_tonumber(lua,-1));
					lua_pop(lua,1);
					
				}
			}
			lua_pop(lua,1);

			if(tmp != Ogre::Vector3(-1000,-1000,-1000))
			{
				moveTarget = tmp;
			}
			else
			{
				moveTarget = _destination;
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
		case AI::ACT_IDLE:
			_actionIdle();
			break;
		case AI::ACT_LOOKAT:
			lua_pushstring(lua,"lookat");
			lua_gettable(lua,1);
			Ogre::Vector3 tmp;
			if(lua_istable(lua,-1))
			{
				//this works.
				for(int i = 0; i < 3; i++)
				{
					lua_pushnumber(lua,i+1);
					lua_gettable(lua,-2);
					tmp[i] = static_cast<float>(lua_tonumber(lua,-1));
					lua_pop(lua,1);
				}
			}
			lua_pop(lua,1);

			if(tmp != Ogre::Vector3(-1000,-1000,-1000))
			{
				_actionLook(tmp);
			}
			else
			{
				_actionIdle();
			}
			break;
		}
		
	}

	//Just in case I missed something up there(missing lua_pop,etc)
	lua_settop(lua,0);

	//std::cout << behavior << "," << action << std::endl;

	_prevBhv = behavior;
	_prevAct = action;

	//lastly, update the animations
	//something is funky with FPSC character models.
	//Will probably have to get custom ones somewhere.
	_animHandler.addTime(deltaTimeInMilliSecs / 1000.0f);
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

	_isBhvFinished = true;

	//transition to idle animation
	_animHandler.blend("Idle",AnimationBlender::BlendWhileAnimating,.2f,true);
}

void NPCCharacter::_behaviorMove(const Ogre::Vector3& target)
{
	//Check for duplicate move calls and update lua function call with that info
	//std::cout << target << std::endl;
	if(_destination.squaredDistance(target) >= 6)
	{
		updateDestination(target,false);
		_destination = target;
	}

	if(destinationReached())
	{
		_isBhvFinished = true;
	}
	else
	{
		_isBhvFinished = false;
	}
	

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
		if(_animHandler.getSource() != nullptr)
		{
			Ogre::AnimationState* target = _animHandler.getTarget();
			//this relies on the properties of the '&&' construct used by C++(if the first is false,
			//then the if-statement is false. It DOESN'T check both fields.
			if((target == nullptr && _animHandler.getSource()->getAnimationName() != "Walk") || 
				(target != nullptr && target->getAnimationName() != "Walk"))
			{
				_animHandler.blend("Walk",AnimationBlender::BlendWhileAnimating,.2f,true);
			}
		}
		
	}
}

void NPCCharacter::_behaviorWander(Ogre::Vector3& min,Ogre::Vector3& max)
{
	// don't want to override an earlier target.
	if(min == _destination || max == _destination)
	{
		_behaviorMove(_destination);
		return;
	}
	//pick a random point within the box defined by min and max and go there.
	Utility::fixMinMax(min,max); //just to ensure that I won't get an assert.
	Ogre::AxisAlignedBox box(min,max);
	
	//generate a random point
	Ogre::Vector3 point;
	point.x = Ogre::Math::RangeRandom(min.x,max.x);
	point.y = (min.y + max.y) / 2.0f; //average of the two heights(really ought to be the same lol).
	point.z = Ogre::Math::RangeRandom(min.z,max.z);

	if(box.intersects(point))
	{
		_behaviorMove(point);
	}
	else
	{
		_isBhvFinished = false;
		return;
	}
	
	//If the bhvFinished is set, then I think that's it. Sweet!

	return;
}

//Probably the most involved behavior so far.
void NPCCharacter::_behaviorTalk(const std::string& targetName)
{
	//have to start moving to the target entity until the distance is acceptable
	LevelData::BaseEntity* targetEnt = LuaManager::getSingleton().getEntity(targetName);

	//Can't talk to a trigger zone or door(though I *could* do the door, it would be an easter egg).
	if(targetEnt->getType() != LevelData::NPC && targetEnt->getType() != LevelData::ENEMY)
	{
		_isBhvFinished = true;
		return;
	}

	if(targetEnt->getType() == LevelData::NPC)
	{
		NPCCharacter* targetNpc = static_cast<NPCCharacter*>(targetEnt);

		//choosing to ignore the y-value to get a true 4 unit distance horizontally from the target.
		//might remove if it doesn't make a difference.
		Ogre::Vector3 tgtNpc = targetNpc->getPosition(); tgtNpc.y = 0;
		Ogre::Vector3 pos = getPosition(); pos.y = 0;

		//position has to be less than 16 units to talk to the entity.
		//chose to use 18 instead due to float inaccuracies(might have a value like 16.0178)
		if(tgtNpc.squaredDistance(pos) > 18)
		{
			//move towards the entity(not using the _behaviorMove function though)
			//or can I??
			//yes I can actually. *does a jig*
			//generate a more correct position
			Ogre::Vector3 target,tmp;
			tmp = targetNpc->getPosition() - getPosition(); //distance and direction to the target
			tmp.y = 0;
			Ogre::Real len = tmp.normalise();
			//original + (unit direction vector * desired distance from target[non-squared])
			target = getPosition() + (tmp * ( len - 4));

			_behaviorMove(target);

			if(_isBhvFinished)
			{
				//reached the destination, but behavior isn't done yet.
				_isBhvFinished = false;
			}
		}
		else
		{
			//rotate to face the target
			Ogre::Vector3 dir = tgtNpc - pos; dir.normalise();
			Ogre::Vector3 src = _node->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
			src.y = 0;

			_node->rotate(src.getRotationTo(dir));

			//make sure any other blends are finished first.
			if(_animHandler.getTarget() == nullptr)
			{
				//this ends the behavior until the talk animation or sound file is implemented
				_isBhvFinished = true;

				//this would be the actual way to finish the behavior(or even with the end of a sound file)
				if(_animHandler.getSource()->getAnimationName() == "Talk")
				{
					_isBhvFinished = true;
				}
				else
				{
					//start the blend to the talk animation.
					//_animHandler.blend([animation],[blend technique],.2,true);
				}
			}
		}

		return;
	}

	if(targetEnt->getType() == LevelData::ENEMY)
	{
		//nothing for now.

		return;
	}


	return;
}

void NPCCharacter::_behaviorFollow(const std::string& targetName)
{
	LevelData::BaseEntity* ent = LuaManager::getSingleton().getEntity(targetName);
	if(ent == nullptr)
	{
		_isBhvFinished = true;
		return;
	}

	if(ent->getType() == LevelData::NPC)
	{
		NPCCharacter* npc = static_cast<NPCCharacter*>(ent);
		Ogre::Vector3 pos = getPosition(),npcPos = npc->getPosition();
		pos.y = 0; npcPos.y = 0;
		Ogre::Vector3 diff = npcPos - pos;
		float len = diff.normalise();
		if(len > 3)
		{
			//new position = old position + (direction vector * (length - distance from target))
			pos = pos + (diff * (len - 4.0));
			_behaviorMove(pos);
		}
		else
		{
			_isBhvFinished = true;
		}

	}

}

//This is basically just a time-waster
void NPCCharacter::_actionIdle()
{
	//want the head to point straight ahead when it's idling
	Ogre::Bone* headBone;
	Ogre::Skeleton* skel = static_cast<Ogre::Entity*>(_movableObject)->getSkeleton();
	headBone = skel->getBone("Bip01_Head");
	if(headBone->isManuallyControlled())
	{
		//rotate it to be aligned with the body.

	}

	_isActFinished = true;
}

void NPCCharacter::_actionLook(const Ogre::Vector3& target)
{
	Ogre::Bone* headBone;
	std::string n = _node->getName();
	Ogre::Skeleton* skel = static_cast<Ogre::Entity*>(_movableObject)->getSkeleton();
	headBone = skel->getBone("Bip01_Head");
	headBone->setManuallyControlled(true);
	headBone->setInheritOrientation(true);
	int nAnim = skel->getNumAnimations();
	for(int i = 0; i < nAnim; ++i)
	{
		skel->getAnimation(i)->destroyNodeTrack(headBone->getHandle());
	}

	Ogre::Vector3 test = headBone->_getDerivedPosition() * .1f + _node->getPosition();
	Ogre::Vector3 dir = target - test;
	Ogre::Quaternion nodeRot,boneRot;
	boneRot = headBone->_getDerivedOrientation();
	Ogre::Vector3 boneTest = boneRot * Ogre::Vector3::UNIT_Z;
	dir.y = 0;
	dir.normalise();

	headBone->rotate(boneTest.getRotationTo(dir),Ogre::Node::TS_WORLD);

	_isActFinished = true;
}