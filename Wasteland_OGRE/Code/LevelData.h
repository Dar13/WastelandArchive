#include "StdAfx.h"

#ifndef _LEVELDATA_H_
#define _LEVELDATA_H_

//forward declarations.
struct OgreTransform;
class GraphicsManager;
class PhysicsManager;
class btHingeConstraint;
struct OgreBulletPair;

//System to hold data for current level such as triggerzones and light positions.
namespace LevelData
{
	//TRIGGER ZONE STRUCTS/CLASSES/STRUCTS/FUNCTIONS
	enum TRIGGER_TYPE
	{
		NONE = 0,
		PLAYER,
		ENTITY,
		TIME,
		TO_BE_ADDED_LATER
	};
	class TriggerZone
	{
	public:
		TriggerZone();
		TriggerZone(const Ogre::AxisAlignedBox& zoneBoundaries,TRIGGER_TYPE triggerType);//,(callback code));

		void setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries);
		void setType(TRIGGER_TYPE type);
		int getType();

		//void setCallback(...);

	protected:
		Ogre::AxisAlignedBox  _boundaries;

		bool _triggered;
		bool _triggerInZone;
		TRIGGER_TYPE _triggerType;

		//lua interface, probably a callback to a lua function.
		//callback, whatever.
		//std::string _luaFunc;
	};

	class PlayerTrigger : public TriggerZone
	{
	public:
		void update(const OgreTransform& playerTrans);
		bool check(const OgreTransform& playerTrans);
	private:

	};

	class EntityTrigger : public TriggerZone
	{
	public:
		void setTriggerTarget(const std::string& targetName);
		void setTriggerTargetNode(Ogre::SceneNode* rootNode);

		void update();

		bool check(const Ogre::Vector3& position);
	private:
		std::string _target;
		Ogre::SceneNode* _targetNode;
	};

	class TimeTrigger : public TriggerZone
	{
	public:
		void setTimeGoal(int milliSecs,int startingTime);
		
		void activate(bool activate);

		void update(int time_ms);

		bool check(int time_ms);
	private:
		int _startTime;
		int _goalTime;

		bool _activated;
	};

	//LIGHT STRUCTS/CLASSES/ENUMS/FUNCTIONS/etc
	//no use creating new wrapper class, Ogre::Light does same thing
	struct LightData
	{
		int type;
		float range;
		Ogre::ColourValue diffColour;
		Ogre::ColourValue specColour;
	};

	struct SpotLightData : public LightData
	{
		float innerAngle,outerAngle;
		Ogre::Vector3 direction;
		Ogre::Vector3 position;
	};

	struct DirectionalLightData : public LightData
	{
		Ogre::Vector3 direction;
	};

	struct PointLightData : public LightData
	{
		Ogre::Vector3 position;
	};

	//DOORS STRUCTS/CLASSES/ENUMS/FUNCTIONS/etc
	class DoorData
	{
	public:
		DoorData();

		void createDoor(GraphicsManager* g,PhysicsManager* p);

		void update();

		void activate(bool active = false);

		void setName(const std::string& name);
		std::string getName();

		void setScriptName(const std::string& scriptName);
		std::string getScriptName();

		void setObjectFile(const std::string& fileName);
		
		void setPosition(const Ogre::Vector3& position);
		Ogre::Vector3 getPosition();

		void setAxis(const Ogre::Vector3& axis);
		Ogre::Vector3 getAxis();

		void setDirection(const Ogre::Vector3& direction);
		Ogre::Vector3 getDirection();

		void setMinAngle(float angle);
		float getMinAngle();

		void setMaxAngle(float angle);
		float getMaxAngle();

	private:
		bool _activated;

		std::string _name;
		std::string _scriptName;
		std::string _objectFile;
		
		Ogre::Vector3 _position;
		Ogre::Vector3 _direction,_axis;
		Ogre::Vector3 _connectionPoint;
		float _minAngle;
		float _maxAngle;

		btHingeConstraint* _hinge;
		OgreBulletPair _door;
	};

	//Actual LevelParser.
	//Used to parse level data contained in file.
	class LevelParser
	{
	public:
		void setFile(const char* fileName);

		void parseTriggers(std::vector<std::unique_ptr<TriggerZone>>* triggers);

		void parseLights(std::vector<std::unique_ptr<LightData>>* lights);

		void parseDoors(std::vector<std::unique_ptr<DoorData>>* doors);
	private:
		std::string _file;
	};
};

#endif