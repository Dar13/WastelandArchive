#include "StdAfx.h"
#include <lua.hpp>

//#include <OgreSingleton.h>

#ifndef _LUA_MANAGER_H_
#define _LUA_MANAGER_H_

#include "LevelData.h"

//Make my life easier
typedef int (*luaFunction)(lua_State*);

class LuaManager : public Ogre::Singleton<LuaManager>
{
public:
	LuaManager();
	~LuaManager();

	//since get function
	lua_State* getLuaState();

	//Required!
	void Setup(std::string luaListFileName);

	//Registers C-functions with Lua.
	void registerFunction(std::string funcName,luaFunction funcPtr);

	//Calls the function, but calling code must handle parameters and return values
	//no return values
	void callFunction(const std::string& funcName);
	//return values
	void callFunction(const std::string& funcName,int expectedNumReturn);

	//If prepFunction is called, then callFunction must be called afterwards.
	//All prepFunction does is allow the calling code to push parameters onto the stack for the lua function
	void prepFunction(const std::string& funcName);
	void callFunction(int paramNum,int retNum);

	//Function argument functions
	void pushFunctionArg(boost::variant<int,double,std::string> arg);
	void pushFunctionArgVector(const Ogre::Vector3& vector);
	void pushFunctionArgVector(const btVector3& vector);

	//Function cleanup
	void clearLuaStack() { lua_settop(luaState,0); }

	//Entity handling functions.
	void addEntity(const std::string& name,LevelData::BaseEntity* entity);
	LevelData::BaseEntity* getEntity(const std::string& name) 
	{ 
		if(_entities.find(name) != _entities.end())
		{
			return _entities[name];
		}
		else
		{
			return nullptr;
		}
	}
	void removeEntity(const std::string& name) { _entities.erase(_entities.find(name)); }
	void purgeEntities();
	void activateEntity(const std::string& name, bool value);
	std::map<std::string,LevelData::BaseEntity*>* _getEntities() { return &_entities; }

	//Maintained values handlers
	void addDataPointer(const std::string& name,void* dataPtr);
	void removeDataPointer(const std::string& name);
	//!This must be called every time the application exits an app-state
	void purgeData();
	//NOTE: The calling function takes full responsibility of the subsequent handling of the pointer.
	//Any memory corruption called by the calling function can crash entire game, as this Manager is consistent
	//throughout the game, unlike the physics engine or the navmesh.
	void* _getData(const std::string& name);

	//LuaData functions
	bool searchForLuaData(std::string name,LuaData* luaData);
	void addLuaData(LuaData data,lua_State* lua);
	void purgeLuaData();

	//SoundEvent functions
	std::vector<SoundEvent>& getSoundEventQueue() { return _soundEvents; }
	void addSoundEvent(SoundEvent& sEvent);
	void addSoundEvent(std::string& name, FMOD_VECTOR& position);

	//Helper functions
	static int getIntegerFromLuaTable(lua_State* lua,const std::string& field);
	static std::string getStringFromLuaTable(lua_State* lua,const std::string& field);
	static Ogre::Vector3 getVectorFromLuaTable(lua_State* lua,const std::string& field);
	static Ogre::Vector3 getVectorFromLua(lua_State* lua,int tableIndex);

	//Comprehensive cleaning function. MUST BE CALLED IN EVERY AppState's Shutdown() METHOD!
	void deepClean();

	//for debugging purposes only!
	void _printStack();

private:
	lua_State* luaState;

	std::map<std::string,LevelData::BaseEntity*> _entities;

	std::map<std::string,void*> _data;
	std::map<std::string,boost::variant<double,std::string,bool>> _luaData;

	std::vector<SoundEvent> _soundEvents;

	LuaManager(const LuaManager&);
	LuaManager& operator=(const LuaManager&);
};

//Helper class for LuaManager
class argVisitor : public boost::static_visitor<>
{
public:
	argVisitor() : numeric(-9999),rational(-9999.99),string("") {}
	int numeric;
	double rational;
	std::string string;

	void clear() { numeric = 0; rational = 0.0; string = ""; }

	void getType(bool* num,bool* rat,bool* str);

	void operator()(const int& i);
	void operator()(const std::string& str);
	void operator()(const double& d);
private:
};

//==========================================
//Functions that will be registered with Lua
//==========================================

//Activation function. Allows interface between LuaManager and Lua without luabind or tolua++ or whatever.
int activate(lua_State* lua);

//Allows Lua scripts to change in-game entity names.
int changeEntityName(lua_State* lua);

//Allows Lua scripts to get entity positions.
int getEntityPosition(lua_State* lua);

//Allows Lua scripts to get the position of an entity's head
int getEntityHeadPosition(lua_State* lua);

//Allows Lua scripts to find nearest entities
int getNearestEntity(lua_State* lua);

//Allows Lua to print through std::cout directly.
int printDebug(lua_State* lua);

//Fast distance check function available to Lua.
int distanceCheck(lua_State* lua);

//Allows Lua access to player position information.
int getPlayerPosition(lua_State* lua);

//Allows Lua scripts to set an entry in a string/integer table
int setIntegerData(lua_State* lua);

//Allows Lua scripts to set an entry in a string/boost::variant map.
int setStringData(lua_State* lua);

//Allows Lua scripts to set an entry in a string/boost::variant map.
int setBooleanData(lua_State* lua);

//Allows Lua scripts to play sounds directly.
int playSound(lua_State* lua);

#endif