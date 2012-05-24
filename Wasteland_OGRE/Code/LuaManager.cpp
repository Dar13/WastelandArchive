#include "StdAfx.h"

#include "LuaManager.h"

#include "interfaces\interfaces.h"

template<> LuaManager* Ogre::Singleton<LuaManager>::ms_Singleton = 0;

LuaManager::LuaManager()
{

}

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

	return;
}

//Function must be static and of form "int func(lua_State*)"
//define the arguments through comments
void LuaManager::registerFunction(std::string funcName,luaFunction funcPtr)
{
	lua_register(luaState,funcName.c_str(),funcPtr);
}

LuaManager::~LuaManager()
{
	//clean up lua.
	lua_close(luaState);
}