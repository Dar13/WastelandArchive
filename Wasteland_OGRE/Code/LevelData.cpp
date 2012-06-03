#include"StdAfx.h"

#include "LevelData.h"
#include "LuaManager.h"
#include "Utility.h"

#include <boost\lexical_cast.hpp>

namespace LevelData
{
	//=================================
	//Base Entity class
	//=================================
	void BaseEntity::setName(const std::string& name)
	{
		_name = name;
	}
	std::string BaseEntity::getName() { return _name;}

	void BaseEntity::setType(int entType)
	{
		_type = entType;
	}
	int BaseEntity::getType() { return _type;}

	void BaseEntity::setScriptFunction(const std::string& scriptFunc)
	{
		_scriptName = scriptFunc;
	}
	std::string BaseEntity::getScriptFunction() { return _scriptName;}

	void BaseEntity::activate(bool active)
	{
		_activated = active;
	}

	//============================
	//TriggerZone, base of all other triggers
	//============================

	void TriggerZone::setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries)
	{
		_boundaries = zoneBoundaries;
	}

	void TriggerZone::setTriggerType(TRIGGER_TYPE type)
	{
		_triggerType = type;
	}
	int TriggerZone::getTriggerType(){ return _triggerType; }

	//============================
	//Player Trigger, derived from TriggerZone
	//============================
	void PlayerTrigger::update(const OgreTransform& playerTrans)
	{
		_triggered = check(playerTrans);
		if((_triggered && !_triggerInZone) || _activated)
		{
			//callback to lua or other function
			LuaManager::getSingleton().callFunction(_scriptName);
			//This function must handle lua return values

			//most scripts called by this type of triggerzone are activation scripts.
			//no need for return values

			_activated = false; //function is only called once upon activation

			if(_triggered)
				_triggerInZone = true;
		}
		else
		{
			if(!_triggered)
			{
				_triggerInZone = false;
			}
		}
	}

	bool PlayerTrigger::check(const OgreTransform& playerTrans)
	{
		if(_boundaries.intersects(playerTrans.position))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//============================
	//EntityTrigger, derived from TriggerZone
	//============================
	void EntityTrigger::setTriggerTarget(const std::string& targetName)
	{
		_target = targetName;
	}
	void EntityTrigger::setTriggerTargetNode(Ogre::SceneNode* rootNode)
	{
		_targetNode = static_cast<Ogre::SceneNode*>(rootNode->getChild(_target));
	}
	void EntityTrigger::update()
	{
		_triggered = check(_targetNode->getPosition());

		if((_triggered && !_triggerInZone) || _activated)
		{
			//call the callback
			LuaManager::getSingleton().callFunction(_scriptName);
			//handle return values from lua

			//most scripts will be activating other entities.
			//No need for return values in that case.

			_triggerInZone = true;
			_activated = false; //activation will only run the function once
		}
		else
		{
			if(!_triggered)
			{
				_triggerInZone = false;
			}
		}
	}

	bool EntityTrigger::check(const Ogre::Vector3& position)
	{
		if(_boundaries.intersects(position))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//===============================
	//TimeTrigger, derived from TriggerZone
	//===============================
	void TimeTrigger::setTimeDelay(int milliSecs)
	{
		_timeDelay = milliSecs;
	}

	void TimeTrigger::update(int time_ms)
	{
		if(_timeActivated)
		{
			_triggered = check(time_ms);
		}
		else
		{
			_triggered = false;
		}

		if(_activated && !_timeActivated)
		{
			_timeActivated = true;
			_goalTime = time_ms + _timeDelay;
			_activated = false;
		}

		if(_triggered)
		{
			//call the callback
			LuaManager::getSingleton().callFunction(_scriptName);
			//handle lua return values

			//these are one-time triggers, unless manually reset
			_activated = false;
			_triggered = false;
			_timeActivated = false;
			_goalTime = 0;
		}
	}

	bool TimeTrigger::check(int time_ms)
	{
		if(time_ms >= _goalTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//===========================================
	//Light class, handles all light aspects
	//===========================================
	void LightData::update()
	{
		if(_activated)
		{
			//should be three return values.
			/*
			boolean for visible
			3 integers for diffuse color
			3 integers for specular color
			*/
			LuaManager::getSingleton().callFunction(0,7);

			int d1,d2,d3;
			int s1,s2,s3;
			bool vis;
			lua_State* L = LuaManager::getSingleton().getLuaState();

			if(lua_isnumber(L,1))
			{
				vis = (lua_toboolean(L,1) != 0);
			}
			if(lua_isnumber(L,2))
			{
				d1 = lua_tointeger(L,2);
			}
			if(lua_isnumber(L,3))
			{
				d2 = lua_tointeger(L,3);
			}
			if(lua_isnumber(L,4))
			{
				d3 = lua_tointeger(L,4);
			}
			if(lua_isnumber(L,5))
			{
				s1 = lua_tointeger(L,5);
			}
			if(lua_isnumber(L,6))
			{
				s2 = lua_tointeger(L,6);
			}
			if(lua_isnumber(L,7))
			{
				s3 = lua_tointeger(L,7);
			}

			_diffColour.r = d1 / 255.0f;
			_diffColour.g = d2 / 255.0f;
			_diffColour.b = d3 / 255.0f;
			_specColour.r = s1 / 255.0f;
			_specColour.g = s2 / 255.0f;
			_specColour.b = s3 / 255.0f;

			_light->setVisible(vis);
			_light->setDiffuseColour(_diffColour);
			_light->setSpecularColour(_specColour);

		}
	}

	//Almost all properties are able to be changed after light creation.
	//Exceptions: range

	void LightData::setLightType(int type)
	{
		_lightType = type;
		if(_light != nullptr)
		{
			_light->setType(static_cast<Ogre::Light::LightTypes>(type));
		}
	}
	int LightData::getLightType() { return _lightType; }

	void LightData::setRange(float range)
	{
		_range = range;
	}
	float LightData::getRange() { return _range; }

	void LightData::setDiffuseColour(const Ogre::ColourValue& diffuseColour)
	{
		_diffColour = diffuseColour;
		if(_light != nullptr)
		{
			_light->setDiffuseColour(_diffColour);
		}
	}
	Ogre::ColourValue LightData::getDiffuseColour() { return _diffColour; }

	void LightData::setSpecularColour(const Ogre::ColourValue& specularColour)
	{
		_specColour = specularColour;
		if(_light != nullptr)
		{
			_light->setSpecularColour(_specColour);
		}
	}
	Ogre::ColourValue LightData::getSpecularColour() { return _specColour; }

	//Spotlight start
	void SpotLightData::createLight(Ogre::SceneManager* scene,GraphicsManager* g)
	{
		_light = scene->createLight(_name);
		_light->setType(static_cast<Ogre::Light::LightTypes>(_lightType));

		_light->setDirection(_direction);
		_light->setDiffuseColour(_diffColour);
		_light->setSpecularColour(_specColour);
		_light->setSpotlightInnerAngle(Ogre::Radian(_innerAngle));
		_light->setSpotlightOuterAngle(Ogre::Radian(_outerAngle));
		g->setLightRange(_light,_range);
	}

	void SpotLightData::setAngles(float innerAngle,float outerAngle)
	{
		_innerAngle = innerAngle;
		_outerAngle = outerAngle;
		if(_light != nullptr)
		{
			_light->setSpotlightInnerAngle(Ogre::Radian(innerAngle));
			_light->setSpotlightOuterAngle(Ogre::Radian(outerAngle));
		}
	}

	void SpotLightData::setDirection(const Ogre::Vector3& direction)
	{
		_direction = direction;
		if(_light != nullptr)
		{
			_light->setDirection(_direction);
		}
	}
	Ogre::Vector3 SpotLightData::getDirection() { return _direction;}

	void SpotLightData::setPosition(const Ogre::Vector3& position)
	{
		_position = position;
		if(_light != nullptr)
		{
			_light->setPosition(_position);
		}
	}
	Ogre::Vector3 SpotLightData::getPosition() { return _position;}

	//directional start

	void DirectionalLightData::createLight(Ogre::SceneManager* scene,GraphicsManager* g)
	{
		_light = scene->createLight(_name);
		_light->setType(static_cast<Ogre::Light::LightTypes>(_lightType));
		
		_light->setDirection(_direction);
		_light->setDiffuseColour(_diffColour);
		_light->setSpecularColour(_specColour);
		g->setLightRange(_light,_range);
	}

	void DirectionalLightData::setDirection(const Ogre::Vector3& direction)
	{
		_direction = direction;
		if(_light != nullptr)
		{
			_light->setDirection(_direction);
		}
	}
	Ogre::Vector3 DirectionalLightData::getDirection() { return _direction; }

	//point start

	void PointLightData::createLight(Ogre::SceneManager* scene,GraphicsManager* g)
	{
		_light = scene->createLight(_name);
		_light->setType(static_cast<Ogre::Light::LightTypes>(_lightType));

		_light->setPosition(_position);
		
		_light->setDiffuseColour(_diffColour);
		_light->setSpecularColour(_specColour);
		
		g->setLightRange(_light,_range);
	}

	void PointLightData::setPosition(const Ogre::Vector3& position)
	{
		_position = position;
		if(_light != nullptr)
		{
			_light->setPosition(_position);
		}
	}
	Ogre::Vector3 PointLightData::getPosition() { return _position; }

	//======================================
	//Door class, controls all the inner workings of doors.
	//======================================
	
	void DoorData::createDoor(Ogre::SceneManager* scene,GraphicsManager* g,PhysicsManager* p,OgreBulletPair* staticLevel)
	{
		//create door from internal data.
		_door = GameManager::createObject(scene,_objectFile,p,g);

		//just need to update the position
		btTransform old = _door.btBody->getWorldTransform();
		btTransform trans = old;
		btVector3 pos = Utility::convert_OgreVector3(_position);
		trans.setOrigin(pos);
		_door.btBody->setWorldTransform(trans);

		//assumes levelPair is valid pointer
		_door.btBody->setActivationState(DISABLE_DEACTIVATION);

		Ogre::AxisAlignedBox box = _door.ogreNode->getAttachedObject(0)->getBoundingBox();
		btVector3 doorConnection,axis,connectionPoint;
		axis = Utility::convert_OgreVector3(_axis);
		connectionPoint = Utility::convert_OgreVector3(_connectionPoint);
		Ogre::Matrix4 toWorld = staticLevel->ogreNode->_getFullTransform();
		//need to know where the door is pointing
		//Works, but have only tried a couple of scenarios.
		if(_direction == Ogre::Vector3::UNIT_Z || _direction == Ogre::Vector3::NEGATIVE_UNIT_Z)
		{
			//facing either z-positive or z-negative
			//shouldn't matter, but let's keep that in mind.
			//means deltaX should be longer than deltaZ(if I set up the .ent file correctly anyways).
			//getting deltaZ
			float dZ = box.getMaximum().z - box.getMinimum().z;

			//get points on side of box(x-axis)
			Ogre::Vector3 pSide,nSide;
			pSide = box.getMaximum();
			pSide.y = 0.0f;
			pSide.z = dZ/2;

			nSide = box.getMinimum();
			nSide.y = 0.0f;
			nSide.z = dZ/2;

			//compare distance of nSide/pSide to connectionPoint, after nSide and pSide are in staticLevel coordinate space
			pSide = toWorld * pSide;
			nSide = toWorld * nSide;
			if(pSide.squaredDistance(_connectionPoint) > nSide.squaredDistance(_connectionPoint))
			{
				//nSide wins
				doorConnection.setX(box.getMinimum().x);
				doorConnection.setY(0.0f);
				doorConnection.setZ(dZ/2);
			}
			else
			{
				doorConnection.setX(box.getMaximum().x);
				doorConnection.setY(0.0f);
				doorConnection.setZ(dZ/2);
			}
		}
		if(_direction == Ogre::Vector3::UNIT_X || _direction == Ogre::Vector3::NEGATIVE_UNIT_X)
		{
			//facing either x-positive or x-negative
			//get deltaX
			float dX = box.getMaximum().x - box.getMinimum().x;

			Ogre::Vector3 pSide,nSide;
			pSide = box.getMaximum();
			pSide.x = dX/2;
			pSide.y = 0.0f;

			nSide = box.getMinimum();
			nSide.x = dX/2;
			nSide.y = 0.0f;

			nSide = toWorld * nSide;
			pSide = toWorld * pSide;
			if(pSide.squaredDistance(_connectionPoint) > nSide.squaredDistance(_connectionPoint))
			{
				//nSide wins
				doorConnection.setX(dX/2);
				doorConnection.setY(0.0f);
				doorConnection.setZ(box.getMinimum().z);
			}
			else
			{
				doorConnection.setX(dX/2);
				doorConnection.setY(0.0f);
				doorConnection.setZ(box.getMaximum().z);
			}
		}

		//NOT TESTED
		//p->createHingeConstraint(staticLevel->btBody,_door.btBody,connectionPoint,doorConnection,axis,axis);
		//TESTED
		_hinge = new btHingeConstraint(*(staticLevel->btBody),*_door.btBody,connectionPoint,doorConnection,axis,axis);
		_hinge->setLimit(_minAngle,_maxAngle);
		p->getWorld()->addConstraint(_hinge,true);
	}

	/*
	Note: Door script functions are expected to return these values(and in this order):
	a boolean for motor activation(mandatory)
	float for motor top speed(if motor is activated, mandatory)
	float for motor top impulse(if motor is activated, mandatory)
	string for entity activation(optional, return "NULL" for no change)
	string for new lua script function(optional, return "NULL" for no change)
	*/
	/*
	example lua function
	function foo()
		local bMotor = 1
		local fMotorTop = 3.0
		local fMotorInc = 5.0
		local newEnt = "NULL"
		local newScript = "NULL"
		return bMotor,fMotorTop,fMotorInc,newEnt,newScript
	end
	*/
	void DoorData::update()
	{
		//check for activation and act upon it
		if(_activated)
		{
			LuaManager::getSingleton().callFunction(_scriptName);
			//handle lua return values
			bool motor;
			float motorTop;
			float motorInc;
			std::string ent,nScript;
			lua_State* L = LuaManager::getSingleton().getLuaState();
			int argNum = lua_gettop(L);
			if(argNum >=3)
			{
				//all necessary arguments are there
				if(lua_isnumber(L,1))
				{
					motor = (lua_toboolean(L,1) != 0);
				}
				if(lua_isnumber(L,2))
				{
					motorTop = static_cast<float>(lua_tonumber(L,2));
				}
				if(lua_isnumber(L,3))
				{
					motorInc = static_cast<float>(lua_tonumber(L,3));
				}
				if(argNum == 5)
				{
					if(lua_isstring(L,4))
					{
						ent = lua_tostring(L,4);
					}
					if(lua_isstring(L,5))
					{
						nScript = lua_tostring(L,5);
					}
				}
				else
				{
					ent = "NULL";
					nScript = "NULL";
				}
			}

			//act on return values
			if(nScript != "NULL")
			{
				_scriptName = nScript;
			}
			if(ent != "NULL")
			{
				LuaManager::getSingleton().activateEntity(ent,true);
			}
			if(motor)
			{
				_hinge->enableAngularMotor(true,motorTop,motorInc);
			}
		}
	}

	//only some physics properties are changeable after hinge creation.
	//The list: rotation axis and angle limits.
	
	//NOTE: All other properties MUST be set before hinge creation(DoorData::CreateDoor).

	void DoorData::setScriptName(const std::string& scriptName)
	{
		_scriptName = scriptName;
	}
	std::string DoorData::getScriptName(){ return _scriptName;}

	void DoorData::setObjectFile(const std::string& fileName)
	{
		_objectFile = fileName;
		//load object into scene node and rigid body
	}

	void DoorData::setPosition(const Ogre::Vector3& position)
	{
		_position = position;
	}
	Ogre::Vector3 DoorData::getPosition(){ return _position;}

	void DoorData::setDirection(const Ogre::Vector3& direction)
	{
		_direction = direction;
	}
	Ogre::Vector3 DoorData::getDirection(){ return _direction;}

	void DoorData::setAxis(const Ogre::Vector3& axis)
	{
		_axis = axis;
		if(_hinge)
		{
			_hinge->setAxis(Utility::convert_OgreVector3(axis));
		}
	}
	Ogre::Vector3 DoorData::getAxis() { return _axis;}

	void DoorData::setConnectionPoint(const Ogre::Vector3& connection)
	{
		_connectionPoint = connection;
	}
	Ogre::Vector3 DoorData::getConnectionPoint() { return _connectionPoint; }

	void DoorData::setMinAngle(float angle)
	{
		_minAngle = angle;
		if(_hinge)
		{
			_hinge->setLimit(_minAngle,_maxAngle);
		}
	}
	float DoorData::getMinAngle() { return _minAngle;}

	void DoorData::setMaxAngle(float angle)
	{
		_maxAngle = angle;
		if(_hinge)
		{
			_hinge->setLimit(_minAngle,_maxAngle);
		}
	}
	float DoorData::getMaxAngle() { return _maxAngle;}

	//======================================
	//Level Parser. Takes a file and determines level data based on it.
	//======================================
	void LevelParser::setFile(const char* fileName)
	{
		_file = fileName;
	}

	void LevelParser::parseTriggers(std::vector<std::unique_ptr<TriggerZone>>* triggers)
	{
		std::ifstream dataFile(_file);
		std::string data;
		std::string type;
		std::string substr;
		std::string dataType;
		std::string values;

		//corners for boundaries = center (+/-) cornersOffset
		Ogre::Vector3 center,cornersOffset;
		std::string script;
		std::string target;
		int timeDelay;

		bool startObject = false;
		bool finishedObject = false;

		//go through the file and load data into containers.
		while(!dataFile.eof() && dataFile.good() && dataFile.is_open())
		{
			std::getline(dataFile,data);

			if(!startObject)
			{
				//get the type of the object first(if applicable)
				type = data.substr(0,data.find(';'));
				if(type.find("TriggerZone") != type.npos)
				{
					//data being read-in is a triggerzone
					//get triggerzone type
					type = type.substr(0,type.find('_'));
					startObject = true;
				}
			}

			if(startObject)
			{
				//determine what type of data this is.
				dataType = data.substr(0,data.find(':'));
				if(dataType == "Position")
				{	
					values = data.substr(data.find(':')+1,data.find(';'));
					float x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<float,std::string>(substr);
					center.x = static_cast<Ogre::Real>(x);
					center.y = static_cast<Ogre::Real>(y);
					center.z = static_cast<Ogre::Real>(z);
				}
				if(dataType == "ZoneScale")
				{
					values = data.substr(data.find(':')+1,data.find(';'));
					float x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<float,std::string>(substr);
					//build offsets for triggerzone boundaries
					cornersOffset.x = static_cast<Ogre::Real>(x / 2);
					cornersOffset.y = static_cast<Ogre::Real>(y / 2);
					cornersOffset.z = static_cast<Ogre::Real>(z / 2);
				}
				if(dataType == "TargetName")
				{
					target = data.substr(data.find(':')+1,data.find(';'));
				}
				if(dataType == "TimeDelay")
				{
					timeDelay = boost::lexical_cast<int,std::string>(data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1)));
				}
				if(dataType == "Callback")
				{
					script = data.substr(data.find(':')+1,data.find(';') - data.find(':'));
				}

				if(data == "};")
				{
					//marks end of triggerzone data.
					startObject = false;
					finishedObject = true;
				}
			}

			std::unique_ptr<TriggerZone> tZone;
			if(type == "plr" && finishedObject)
			{
				PlayerTrigger* playerTrig = new PlayerTrigger();
				playerTrig->setBoundaries(Ogre::AxisAlignedBox(center - cornersOffset,center + cornersOffset));
				playerTrig->setTriggerType(PLAYER);
				playerTrig->setScriptFunction(script);
				playerTrig->activate(false);
				//add to the vector
				tZone.reset(playerTrig);
			}

			if(type == "ent" && finishedObject)
			{
				EntityTrigger* entTrig = new EntityTrigger();
				entTrig->setBoundaries(Ogre::AxisAlignedBox(center - cornersOffset,center + cornersOffset));
				entTrig->setTriggerTarget(target);
				entTrig->setScriptFunction(script);
				entTrig->setTriggerType(ENTITY);
				entTrig->activate(false);
				tZone.reset(entTrig);
			}

			if(type == "time" && finishedObject)
			{
				TimeTrigger* timeTrig = new TimeTrigger();
				timeTrig->setBoundaries(Ogre::AxisAlignedBox(center - cornersOffset,center + cornersOffset));
				timeTrig->setTimeDelay(timeDelay);
				timeTrig->setTriggerType(TIME);
				timeTrig->setScriptFunction(script);
				timeTrig->activate(false);
				tZone.reset(timeTrig);
			}

			if(finishedObject)
			{
				triggers->push_back(std::move(tZone));
				finishedObject = false;
			}
		}
	}

	void LevelParser::parseLights(std::vector<std::unique_ptr<LightData>>* lights)
	{
		std::ifstream dataFile(_file);
		std::string data;
		std::string type;
		std::string substr;
		std::string dataType;
		std::string values;

		Ogre::Vector3 position,direction;
		int range;
		Ogre::ColourValue diffColour;
		Ogre::ColourValue specColour;
		float innerAng,outerAng;

		bool startObject = false;
		bool finishedObject = false;

		//go through the file and load data into containers.
		while(!dataFile.eof() && dataFile.good() && dataFile.is_open())
		{
			std::getline(dataFile,data);

			if(!startObject)
			{
				//get the type of the object first(if applicable)
				type = data.substr(0,data.find(';'));
				if(type.find("Light") != type.npos)
				{
					//data being read-in is a triggerzone
					//get triggerzone type
					type = type.substr(0,type.find('_'));
					startObject = true;
				}
			}

			if(startObject)
			{
				//determine what type of data this is.
				dataType = data.substr(0,data.find(':'));
				if(dataType == "Position")
				{	
					values = data.substr(data.find(':')+1,data.find(';'));
					float x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<float ,std::string>(substr);
					position.x = static_cast<Ogre::Real>(x);
					position.y = static_cast<Ogre::Real>(y);
					position.z = static_cast<Ogre::Real>(z);
				}
				if(dataType == "Direction")
				{
					values = data.substr(data.find(':')+1,data.find(';'));
					float x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<float,std::string>(substr);
					//build offsets for triggerzone boundaries
					direction.x = x;
					direction.y = y;
					direction.z = z;
				}
				if(dataType == "InnerAng")
				{
					innerAng = boost::lexical_cast<float,std::string>(data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1)));
				}
				if(dataType == "OuterAng")
				{
					outerAng = boost::lexical_cast<float,std::string>(data.substr(data.find(':')+1,data.find(';') - data.find(':')));
				}
				if(dataType == "Range")
				{
					range = boost::lexical_cast<int,std::string>(data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1)));
				}
				if(dataType == "Color")
				{
					values = data.substr(data.find(':')+1,data.find(';'));
					float r,b,g;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					r = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					g = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					b = boost::lexical_cast<float,std::string>(substr);
					//diffuse colour value
					diffColour.r = r;
					diffColour.g = g;
					diffColour.b = b;
				}

				if(dataType == "SpecColor")
				{
					values = data.substr(data.find(':')+1,data.find(';'));
					float r,b,g;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					r = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					g = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					b = boost::lexical_cast<float,std::string>(substr);
					//build specular colour value
					specColour.r = r;
					specColour.g = g;
					specColour.b = b;
				}

				if(data == "};")
				{
					//marks end of light data
					startObject = false;
					finishedObject = true;
				}
			}

			std::unique_ptr<LightData> lData;
			if(type == "spot" && finishedObject)
			{
				SpotLightData* sLight = new SpotLightData();
				sLight->setLightType(Ogre::Light::LT_SPOTLIGHT);
				sLight->setDirection(direction);
				sLight->setDiffuseColour(diffColour);
				sLight->setSpecularColour(specColour);
				sLight->setAngles(innerAng,outerAng);
				sLight->setRange(static_cast<float>(range));
				sLight->setPosition(position);
				lData.reset(sLight);
			}

			if(type == "point" && finishedObject)
			{
				PointLightData* pLight = new PointLightData();
				pLight->setLightType(Ogre::Light::LT_POINT);
				pLight->setDiffuseColour(diffColour);
				pLight->setSpecularColour(specColour);
				pLight->setRange(static_cast<float>(range));
				pLight->setPosition(position);
				lData.reset(pLight);
			}

			if(type == "directional" && finishedObject)
			{
				DirectionalLightData* dLight = new DirectionalLightData();
				dLight->setLightType(Ogre::Light::LT_DIRECTIONAL);
				dLight->setDirection(direction);
				dLight->setDiffuseColour(diffColour);
				dLight->setSpecularColour(specColour);
				lData.reset(dLight);
			}

			if(finishedObject)
			{
				lData->setType(LIGHT);
				lights->push_back(std::move(lData));
				finishedObject = false;
			}
		}
	}

	void LevelParser::parseDoors(std::vector<std::unique_ptr<DoorData>>* doors)
	{
		std::ifstream dataFile(_file);
		std::string data;
		std::string type;
		std::string substr;
		std::string dataType;
		std::string values;

		//different data to be collected
		Ogre::Vector3 position,direction,connectionPoint,axis;
		std::string name;
		std::string doorFileName,scriptName,baseName;
		float minAng,maxAng;

		bool startObject = false;
		bool finishedObject = false;

		//go through the file and load data into containers.
		while(!dataFile.eof() && dataFile.good() && dataFile.is_open())
		{
			std::getline(dataFile,data);

			if(!startObject)
			{
				//get the type of the object first(if applicable)
				type = data.substr(0,data.find(';'));
				if(type.find("Door") != type.npos)
				{
					//data being read-in is a door
					//get door type, but there aren't any door types
					//type = type.substr(0,type.find('_'));
					startObject = true;
				}
			}

			if(startObject)
			{
				//determine what type of data this is.
				dataType = data.substr(0,data.find(':'));
				if(dataType == "Position")
				{	
					values = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
					values = Ogre::StringUtil::replaceAll(values,","," ");
					position = Ogre::StringConverter::parseVector3(values);
					/*
					float x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<float ,std::string>(substr);
					position.x = static_cast<Ogre::Real>(x);
					position.y = static_cast<Ogre::Real>(y);
					position.z = static_cast<Ogre::Real>(z);
					*/
				}
				if(dataType == "Direction")
				{
					values = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
					values = Ogre::StringUtil::replaceAll(values,","," ");
					direction = Ogre::StringConverter::parseVector3(values);
					/*
					float x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<float,std::string>(substr);
					//turn the three floats into direction vector
					direction.x = x;
					direction.y = y;
					direction.z = z;
					*/
				}
				if(dataType == "MinAngle")
				{
					minAng = boost::lexical_cast<float,std::string>(data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1)));
				}
				if(dataType == "MaxAngle")
				{
					maxAng = boost::lexical_cast<float,std::string>(data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1)));
				}
				if(dataType == "Name")
				{
					name = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
				}
				if(dataType == "BaseName")
				{
					baseName = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
				}
				if(dataType == "Script")
				{
					scriptName = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
				}
				if(dataType == "DoorObject")
				{
					doorFileName = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
				}
				if(dataType == "Axis")
				{
					values = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
					values = Ogre::StringUtil::replaceAll(values,","," ");
					axis = Ogre::StringConverter::parseVector3(values);
					/*
					float x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<float,std::string>(substr);
					axis.x = x;
					axis.y = y;
					axis.z = z;
					*/
				}

				if(dataType == "DoorConnectionPoint")
				{
					values = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
					values = Ogre::StringUtil::replaceAll(values,","," ");
					connectionPoint = Ogre::StringConverter::parseVector3(values);
					/*
					float x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<float,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<float,std::string>(substr);
					connectionPoint.x = x;
					connectionPoint.y = y;
					connectionPoint.z = z;
					*/
				}

				if(data == "};")
				{
					//marks end of door data.
					startObject = false;
					finishedObject = true;
				}
			}

			if(finishedObject)
			{	
				std::unique_ptr<DoorData> dData(new DoorData());
				dData->setPosition(position);
				dData->setDirection(direction);
				dData->setAxis(axis);
				dData->setObjectFile(doorFileName);
				dData->setConnectionPoint(connectionPoint);
				dData->setMaxAngle(maxAng);
				dData->setMinAngle(minAng);
				dData->setName(name);
				dData->setScriptName(scriptName);
				doors->push_back(std::move(dData));
				finishedObject = false;
			}
		}
	}
};