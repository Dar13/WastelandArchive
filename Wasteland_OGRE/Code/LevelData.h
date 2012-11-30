#include "StdAfx.h"

#ifndef _LEVELDATA_H_
#define _LEVELDATA_H_

#include "GameManager.h"

//System to hold data for current level such as triggerzones and light positions.
namespace LevelData
{
	enum ENTITY_TYPE
	{
		NONE = 0,
		LIGHT,
		TRIGGERZONE,
		DOOR,
		WAYPOINT,
		NPC,
		ENEMY
	};
	//Base entity class, allows some pretty cool things further on
	class BaseEntity
	{
	public:
		BaseEntity(bool active,int type) : _activated(active),_type(type) {}

		void setType(int entType);
		int getType();

		void setName(const std::string& name);
		std::string getName();

		void setScriptFunction(const std::string& scriptFunc);
		std::string getScriptFunction();

		void activate(bool active);
	protected:
		int _type;
		bool _activated;
		std::string _name;
		std::string _scriptName;
	};

	//TRIGGER ZONE STRUCTS/CLASSES/STRUCTS/FUNCTIONS
	enum TRIGGER_TYPE
	{
		PLAYER = 1,
		ENTITY,
		TIME,
		GLOBAL
	};
	class TriggerZone : public BaseEntity
	{
	public:
		TriggerZone() : BaseEntity(false,TRIGGERZONE)
		{
			_triggered = false;
			_triggerType = 0;
			_triggerInZone = false;
		}
		TriggerZone(std::string scriptName,bool activated = false) : BaseEntity(activated,TRIGGERZONE)
		{
			_scriptName = scriptName;
			_triggered = false;
			_triggerType = 0;
			_triggerInZone = false;
		}

		virtual void update(OgreTransform& playerTransform, int deltaTimeInMs) {}

		void setTriggerType(TRIGGER_TYPE type);
		int getTriggerType();

	protected:
		bool _triggered;
		bool _triggerInZone;
		int _triggerType;
	};

	class GlobalTrigger : public TriggerZone
	{
	public:
		GlobalTrigger() {}
		GlobalTrigger(std::string scriptName,bool contExec = false,bool activated = false)
			: TriggerZone(scriptName,activated),
			  _continuousExecution(contExec)
		{ _triggerType = GLOBAL; }

		virtual void update(OgreTransform& playerTransform,int deltaTimeInMs);

		void setContinuousExecution(bool continuous) { _continuousExecution = continuous; }

	private:
		bool _continuousExecution;
	};

	class PlayerTrigger : public TriggerZone
	{
	public:
		PlayerTrigger() {}
		PlayerTrigger(std::string scriptName, Ogre::AxisAlignedBox boundaries,bool activated = false)
			: TriggerZone(scriptName,activated),
			  _boundaries(boundaries)
		{ _triggerType = PLAYER; }

		virtual void update(OgreTransform& playerTransform, int deltaTimeInMs);
		bool check(const OgreTransform& playerTrans);

