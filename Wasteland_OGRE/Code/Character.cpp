#include "StdAfx.h"

#include "Character.h"

Character::Character(const std::string& name,Ogre::SceneManager* scene,CrowdManager* crowd,const Ogre::Vector3& position)
{
	//won't use this often.
}

Character::Character(Ogre::SceneNode* node,CrowdManager* crowd,const Ogre::Vector3& position)
	: _node(nullptr),
	  _entity(nullptr),
	  _agentID(-1),
	  _agent(nullptr),
	  _isStopped(false),
	  _isAgentControlled(true),
	  _crowd(crowd)
{
	
}