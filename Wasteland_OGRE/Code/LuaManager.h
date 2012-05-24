#include <lua.hpp>

#include <OgreSingleton.h>

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

	void Setup(std::string luaListFileName);

	void registerFunction(std::string funcName,luaFunction funcPtr);

	void callFunction(std::string funcName);

private:
	lua_State* luaState;

	LuaManager(const LuaManager&);
	LuaManager& operator=(const LuaManager&);
};