		void setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries);
	private:
		Ogre::AxisAlignedBox  _boundaries;
	};

	class EntityTrigger : public TriggerZone
	{
	public:
		EntityTrigger() {}
		EntityTrigger(std::string scriptName,std::string& targetName, Ogre::AxisAlignedBox boundaries,Ogre::SceneNode* rootNode, bool activated = false)
			: TriggerZone(scriptName,activated),
			  _target(targetName)
		{
			setTriggerTargetNode(rootNode);
			_triggerType = ENTITY;
		}

		void setTriggerTarget(const std::string& targetName);
		void setTriggerTargetNode(Ogre::SceneNode* rootNode);

		virtual void update(OgreTransform& playerTransform,int deltaTimeInMs);

		bool check(const Ogre::Vector3& position);

		void setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries);
	private:
		std::string _target;
		Ogre::SceneNode* _targetNode;
		Ogre::AxisAlignedBox  _boundaries;
	};

	class TimeTrigger : public TriggerZone
	{
	public:
		TimeTrigger() {}
		TimeTrigger(std::string scriptName, int timeDelay, bool activated = false)
			: TriggerZone(scriptName,activated),
			  _timeDelay(timeDelay),
			  _currentTime(0),
			  _goalTime(0),
			  _timeActivated(false)
		{ _triggerType = TIME; }

		void setTimeDelay(int milliSecs);

		virtual void update(OgreTransform& playerTransform,int deltaTimeInMs);

		bool check(int time_ms);
	private:
		int _currentTime;
		int _goalTime;
		int _timeDelay;
		bool _timeActivated;
	};

	//LIGHT STRUCTS/CLASSES/ENUMS/FUNCTIONS/etc
	//no use creating new wrapper class, Ogre::Light does same thing
	class LightData : public BaseEntity
	{
	public:
		LightData() : BaseEntity(false,LIGHT),_light(0) {}
		
		void update();

		void setLightType(int type);
		int getLightType();

		void setRange(float range);
		float getRange();

		void setDiffuseColour(const Ogre::ColourValue& diffuseColour);
		Ogre::ColourValue getDiffuseColour();

		void setSpecularColour(const Ogre::ColourValue& specularColour);
		Ogre::ColourValue getSpecularColour();
	protected:
		Ogre::Light* _light;

		int _lightType;
		float _range;
		Ogre::ColourValue _diffColour;
		Ogre::ColourValue _specColour;
	};

	class SpotLightData : public LightData
	{
	public:
		void createLight(Ogre::SceneManager* scene,GraphicsManager* g);

		void setAngles(float innerAngle,float outerAngle);

		void setDirection(const Ogre::Vector3& direction);
		Ogre::Vector3 getDirection();

		void setPosition(const Ogre::Vector3& position);
		Ogre::Vector3 getPosition();
	private:
		float _innerAngle,_outerAngle;
		Ogre::Vector3 _direction;
		Ogre::Vector3 _position;
	};

	class DirectionalLightData : public LightData
	{
	public:
		void createLight(Ogre::SceneManager* scene,GraphicsManager* g);

		void setDirection(const Ogre::Vector3& direction);
		Ogre::Vector3 getDirection();
	private:
		Ogre::Vector3 _direction;
	};

	class PointLightData : public LightData
	{
	public:
		void createLight(Ogre::SceneManager* scene,GraphicsManager* g);

		void setPosition(const Ogre::Vector3& position);
		Ogre::Vector3 getPosition();
	private:
		Ogre::Vector3 _position;
	};

	//DOORS STRUCTS/CLASSES/ENUMS/FUNCTIONS/etc
	class DoorData : public BaseEntity
	{
	public:
		DoorData() : BaseEntity(false,DOOR),_hinge(nullptr) {}

		void createDoor(Ogre::SceneManager* scene,GraphicsManager* g,PhysicsManager* p,OgreBulletPair* staticLevel);

		void update();

		void setScriptName(const std::string& scriptName);
		std::string getScriptName();

		void setObjectFile(const std::string& fileName);
		
		void setPosition(const Ogre::Vector3& position);
		Ogre::Vector3 getPosition();

		void setAxis(const Ogre::Vector3& axis);
		Ogre::Vector3 getAxis();

		void setDirection(const Ogre::Vector3& direction);
		Ogre::Vector3 getDirection();

		void setConnectionPoint(const Ogre::Vector3& connection);
		Ogre::Vector3 getConnectionPoint();

		void setMinAngle(float angle);
		float getMinAngle();

		void setMaxAngle(float angle);
		float getMaxAngle();

	private:
		std::string _objectFile;
		
		Ogre::Vector3 _position;
		Ogre::Vector3 _direction,_axis;
		Ogre::Vector3 _connectionPoint;
		float _minAngle;
		float _maxAngle;

		btHingeConstraint* _hinge;
		OgreBulletPair _door;
	};

	//WAYPOINT CLASSES/ENUMS/STRUCTS/ETC
	class Waypoint : public BaseEntity
	{
	public:
		Waypoint() : BaseEntity(false,WAYPOINT),_position(Ogre::Vector3::ZERO) {}

		~Waypoint() {}

		Ogre::Vector3& getPosition() { return _position; }
		void setPosition(const Ogre::Vector3& position) { _position = position; }

		void setOrder(int order) { _order = order; }
		int getOrder() { return _order; }

	protected:
		Ogre::Vector3 _position;
		int _order;
	};

	class WaypointSet
	{
	public:
		WaypointSet() : _currentWaypoint(0),_sorted(false),_finalized(false),_finished(false) {}
		WaypointSet(std::vector<Waypoint> waypoints,bool sortByOrder = true);

		float getProgress() 
		{ 
			return (static_cast<float>(_currentWaypoint) / static_cast<float>(_waypoints[_waypoints.size() - 1].getOrder()));
		}

		bool isFinished() { return _finished; }

		int getCurrentWaypoint() { return _currentWaypoint; }

		void addWaypoint(const Waypoint& waypoint,bool sort = false);
		void finalizeSet();

		void updateProgress(const Ogre::Vector3& position);

		Ogre::Vector3 getTargetPosition() { return _waypoints[_currentWaypoint].getPosition(); }

		Ogre::SimpleSpline generateSpline();

	private:
		void _sort();
		bool _finalized;
		bool _finished;
		bool _sorted;
		std::vector<Waypoint> _waypoints;
		int _currentWaypoint;
	};

	//Actual LevelParser.
	//Used to parse level data contained in file.
	class LevelParser
	{
	public:
		void setFile(const char* fileName);

		void parseTriggers(std::vector<std::unique_ptr<TriggerZone>>* triggers,Ogre::SceneNode* rootNode);

		void parseLights(std::vector<std::unique_ptr<LightData>>* lights);

		void parseDoors(std::vector<std::unique_ptr<DoorData>>* doors);

		void parseWaypoints(std::vector<Waypoint>* waypoints);
	private:
		std::string _file;
	};
};

#endif