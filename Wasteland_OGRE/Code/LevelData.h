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

	//Base TriggerZone class
	class TriggerZone : public BaseEntity
	{
	public:
		TriggerZone(std::string& scriptName) : BaseEntity(false,TRIGGERZONE)
		{
			_triggered = false;
			_triggerType = 0;
			_triggerInZone = false;
			_scriptName = scriptName;
		}

		virtual void update(OgreTransform* playerTransformation, int deltaTimeInMs) {}

	protected:
		bool _triggered;
		bool _triggerInZone;
		int _triggerType;
	};

	//Derived TriggerZone class, allows for global scripts
	//Used primarily for activating different bunches of scripts after activation.
	class GlobalTrigger : public TriggerZone
	{
	public:
		GlobalTrigger(std::string& scriptName,bool activated = false)
			: TriggerZone(scriptName)
		{}

		virtual void update(OgreTransform* playerTransformation,int deltaTimeInMs);

		void setContinuousExecution(bool continuous) { _continuousExecution = continuous; }
	private:
		bool _continuousExecution;
		//do I even need anything here?
	};

	//Derived TriggerZone class, handles player walking into a defined zone.
	class PlayerTrigger : public TriggerZone
	{
	public:
		PlayerTrigger(std::string& scriptName,Ogre::AxisAlignedBox boundaries,bool activated = false)
			: TriggerZone(scriptName),
			  _boundaries(boundaries)
		{
		}

		virtual void update(OgreTransform* playerTransformation, int deltaTimeInMs);

		bool check(const OgreTransform& playerTrans);
		
		void setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries);
	private:
		Ogre::AxisAlignedBox  _boundaries;
	};

	//Derived TriggerZone class, handles entity being in a defined zone.
	class EntityTrigger : public TriggerZone
	{
	public:
		EntityTrigger(std::string& targetName,Ogre::AxisAlignedBox box,std::string& scriptName,bool activated = false)
			: TriggerZone(scriptName),
			  _boundaries(box),
			  _target(targetName)
		{
		}
		void setTriggerTarget(const std::string& targetName);
		void setTriggerTargetNode(Ogre::SceneNode* rootNode);

		virtual void update(OgreTransform* playerTransformation, int deltaTimeInMs);

		bool check(const Ogre::Vector3& position);

		void setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries);
	private:
		Ogre::AxisAlignedBox _boundaries;
		std::string _target;
		Ogre::SceneNode* _targetNode;
	};

	//Derived TriggerZone class, handles a timer.
	class TimeTrigger : public TriggerZone
	{
	public:
		TimeTrigger(std::string& scriptName, int milliSecs, bool activated = false)
			: TriggerZone(scriptName),
			  _timeDelay(milliSecs),
			  _currentTime(0),
			  _goalTime(0),
			  _timeActivated(false)
		{
		}
		void setTimeDelay(int milliSecs);

		virtual void update(OgreTransform* playerTransformation,int deltaTimeInMs);

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
		
		virtual void update();

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

		void parseTriggers(std::vector<std::unique_ptr<TriggerZone>>* triggers, Ogre::SceneNode* rootNode);

		void parseLights(std::vector<std::unique_ptr<LightData>>* lights);

		void parseDoors(std::vector<std::unique_ptr<DoorData>>* doors);

		void parseWaypoints(std::vector<Waypoint>* waypoints);
	private:
		std::string _file;
	};
};

#endif