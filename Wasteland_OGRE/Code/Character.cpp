#include "StdAfx.h"

#include "Character.h"
#include "Utility.h"

Character::Character(Ogre::SceneManager* scene,CrowdManager* crowd,const Ogre::Vector3& position)
{
	//won't use this often.
}

Character::Character()
	: _node(nullptr),
	  _movableObject(nullptr),
	  _agentID(-1),
	  _agent(nullptr),
	  _isStopped(false),
	  _isAgentControlled(true),
	  _crowd(nullptr),
	  _destination(0.0f),
	  _manualVelocity(0.0f),
	  _destRadius(0.0f)
{}

Character::Character(Ogre::SceneNode* node,CrowdManager* crowd,const Ogre::Vector3& position)
	: _node(node),
	  _movableObject(nullptr),
	  _agentID(-1),
	  _agent(nullptr),
	  _isStopped(false),
	  _isAgentControlled(true),
	  _crowd(crowd)
{
	_agentID = _crowd->addAgent(position);
	_agent = _crowd->getAgent(_agentID);

	//assumes at least one attached entity/light/etc
	_movableObject = node->getAttachedObject(0);
}

//!Finds closest point on navmesh and updates destination of agent to it.
void Character::updateDestination(const Ogre::Vector3& destination,bool updatePrevPath)
{
	if(!_isAgentControlled)
	{
		return;
	}

	Ogre::Vector3 result;
	if(!_crowd->_getDetour()->findNearestPointOnNavmesh(destination,result))
	{
		return;
	}

	_crowd->setMoveTarget(result,updatePrevPath,_agentID);
	_destination = destination;
	_isStopped = false;
	_manualVelocity = Ogre::Vector3::ZERO;
}

void Character::setPosition(const Ogre::Vector3& position)
{
	if(!_isAgentControlled)
	{
		_node->setPosition(position);
		return;
	}
	
	Ogre::Vector3 result;
	if(!_crowd->_getDetour()->findNearestPointOnNavmesh(position,result))
	{
		return;
	}

	_crowd->removeAgent(_agentID);
	_agentID = _crowd->addAgent(result);

	_node->setPosition(result);
}

Ogre::Vector3 Character::getPosition()
{
	return _node->getPosition();
}

bool Character::destinationReached()
{
	return (_node->getPosition().squaredDistance(getDestination()) <= _destRadius);
}

void Character::setVelocity(const Ogre::Vector3& velocity)
{
	_manualVelocity = velocity;
	_isStopped = false;
	_destination = Ogre::Vector3::ZERO;

	if(_isAgentControlled)
	{
		_crowd->requestVelocity(_agentID,_manualVelocity);
	}
}

Ogre::Vector3 Character::getVelocity()
{
	if(_isAgentControlled)
	{
		Ogre::Vector3 vel;
		Utility::floatPtr_toVector3(_agent->nvel,vel);
		return vel;
	}
	else
	{
		return _manualVelocity;
	}
}

void Character::stop()
{
	if(_isAgentControlled && _crowd->stopAgent(_agentID))
	{
		_destination = Ogre::Vector3::ZERO;
	}
	
	_manualVelocity = Ogre::Vector3::ZERO;
	_isStopped = true;
	return;
}

float Character::getSpeed()
{
	return getVelocity().length();
}

float Character::getMaxSpeed()
{
	return _agent->params.maxSpeed;
}

float Character::getMaxAcceleration()
{
	return _agent->params.maxAcceleration;
}

bool Character::isMoving()
{
	return !_isStopped || getSpeed() != 0;
}

Ogre::Vector3 Character::getLookingDirection()
{
	return _node->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
}

void Character::setAgentControlled(bool agentControlled)
{
	if(_isAgentControlled != agentControlled)
	{
		if(agentControlled)
		{
			_agentID = _crowd->addAgent(_node->getPosition());
			_agent = _crowd->getAgent(_agentID);
			_destination = _crowd->getLastDestination();
			_manualVelocity = Ogre::Vector3::ZERO;
			_isStopped = true;
		}
		else
		{
			_crowd->removeAgent(_agentID);
			_agentID = -1;
			_destination = Ogre::Vector3::ZERO;
			_isStopped = false;
		}
		_isAgentControlled = agentControlled;
	}
	return;
}

void Character::updatePosition(float deltaTime)
{
	if(_isAgentControlled)
	{
		Ogre::Vector3 agentPosition;
		Utility::floatPtr_toVector3(_agent->npos,agentPosition);
	}
	else
	{
		if(getVelocity().isZeroLength())
		{
			return;
		}

		Ogre::Vector3 testPosition = getPosition() + deltaTime * getVelocity();
		if(_crowd->_getDetour()->findNearestPointOnNavmesh(testPosition,testPosition))
		{
			_node->setPosition(testPosition);
		}
	}
}

void Character::setDestination(const Ogre::Vector3& destination)
{
	if(!_isAgentControlled)
	{
		return;
	}

	_destination = destination;
	_manualVelocity = Ogre::Vector3::ZERO;
	_isStopped = false;
}