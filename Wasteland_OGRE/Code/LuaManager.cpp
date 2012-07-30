#include "StdAfx.h"

#include "LuaManager.h"

#include "interfaces\interfaces.h"

template<> LuaManager* Ogre::Singleton<LuaManager>::ms_Singleton = 0;

LuaManager::LuaManager()
{
	luaState = nullptr;
}

lua_State* LuaManager::getLuaState() { return luaState;}

//Forces explicit setup.
void LuaManager::Setup(std::string luaListFileName)
{
	luaState = lua_open();
	luaL_openlibs(luaState);

	//exposes all Lua functions to luaState and thus to the program itself through the LuaManager.
	list_t* llist = list(luaListFileName).release();
	std::unique_ptr<list_t> luaList(llist);
	for(list_t::file_const_iterator itr = luaList->file().begin(); itr != luaList->file().end(); ++itr)
	{
		luaL_dofile(luaState,(*itr).c_str());
	}

	//register lua-accessible functions
	registerFunction("activate",activate);

	return;
}

//Function must be static and of form "int func(lua_State*)"
//define the arguments through comments
void LuaManager::registerFunction(std::string funcName,luaFunction funcPtr)
{
	lua_register(luaState,funcName.c_str(),funcPtr);
}

void LuaManager::callFunction(const std::string& funcName)
{
	lua_getglobal(luaState,funcName.c_str());
	
	lua_call(luaState,0,0);
}

void LuaManager::callFunction(const std::string& funcName,int expectedNumReturn)
{
	lua_getglobal(luaState,funcName.c_str());

	lua_call(luaState,0,expectedNumReturn);
}

//Grouped functions ->
//Allows for calling code(from states or something else) to push parameters to stack.
//Useful for AI and global scripts.
void LuaManager::prepFunction(const std::string& funcName)
{
	lua_getglobal(luaState,funcName.c_str());
}

void LuaManager::callFunction(int paramNum,int retNum)
{
	lua_call(luaState,paramNum,retNum);
}
//<- Grouped functions

void LuaManager::addEntity(const std::string& name,LevelData::BaseEntity* entity)
{
	std::pair<std::string,LevelData::BaseEntity*> p;

	p.first = name;
	p.second = entity;

	_entities.insert(p);

	return;
}

void LuaManager::activateEntity(const std::string& name,bool value)
{
	_entities[name]->activate(value);
}

LuaManager::~LuaManager()
{
	//clean up lua.
	lua_close(luaState);
}

int activate(lua_State* lua)
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