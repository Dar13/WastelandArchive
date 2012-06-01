#include <lua.hpp>

#include <OgreSingleton.h>

#ifndef _LUA_MANAGER_H_
#define _LUA_MANAGER_H_

#include "LevelData.h"

//Make my life easier
typedef int (*luaFunction)(lua_State*);

/*
lua_State* luaTest;
luaTest = lua_open();

luaL_openlibs(luaTest);
luaL_dofile(luaTest,"resource\\lua\\test.lua");
int sum;
{
	lua_getglobal(luaTest,"add");
	lua_pushnumber(luaTest,5);
	lua_pushnumber(luaTest,2);
	lua_call(luaTest,2,1);
	sum = (int)lua_tointeger(luaTest,-1);
	lua_pop(luaTest,1);
}
lua_close(luaTest);
*/

class LuaManager : public Ogre::Singleton<LuaManager>
{
public:
	LuaManager();
	~LuaManager();

	lua_State* getLuaState();

	void Setup(std::string luaListFileName);

	void registerFunction(std::string funcName,luaFunction funcPtr);

	//Calls the function, but calling code must handle parameters and return values
	//no return values
	void callFunction(const std::string& funcName);
	void callFunction(const std::string& funcName,int expectedNumReturn);

	//If prepFunction is called, then callFunction must be called afterwards.
	//All prepFunction does is allow the calling code to push parameters onto the stack for the lua function
	void prepFunction(const std::string& funcName);
	void callFunction(int paramNum,int retNum);

	void addEntity(const std::string& name,LevelData::BaseEntity* entity);
	void purgeEntities();

	void activateEntity(const std::string& name, bool value);

private:
	lua_State* luaState;

	std::map<std::string,LevelData::BaseEntity*> _entities;

	LuaManager(const LuaManager&);
	LuaManager& operator=(const LuaManager&);
};

//==========================================
//Functions that will be registered with Lua
//==========================================

//Activation function. Allows interface between LuaManager and Lua without luabind or tolua++ or whatever.
static int activate(lua_State* lua)
{
	//number of arguments. NOT ZERO-INDEXED
	int argNum = lua_gettop(lua);

	std::string entName;
	bool value = true;

	//Lua function call should look like this: retVal = activate("Entity_to_Activate",1)

	if(argNum == 2)
	{
		//assume correct argument order
		if(lua_isstring(lua,1))
		{
			entName = lua_tostring(lua,1);
		}
		if(lua_isnumber(lua,2))
		{
			value = (lua_toboolean(lua,2) != 0);
		}
	}

	LuaManager::getSingleton().activateEntity(entName,value);

	lua_pushboolean(lua,value);

	return 1;
}

#endif