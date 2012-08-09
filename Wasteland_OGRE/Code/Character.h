#include "StdAfx.h"

#include "CrowdManager.h"

#ifndef _CROWD_AGENT_H_
#define _CROWD_AGENT_H_

class Character
{
public:
	Character(const std::string& name, Ogre::SceneManager* scene, CrowdManager* crowd, const Ogre::Vector3& position);
	Character(Ogre::SceneNode* node,CrowdManager* crowd,const Ogre::Vector3& position);

	inline Ogre::SceneNode* getNode() { return _node; }
	inline Ogre::MovableObject* getMovableObject() { return _movableObject; }
	//inline Ogre::Entity* getEntity() { return _entity; }

	virtual void update(float deltaTime) = 0;

	inline int getAgentID() { return _agentID; }
	inline const dtCrowdAgent* getAgent() { return _agent; }

	inline Ogre::Vector3 getDestination() { if(_isAgentControlled) return _destination; else return Ogre::Vector3::ZERO; }

	virtual void setPosition(const Ogre::Vector3& position);
	virtual Ogre::Vector3 getPosition();

	//returns true if destination is reached
	virtual bool destinationReached();

	//requests certain velocity be attained by Detour agent
	void setVelocity(const Ogre::Vector3& velocity);
	
	//gets the current velocity of the associated agent.
	Ogre::Vector3 getVelocity();

	//stops the detour agent, is virtual to allow for animation and game logic stuff.
	virtual void stop();

	virtual float getSpeed();

	virtual float getMaxSpeed();

	virtual float getMaxAcceleration();

	virtual bool isMoving();

	virtual Ogre::Vector3 getLookingDirection();

	void setAgentControlled(bool agentControlled);

protected:
	virtual void updatePosition(float deltaTime);
	
	virtual void setDestination(const Ogre::Vector3& destination);

	float _destRadius;

	std::string name;
	Ogre::SceneNode* _node;
	//Ogre::Entity* _entity;
	Ogre::MovableObject* _movableObject;

	CrowdManager* _crowd;
	
	const dtCrowdAgent* _agent;
	int _agentID;

	Ogre::Vector3 _destination;

	Ogre::Vector3 _manualVelocity;

	bool _isStopped;

	bool _isAgentControlled;
	
	//Not sure how to integrate Bullet into all this. Ghost collision similar to the player character?
	//btRigidBody* _rigidBody;
};

#endif