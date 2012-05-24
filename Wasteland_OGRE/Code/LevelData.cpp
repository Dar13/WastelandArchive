#include"StdAfx.h"

#include "LevelData.h"
#include "LuaManager.h"

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
	TriggerZone::TriggerZone()
	{
		_triggered = false;
		_triggerType = 0;
		_triggerInZone = false;
		_boundaries = Ogre::AxisAlignedBox::BOX_NULL;
	}
	TriggerZone::TriggerZone(const Ogre::AxisAlignedBox& zoneBoundaries,TRIGGER_TYPE triggerType)
	{
		setBoundaries(zoneBoundaries);
		setType(triggerType);
		_triggered = false;
		_triggerType = NONE;
		_triggerInZone = false;
		_boundaries = Ogre::AxisAlignedBox::BOX_NULL;
	}

	void TriggerZone::setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries)
	{
		_boundaries = zoneBoundaries;
	}

	void TriggerZone::setTriggerType(TRIGGER_TYPE type)
	{
		_triggerType = type;
	}
	int TriggerZone::getTriggerType(){ return _triggerType; }

	void TriggerZone::setScriptName(const std::string& script)
	{
		_scriptName = script;
	}
	

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

			_activated = false; //function is only called once upon activation

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
	void TimeTrigger::setTimeGoal(int milliSecs,int startingTime)
	{
		_goalTime = _startTime + milliSecs;
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

		if(_triggered || _activated)
		{
			//call the callback
			LuaManager::getSingleton().callFunction(_scriptName);
			//handle lua return values

			//these are one-time triggers, unless manually reset
			_activated = false;
			_triggered = false;
			_timeActivated = false;
			_goalTime = 0;
			_startTime = 0;
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
	void LightData::setLightType(int type)
	{
		_lightType = type;
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
	}
	Ogre::ColourValue LightData::getDiffuseColour() { return _diffColour; }

	void LightData::setSpecularColour(const Ogre::ColourValue& specularColour)
	{
		_specColour = specularColour;
	}
	Ogre::ColourValue LightData::getSpecularColour() { return _specColour; }

	void SpotLightData::setAngles(float innerAngle,float outerAngle)
	{
		_innerAngle = innerAngle;
		_outerAngle = outerAngle;
	}

	void SpotLightData::setDirection(const Ogre::Vector3& direction)
	{
		_direction = direction;
	}
	Ogre::Vector3 SpotLightData::getDirection() { return _direction;}

	void SpotLightData::setPosition(const Ogre::Vector3& position)
	{
		_position = position;
	}
	Ogre::Vector3 SpotLightData::getPosition() { return _position;}

	void DirectionalLightData::setDirection(const Ogre::Vector3& direction)
	{
		_direction = direction;
	}
	Ogre::Vector3 DirectionalLightData::getDirection() { return _direction; }

	void PointLightData::setPosition(const Ogre::Vector3& position)
	{
		_position = position;
	}
	Ogre::Vector3 PointLightData::getPosition() { return _position; }

	//======================================
	//Door class, controls all the inner workings of doors.
	//======================================
	DoorData::DoorData()
	{

	}
	
	void DoorData::createDoor(GraphicsManager* g,PhysicsManager* p)
	{
		//create door from internal data.
	}

	/*
	Note: Door script functions are expected to return these values(and in this order):
	a boolean for motor activation(mandatory)
	float for motor top speed(if motor is activated, mandatory)
	float for motor top impulse(if motor is activated, mandatory)
	string for entity activation(optional, return "NULL" for no change)
	string for new lua script function(optional, return "NULL" for no change)
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
					motor = lua_toboolean(L,1);
				}
				if(lua_isnumber(L,2))
				{
					motorTop = lua_tonumber(L,2);
				}
				if(lua_isnumber(L,3))
				{
					motorInc = lua_tonumber(L,3);
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

			}
		}
	}

	void DoorData::setName(const std::string& name)
	{
		_name = name;
	}
	std::string DoorData::getName(){ return _name;}

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
	}
	Ogre::Vector3 DoorData::getAxis() { return _axis;}

	void DoorData::setMinAngle(float angle)
	{
		_minAngle = angle;
	}
	float DoorData::getMinAngle() { return _minAngle;}

	void DoorData::setMaxAngle(float angle)
	{
		_maxAngle = angle;
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
		std::string callback;
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
					int x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<int,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<int,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<int,std::string>(substr);
					center.x = static_cast<Ogre::Real>(x);
					center.y = static_cast<Ogre::Real>(y);
					center.z = static_cast<Ogre::Real>(z);
				}
				if(dataType == "ZoneScale")
				{
					values = data.substr(data.find(':')+1,data.find(';'));
					int x,y,z;
					int first = values.find(',');
					int second = values.find(',',first + 1);
					substr = values.substr(0,values.find_first_of(','));
					x = boost::lexical_cast<int,std::string>(substr);
					substr = values.substr(first+1,second - (first + 1));
					y = boost::lexical_cast<int,std::string>(substr);
					substr = values.substr(second+1,values.find(';') - (second+1));
					z = boost::lexical_cast<int,std::string>(substr);
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
					callback = data.substr(data.find(':')+1,data.find(';') - data.find(':'));
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
				playerTrig->setType(PLAYER);
				//add to the vector
				tZone.reset(playerTrig);
			}

			if(type == "ent" && finishedObject)
			{
				EntityTrigger* entTrig = new EntityTrigger();
				entTrig->setBoundaries(Ogre::AxisAlignedBox(center - cornersOffset,center + cornersOffset));
				entTrig->setTriggerTarget(target);
				entTrig->setType(ENTITY);
				tZone.reset(entTrig);
			}

			if(type == "time" && finishedObject)
			{
				TimeTrigger* timeTrig = new TimeTrigger();
				timeTrig->setBoundaries(Ogre::AxisAlignedBox(center - cornersOffset,center + cornersOffset));
				timeTrig->setTimeGoal(timeDelay,0);
				timeTrig->setType(TIME);
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

		//corners for boundaries = center (+/-) cornersOffset
		Ogre::Vector3 center,direction;
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
					center.x = static_cast<Ogre::Real>(x);
					center.y = static_cast<Ogre::Real>(y);
					center.z = static_cast<Ogre::Real>(z);
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
				sLight->setRange(range);
				sLight->setPosition(center);
				lData.reset(sLight);
			}

			if(type == "point" && finishedObject)
			{
				PointLightData* pLight = new PointLightData();
				pLight->setLightType(Ogre::Light::LT_POINT);
				pLight->setDiffuseColour(diffColour);
				pLight->setSpecularColour(specColour);
				pLight->setRange(range);
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
				if(type.find("Doors") != type.npos)
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
					//turn the three floats into direction vector
					direction.x = x;
					direction.y = y;
					direction.z = z;
				}
				if(dataType == "MinAngle")
				{
					minAng = boost::lexical_cast<float,std::string>(data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1)));
				}
				if(dataType == "MaxAngle")
				{
					maxAng = boost::lexical_cast<float,std::string>(data.substr(data.find(':')+1,data.find(';') - data.find(':')));
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
					axis.x = x;
					axis.y = y;
					axis.z = z;
				}

				if(dataType == "DoorConnectionPoint")
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
					connectionPoint.x = x;
					connectionPoint.y = y;
					connectionPoint.z = z;
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
				std::unique_ptr<DoorData> dData;
				dData->setPosition(position);
				dData->setDirection(direction);
				dData->setAxis(axis);
				dData->setObjectFile(doorFileName);
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