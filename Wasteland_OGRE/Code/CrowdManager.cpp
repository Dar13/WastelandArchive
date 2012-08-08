#include "StdAfx.h"

#include "CrowdManager.h"
#include "Utility.h"

CrowdManager::CrowdManager(DetourInterface* detour,rcdtConfig* config)
	: _crowd(nullptr),
	  _detour(detour),
	  _targetRef(0),
	  _activeAgents(0),
	  _anticipateTurns(true),
	  _optimizeVis(true),
	  _optimizeTopo(true),
	  _obstacleAvoidance(true),
	  _separation(false),
	  _separationWeight(2.0f),
	  _config(config)
{
	_crowd = dtAllocCrowd();
	if(!_crowd)
	{
		std::cout << "Detour: Couldn't allocate crowd instance." << std::endl;
		Ogre::LogManager::getSingleton().logMessage("Detour: Could not allocate crowd instance.");
	}

	memset(_trails,0,sizeof(_trails));

	dtNavMesh* nav = detour->getNavMesh();
	dtCrowd* crowd = _crowd;
	if(nav && crowd && crowd->getAgentCount() == 0)
	{
		crowd->init(MAX_AGENTS,config->recastConfig->walkableRadius,nav);

		//excludes disabled polygons.
		crowd->getEditableFilter()->setExcludeFlags(0x10);

		dtObstacleAvoidanceParams params;
		memcpy(&params,crowd->getObstacleAvoidanceParams(0),sizeof(dtObstacleAvoidanceParams));

		// Low (11)
        params.velBias = 0.5f;
        params.adaptiveDivs = 5;
        params.adaptiveRings = 2;
        params.adaptiveDepth = 1;
        crowd->setObstacleAvoidanceParams(0, &params);

        // Medium (22)
        params.velBias = 0.5f;
        params.adaptiveDivs = 5;
        params.adaptiveRings = 2;
        params.adaptiveDepth = 2;
        crowd->setObstacleAvoidanceParams(1, &params);

        // Good (45)
        params.velBias = 0.5f;
        params.adaptiveDivs = 7;
        params.adaptiveRings = 2;
        params.adaptiveDepth = 3;
        crowd->setObstacleAvoidanceParams(2, &params);

        // High (66)
        params.velBias = 0.5f;
        params.adaptiveDivs = 7;
        params.adaptiveRings = 3;
        params.adaptiveDepth = 3;
		crowd->setObstacleAvoidanceParams(3,&params);
	}
}

CrowdManager::~CrowdManager()
{
	dtFreeCrowd(_crowd);
	//dtFreeObstacleAvoidanceDebugData(_vod);
}

int CrowdManager::addAgent(const Ogre::Vector3& position)
{
	dtCrowdAgentParams ap;
	memset(&ap,0,sizeof(ap));
	ap.radius = _config->userConfig->getAgentRadius();
	ap.height = _config->userConfig->getAgentHeight();
	ap.maxAcceleration = 8.0f;
	ap.maxSpeed = (ap.height / 2.0f) * 1.5f;
	ap.collisionQueryRange = ap.radius * 12.0f;
	ap.pathOptimizationRange = ap.radius * 30.0f;

	ap.updateFlags = 0;
	if(_anticipateTurns)
	{
		ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;
	}
	if(_optimizeVis)
	{
		ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;
	}
	if(_optimizeTopo)
	{
		ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;
	}
	if(_obstacleAvoidance)
	{
		ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;
	}
	if(_separation)
	{
		ap.updateFlags |= DT_CROWD_SEPARATION;
	}
	//ap.obstacleAvoidanceType = static_cast<unsigned char>(_obstacleAvoidanceType);
	ap.obstacleAvoidanceType = static_cast<unsigned char>(3.0f);

	float p[3];
	Utility::vector3_toFloatPtr(position,p);
	int id = _crowd->addAgent(p,&ap);
	if(id != -1)
	{
		if(_targetRef)
		{
			_crowd->requestMoveTarget(id,_targetRef,_targetPosition);
		}

		AgentTrail* trail = &_trails[id];
		for(int i = 0; i < AGENT_MAX_TRAIL_LENGTH; ++i)
		{
			dtVcopy(&trail->trail[i * 3],p);
		}
		trail->htrail = 0;
	}

	_activeAgents++;

	return id;
}

const dtCrowdAgent* CrowdManager::getAgent(int id)
{
	return _crowd->getAgent(id);
}

void CrowdManager::removeAgent(int id)
{
	_crowd->removeAgent(id);
	_activeAgents--;
}

void CrowdManager::setMoveTarget(const Ogre::Vector3& position,bool adjust,int agentID)
{
	dtNavMeshQuery* navQuery = _detour->getNavQuery();
	const dtQueryFilter* filter = _crowd->getFilter();
	const float* extents = _crowd->getQueryExtents();
	float pos[3];
	Utility::vector3_toFloatPtr(position,pos);

	navQuery->findNearestPoly(pos,extents,filter,&_targetRef,_targetPosition);

	int start = 0,end = _crowd->getAgentCount();
	if(agentID != -1) { start = agentID; end = agentID + 1; }

	if(adjust)
	{
		for(int i = start; i < end; ++i)
		{
			const dtCrowdAgent* agent = _crowd->getAgent(i);
			if(!agent->active) continue;
			float velocity[3];
			calculateVelocity(velocity,pos,_targetPosition,agent->params.maxSpeed);
			_crowd->requestMoveVelocity(i,velocity);
		}
	}
	else
	{
		for(int i = start; i < end; ++i)
		{
			const dtCrowdAgent* agent = _crowd->getAgent(i);
			if(!agent->active) continue;
			_crowd->requestMoveTarget(i,_targetRef,_targetPosition);
		}
	}
}