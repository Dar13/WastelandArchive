#include"StdAfx.h"

#include "LevelData.h"
#include "LuaManager.h"
#include "Utility.h"

#include <boost\lexical_cast.hpp>
#include <boost\tokenizer.hpp>

typedef boost::token_iterator_generator<boost::char_separator<char>>::type TknItr_Str;

TknItr_Str getTknItrStart(const std::string& str,boost::char_separator<char> &sep)
{
	return boost::make_token_iterator<std::string>(str.begin(),str.end(),sep);
}

TknItr_Str getTknItrEnd(const std::string& str,boost::char_separator<char> &sep)
{
	return boost::make_token_iterator<std::string>(str.end(),str.end(),sep);
}

void getVector3FromTknItr(Ogre::Vector3& vector,TknItr_Str start,TknItr_Str end)
{
	int i = 0;
	for(auto itr = start; itr != end && i < 3; ++itr,++i)
	{
		try
		{
			vector[i] = boost::lexical_cast<Ogre::Real,std::string>(*itr);
		}
		catch(boost::bad_lexical_cast& e)
		{
			//these errors are way too common to ignore.
			OutputDebugStringA(e.what());
			vector[i] = static_cast<Ogre::Real>(0.0f);
		}
	}
}

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

	void TriggerZone::setTriggerType(TRIGGER_TYPE type)
	{
		_triggerType = type;
	}
	int TriggerZone::getTriggerType(){ return _triggerType; }

	//============================
	//Global Trigger, derived from TriggerZone
	//============================
	void GlobalTrigger::update(OgreTransform& playerTransform,int deltaTimeInMs)
	{
		if(!_continuousExecution)
		{
			if(_activated && !_triggered)
			{
				_triggered = true;
			}
		}
		else
		{
			_triggered = true;
		}

		if(_triggered)
		{
			LuaManager::getSingleton().callFunction(_scriptName);
			//handle whatever return values I want it to do(none?)
		}
	}

	//============================
	//Player Trigger, derived from TriggerZone
	//============================
	void PlayerTrigger::update(OgreTransform& playerTransform, int deltaTimeInMs)
	{
		_triggered = check(playerTransform);
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

	void PlayerTrigger::setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries)
	{
		_boundaries = zoneBoundaries;
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
	void EntityTrigger::update(OgreTransform& playerTransform,int deltaTimeInMs)
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

	void EntityTrigger::setBoundaries(const Ogre::AxisAlignedBox& zoneBoundaries)
	{
		_boundaries = zoneBoundaries;
	}

	//===============================
	//TimeTrigger, derived from TriggerZone
	//===============================
	void TimeTrigger::setTimeDelay(int milliSecs)
	{
		_timeDelay = milliSecs;
	}

	void TimeTrigger::update(OgreTransform& playerTransform,int deltaTimeInMs)
	{
		if(_timeActivated)
		{
			_currentTime += deltaTimeInMs;
			_triggered = check(_currentTime);
		}
		else
		{
			_triggered = false;
		}

		if(_activated && !_timeActivated)
		{
			_timeActivated = true;
			_goalTime = _timeDelay;
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
			_currentTime = 0;
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
			int d1 = 0,d2 = 0,d3 = 0;
			int s1 = 0,s2 = 0,s3 = 0;
			bool vis = true;
			lua_State* L = LuaManager::getSingleton().getLuaState();

			if(lua_isnumber(L,1))
			{
				vis = (lua_toboolean(L,1) != 0);
			}
			if(lua_isnumber(L,2))
			{
				d1 = lua_tointegerx(L,2,NULL);
			}
			if(lua_isnumber(L,3))
			{
				d2 = lua_tointegerx(L,3,NULL);
			}
			if(lua_isnumber(L,4))
			{
				d3 = lua_tointegerx(L,4,NULL);
			}
			if(lua_isnumber(L,5))
			{
				s1 = lua_tointegerx(L,5,NULL);
			}
			if(lua_isnumber(L,6))
			{
				s2 = lua_tointegerx(L,6,NULL);
			}
			if(lua_isnumber(L,7))
			{
				s3 = lua_tointegerx(L,7,NULL);
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
		Ogre::Vector3 pSide,nSide,localMax = box.getMaximum(),localMin = box.getMinimum();
		if(_direction == Ogre::Vector3::UNIT_Z || _direction == Ogre::Vector3::NEGATIVE_UNIT_Z)
		{
			//facing either z-positive or z-negative
			//shouldn't matter, but let's keep that in mind.
			//means deltaX should be longer than deltaZ(if I set up the .ent file correctly anyways).
			//getting deltaZ
			float dZ = box.getMaximum().z - box.getMinimum().z;

			//get points on side of box(x-axis)
			pSide = box.getMaximum();
			pSide.y = 0.0f;
			pSide.z = 0.0f;
			//dZ/2 gives the corner
			//pSide.z = dZ/2;

			nSide = box.getMinimum();
			nSide.y = 0.0f;
			nSide.z = 0.0f;
			//nSide.z = dZ/2;

			//compare distance of nSide/pSide to connectionPoint, after nSide and pSide are in staticLevel coordinate space
			pSide = toWorld * pSide;
			nSide = toWorld * nSide;
			if(pSide.squaredDistance(_connectionPoint) > nSide.squaredDistance(_connectionPoint))
			{
				//nSide wins
				doorConnection.setX(localMin.x);
				doorConnection.setY(0.0f);
				doorConnection.setZ(0.0f);
				//doorConnection.setZ(dZ/2);
			}
			else
			{
				doorConnection.setX(localMax.x);
				doorConnection.setY(0.0f);
				doorConnection.setZ(0.0f);
				//doorConnection.setZ(dZ/2);
			}
		}
		if(_direction == Ogre::Vector3::UNIT_X || _direction == Ogre::Vector3::NEGATIVE_UNIT_X)
		{
			//facing either x-positive or x-negative
			//get deltaX
			float dX = box.getMaximum().x - box.getMinimum().x;

			pSide = box.getMaximum();
			pSide.x = 0.0f;
			//gets the corner, not the middle
			//pSide.x = dX/2;
			pSide.y = 0.0f;

			nSide = box.getMinimum();
			nSide.x = 0.0f;
			//gets the corner, not the middle
			//nSide.x = dX/2;
			nSide.y = 0.0f;

			//transform the coordinates to world coordinates from the local space
			nSide = toWorld * nSide;
			pSide = toWorld * pSide;
			if(pSide.squaredDistance(_connectionPoint) > nSide.squaredDistance(_connectionPoint))
			{
				//nSide wins
				doorConnection.setX(0.0f);
				//doorConnection.setX(dX/2);
				doorConnection.setY(0.0f);
				doorConnection.setZ(localMin.z);
			}
			else
			{
				doorConnection.setX(0.0f);
				//doorConnection.setX(dX/2);
				doorConnection.setY(0.0f);
				doorConnection.setZ(localMax.z);
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
			LuaManager::getSingleton().callFunction(_scriptName,5);
			//handle lua return values
			bool motor = false;
			float motorTop = 0.0f;
			float motorInc = 0.0f;
			std::string ent = "",nScript = "";
			lua_State* L = LuaManager::getSingleton().getLuaState();
			int argNum = lua_gettop(L);
			if(argNum >=3)
			{
				//all necessary arguments are there
				if(lua_isnumber(L,1))
				{
					int test = lua_toboolean(L,1);
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
			if(nScript != "NULL" && nScript != "" && nScript != "nil" && nScript != "null")
			{
				_scriptName = nScript;
			}
			if(ent != "NULL" && ent != "" && ent != "nil" && ent != "null")
			{
				LuaManager::getSingleton().activateEntity(ent,true);
			}
			if(motor)
			{
				_hinge->enableAngularMotor(true,motorTop,motorInc);
				_activated = false;
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
	//Waypoint class(es)
	//======================================
	WaypointSet::WaypointSet(std::vector<Waypoint> waypoints,bool sortByOrder)
		: _currentWaypoint(0),_sorted(false),_finished(false),_finalized(false)
	{
		_waypoints = waypoints;
		if(sortByOrder)
		{
			_sort();
			_sorted = true;
		}
	}

	void WaypointSet::addWaypoint(const Waypoint& waypoint,bool sort)
	{
		_waypoints.push_back(waypoint);
		if(sort)
		{
			_sort();
		}

		_sorted = sort;

		return;
	}

	void WaypointSet::finalizeSet()
	{
		//sort it if it isn't already.
		if(!_sorted)
		{
			_sort();
			_sorted = true;
		}

		_finalized = true;
	}

	void WaypointSet::updateProgress(const Ogre::Vector3& position)
	{
		Ogre::Vector3 t1 = position;
		Ogre::Vector3 t2 = _waypoints[_currentWaypoint+1].getPosition();
		Ogre::Real distToWayPtSQ = t1.squaredDistance(t2);
		if(Ogre::Math::RealEqual(distToWayPtSQ,2.0f))
		{
			if(static_cast<unsigned int>(_currentWaypoint) + 1 < _waypoints.size() - 1)
			{
				_currentWaypoint++;
			}
			else
			{
				_finished = true;
			}
		}
	}

	Ogre::SimpleSpline WaypointSet::generateSpline()
	{
		Ogre::SimpleSpline spline;
		if(_sorted && _finalized)
		{
			std::for_each(_waypoints.begin(),
						  _waypoints.end(),
						  [&] (Waypoint& w) { spline.addPoint(w.getPosition()); return; });
		}
		else
		{
			//it may not be finalized, but that doesn't mean I can't do it.
			_sort(); 
			_sorted = true;
			std::for_each(_waypoints.begin(),
						  _waypoints.end(),
						  [&] (Waypoint& w) { spline.addPoint(w.getPosition()); return; });
		}

		return spline;
	}

	void WaypointSet::_sort()
	{
		std::sort(_waypoints.begin(),
				  _waypoints.end(),
				  [&] (Waypoint& w1,Waypoint& w2) { return w1.getOrder() < w2.getOrder(); });
	}

	//======================================
	//Level Parser. Takes a file and determines level data based on it.
	//======================================
	void LevelParser::setFile(const char* fileName)
	{
		_file = fileName;
	}

	void LevelParser::parseTriggers(std::vector<std::unique_ptr<TriggerZone>>* triggers,Ogre::SceneNode* rootNode)
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
		std::string name;
		int timeDelay = 0;
		bool contExec = false;

		bool startObject = false;
		bool finishedObject = false;

		boost::char_separator<char> fieldSeparator(":;");
		boost::char_separator<char> dataSeparator(",");

		while(!dataFile.eof() && dataFile.good() && dataFile.is_open())
		{
			std::getline(dataFile,data);

			if(data == "")
			{
				continue;
			}

			if(!startObject && !finishedObject)
			{
				TknItr_Str fieldS = getTknItrStart(data,fieldSeparator);
				TknItr_Str fieldE = getTknItrEnd(data,fieldSeparator);
				if(boost::next(fieldS) == fieldE && (*fieldS).find("TriggerZone") != std::string::npos)
				{
					type = data.substr(0,data.find("_"));
					startObject = true;
				}
			}

			if(startObject)
			{
				TknItr_Str fieldS = getTknItrStart(data,fieldSeparator);
				TknItr_Str fieldE = getTknItrEnd(data,fieldSeparator);
				if( *fieldS == "Position" )
				{
					values = *(boost::next(fieldS));
					TknItr_Str dataS = getTknItrStart(values,dataSeparator);
					TknItr_Str dataE = getTknItrEnd(values,dataSeparator);
					getVector3FromTknItr(center,dataS,dataE);
				}
				if( *fieldS == "ZoneScale" )
				{
					values = *(boost::next(fieldS));
					TknItr_Str dataS = getTknItrStart(values,dataSeparator);
					TknItr_Str dataE = getTknItrEnd(values,dataSeparator);
					getVector3FromTknItr(cornersOffset,dataS,dataE);
				}
				if( *fieldS == "TargetName" )
				{
					target = *boost::next(fieldS);
				}
				if( *fieldS == "TimeDelay" )
				{
					timeDelay = boost::lexical_cast<int,std::string>(*boost::next(fieldS));
				}
				if( *fieldS == "Callback" )
				{
					script = *boost::next(fieldS);
				}
				if( *fieldS == "ContExec" )
				{
					contExec = boost::lexical_cast<bool,std::string>(*boost::next(fieldS));
				}
				if( *fieldS == "}")
				{
					startObject = false;
					finishedObject = true;
				}
			}

			if(finishedObject)
			{
				std::unique_ptr<TriggerZone> tZone;
				Ogre::AxisAlignedBox boundaries(center - cornersOffset,center + cornersOffset);
				if(type == "plr")
				{
					//PlayerTrigger* playerTrig = new PlayerTrigger();
					PlayerTrigger* playerTrig = new PlayerTrigger(script,boundaries,false);
					//playerTrig->setBoundaries(boundaries);
					//playerTrig->setTriggerType(PLAYER);
					//playerTrig->setScriptFunction(script);
					//playerTrig->activate(false);
					tZone.reset(playerTrig);
				}

				if(type == "ent")
				{
					//EntityTrigger* entTrig = new EntityTrigger();
					EntityTrigger* entTrig = new EntityTrigger(script,target,boundaries,rootNode,false);
					//entTrig->setBoundaries(boundaries);
					//entTrig->setTriggerTarget(target);
					//entTrig->setScriptFunction(script);
					//entTrig->setTriggerType(ENTITY);
					//entTrig->activate(false);
					tZone.reset(entTrig);
				}

				if(type == "time")
				{
					//TimeTrigger* timeTrig = new TimeTrigger();
					TimeTrigger* timeTrig = new TimeTrigger(script,timeDelay,false);
					//timeTrig->setTimeDelay(timeDelay);
					//timeTrig->setTriggerType(TIME);
					//timeTrig->setScriptFunction(script);
					//timeTrig->activate(false);
					tZone.reset(timeTrig);
				}

				if(type == "global")
				{
					//GlobalTrigger* globTrig = new GlobalTrigger();
					GlobalTrigger* globTrig = new GlobalTrigger(script,true,false);
					//globTrig->setContinuousExecution(true);
					//globTrig->setTriggerType(GLOBAL);
					//globTrig->setScriptFunction(script);
					//globTrig->activate(false);
					tZone.reset(globTrig);
				}

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
		int range = 0;
		Ogre::ColourValue diffColour;
		Ogre::ColourValue specColour;
		float innerAng = 0.0f,outerAng = 0.0f;

		bool startObject = false;
		bool finishedObject = false;

		boost::char_separator<char> fieldSeparator(":;");
		boost::char_separator<char> dataSeparator(",");

		while(!dataFile.eof() && dataFile.good() && dataFile.is_open())
		{
			std::getline(dataFile,data);
			
			if(data == "") { continue; }

			if(!startObject && !finishedObject)
			{
				TknItr_Str fieldS = getTknItrStart(data,fieldSeparator);
				TknItr_Str fieldE = getTknItrEnd(data,fieldSeparator);
				if(boost::next(fieldS) == fieldE && (*fieldS).find("Light") != std::string::npos)
				{
					type = data.substr(0,data.find("_"));
					startObject = true;
				}
			}

			if(startObject)
			{
				TknItr_Str fieldS = getTknItrStart(data,fieldSeparator);
				TknItr_Str fieldE = getTknItrEnd(data,fieldSeparator);
				if( *fieldS == "Position" )
				{
					values = *boost::next(fieldS);
					TknItr_Str dataS = getTknItrStart(values,dataSeparator);
					TknItr_Str dataE = getTknItrEnd(values,dataSeparator);
					getVector3FromTknItr(position,dataS,dataE);
				}
				if( *fieldS == "Direction" )
				{
					values = *boost::next(fieldS);
					TknItr_Str dataS = getTknItrStart(values,dataSeparator);
					TknItr_Str dataE = getTknItrStart(values,dataSeparator);
					getVector3FromTknItr(direction,dataS,dataE);
				}
				if( *fieldS == "InnerAng" )
				{
					innerAng = boost::lexical_cast<float,std::string>(*boost::next(fieldS));
				}
				if( *fieldS == "OuterAng" )
				{
					outerAng = boost::lexical_cast<float,std::string>(*boost::next(fieldS));
				}
				if( *fieldS == "Range" )
				{
					range = boost::lexical_cast<int,std::string>(*boost::next(fieldS));
				}
				if( *fieldS == "Color" )
				{
					values = *boost::next(fieldS);
					TknItr_Str dataS = getTknItrStart(values,dataSeparator);
					TknItr_Str dataE = getTknItrEnd(values,dataSeparator);
					Ogre::Vector3 t;
					getVector3FromTknItr(t,dataS,dataE);
					diffColour.r = t.x;
					diffColour.g = t.y;
					diffColour.b = t.z;
				}
				if( *fieldS == "SpecColor" )
				{
					values = *boost::next(fieldS);
					TknItr_Str dataS = getTknItrStart(values,dataSeparator);
					TknItr_Str dataE = getTknItrEnd(values,dataSeparator);
					Ogre::Vector3 t;
					getVector3FromTknItr(t,dataS,dataE);
					specColour.r = t.x;
					specColour.g = t.y;
					specColour.b = t.z;
				}
				if( *fieldS == "}" )
				{
					finishedObject = true;
					startObject = false;
				}
			}

			if(finishedObject)
			{
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
		float minAng = 0.0f,maxAng = 0.0f;

		bool startObject = false;
		bool finishedObject = false;

		//go through the file and load data into containers.
		while(!dataFile.eof() && dataFile.good() && dataFile.is_open())
		{
			std::getline(dataFile,data);

			if(data == "")
			{
				continue;
			}

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
				}
				if(dataType == "Direction")
				{
					values = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
					values = Ogre::StringUtil::replaceAll(values,","," ");
					direction = Ogre::StringConverter::parseVector3(values);
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
				}

				if(dataType == "DoorConnectionPoint")
				{
					values = data.substr(data.find(':')+1,data.find(';') - (data.find(':')+1));
					values = Ogre::StringUtil::replaceAll(values,","," ");
					connectionPoint = Ogre::StringConverter::parseVector3(values);
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
	} //end of LevelParser::parseDoors()

	void LevelParser::parseWaypoints(std::vector<Waypoint>* waypoints)
	{
		std::ifstream dataFile(_file);
		std::string data,type,dataType,values;

		Ogre::Vector3 position;
		int order = 0;

		bool startObject = false;
		bool finishedObject = false;

		boost::char_separator<char> fieldSeparator(":;");
		boost::char_separator<char> dataSeparator(",");

		while(!dataFile.eof() && dataFile.good() && dataFile.is_open())
		{
			std::getline(dataFile,data);

			if(data == "")
			{
				continue;
			}

			if(!startObject)
			{
				TknItr_Str fieldS = getTknItrStart(data,fieldSeparator);
				TknItr_Str fieldE = getTknItrEnd(data,fieldSeparator);
				if(boost::next(fieldS) == fieldE && (*fieldS).find("Waypoint") != std::string::npos)
				{
					type = data.substr(0,data.find("_"));
					startObject = true;
				}
			}

			if(startObject)
			{
				TknItr_Str fieldS = getTknItrStart(data,fieldSeparator);
				TknItr_Str fieldE = getTknItrEnd(data,fieldSeparator);
				if(*fieldS == "Position")
				{
					values = *boost::next(fieldS);
					TknItr_Str dataS = getTknItrStart(values,dataSeparator);
					TknItr_Str dataE = getTknItrEnd(values,dataSeparator);
					getVector3FromTknItr(position,dataS,dataE);
				}
				if(*fieldS == "Order")
				{
					order = boost::lexical_cast<int,std::string>(*boost::next(fieldS));
				}
				if(*fieldS == "}")
				{
					finishedObject = true;
					startObject = false;
				}
			}

			if(finishedObject)
			{
				Waypoint way;
				way.setPosition(position);
				way.setOrder(order);
				waypoints->push_back(way);
				finishedObject = false;
				startObject = false;
			}
		}
	}
};