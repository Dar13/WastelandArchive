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
	};
	//Base entity class, allows some pretty cool things further on
	class BaseEntity
	{
	public:
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
	};
	class TriggerZone : public BaseEntity
	{
	public:
		TriggerZone();
		TriggerZone(const Ogre::AxisAlignedBox& zoneBoundaries,TRIGGER_TYPE triggerType);//,(callback code));

		void setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries);
		void setTriggerType(TRIGGER_TYPE type);
		int getTriggerType();

		void setScriptName(const std::string& script);

	protected:
		Ogre::AxisAlignedBox  _boundaries;

		bool _triggered;
		bool _triggerInZone;
		int _triggerType;
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

		void update(int time_ms);

		bool check(int time_ms);
	private:
		int _startTime;
		int _goalTime;
		bool _timeActivated;
	};

	//LIGHT STRUCTS/CLASSES/ENUMS/FUNCTIONS/etc
	//no use creating new wrapper class, Ogre::Light does same thing
	class LightData : public BaseEntity
	{
	public:
		void setLightType(int type);
		int getLightType();

		void setRange(float range);
		float getRange();

		void setDiffuseColour(const Ogre::ColourValue& diffuseColour);
		Ogre::ColourValue getDiffuseColour();

		void setSpecularColour(const Ogre::ColourValue& specularColour);
		Ogre::ColourValue getSpecularColour();
	protected:
		int _lightType;
		float _range;
		Ogre::ColourValue _diffColour;
		Ogre::ColourValue _specColour;
	};

	class SpotLightData : public LightData
	{
	public:
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
		void setDirection(const Ogre::Vector3& direction);
		Ogre::Vector3 getDirection();
	private:
		Ogre::Vector3 direction;
	};

	class PointLightData : public LightData
	{
	public:
		void setPosition(const Ogre::Vector3& position);
		Ogre::Vector3 getPosition();
	private:
		Ogre::Vector3 position;
	};

	//DOORS STRUCTS/CLASSES/ENUMS/FUNCTIONS/etc
	class DoorData : public BaseEntity
	{
	public:
		DoorData();

		void createDoor(GraphicsManager* g,PhysicsManager* p);

		void update();

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
		std::string _name;
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