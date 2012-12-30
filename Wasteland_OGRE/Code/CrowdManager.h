#include "StdAfx.h"

#include "DetourInterface.h"

#ifndef _CROWD_MANAGER_H_
#define _CROWD_MANAGER_H_

#define AGENT_MAX_TRAIL_LENGTH 64
#define MAX_AGENTS 128
#define DEFAULT_MAXSPEED 1.5f

class CrowdManager
{
public:
	CrowdManager(DetourInterface* detour,rcdtConfig* config);
	~CrowdManager();

	int addAgent(const Ogre::Vector3& position);
	int addAgent(const Ogre::Vector3& position, float maxSpeedFactor);

	const dtCrowdAgent* getAgent(int id);

	void removeAgent(int id);

	//Sets the move target either for the entire crowd or for an individual agent.
	//Defaults to entire crowd.
	void setMoveTarget(const Ogre::Vector3& position,bool adjust = true,int agentID = -1);

	//requests that the velocity of a specific character be changed.
	bool requestVelocity(int agentID, const Ogre::Vector3& velocity);

	//cancels any queued requests to the agent, causing it to stop moving.
	bool stopAgent(int agentID);

	//position = current position
	//target = target destination
	//speed = maximum speed the agent can travel at
	static Ogre::Vector3 calculateVelocity(const Ogre::Vector3& position, const Ogre::Vector3& target, float speed);
	static void calculateVelocity(float* velocity,const float* position,const float* target, float speed);

	//Needs to be called every frame.
	void updateTick(const float deltaTime);

	std::vector<dtCrowdAgent*> getActiveAgents();

	Ogre::Vector3 getLastDestination();

	DetourInterface* _getDetour() { return _detour; }
private:

	dtCrowd* _crowd;
	DetourInterface* _detour;
	rcdtConfig _config;

	dtPolyRef _targetRef;
	float _targetPosition[3];

	struct AgentTrail
	{
		float trail[AGENT_MAX_TRAIL_LENGTH*3];
		int htrail;
	};
	AgentTrail _trails[MAX_AGENTS];

	bool _anticipateTurns;
	bool _optimizeVis;
	bool _optimizeTopo;
	bool _obstacleAvoidance;
	bool _separation;

	float _separationWeight;

	int _activeAgents;
};

#endif