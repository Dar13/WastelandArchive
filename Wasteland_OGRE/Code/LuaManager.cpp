#include "StdAfx.h"

#include "LuaManager.h"

#include "interfaces\interfaces.h"

#include "AI\npc_character.h"
#include "AI\enemy_character.h"

template<> LuaManager* Ogre::Singleton<LuaManager>::ms_Singleton = 0;

LuaManager::LuaManager()
{
	luaState = nullptr;
}

lua_State* LuaManager::getLuaState() { return luaState;}

//Forces explicit setup.
void LuaManager::Setup(std::string luaListFileName)
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);
	luaL_checkversion(luaState);
	
	//register lua-accessible functions
	registerFunction("activate",activate);
	registerFunction("changeEntityName",changeEntityName);
	registerFunction("printDebug",printDebug);
	registerFunction("distanceCheck",distanceCheck);
	registerFunction("getPlayerPosition",getPlayerPosition);
	registerFunction("getEntityPosition",getEntityPosition);
	registerFunction("getNearestEntity",getNearestEntity);

	//exposes all Lua functions to luaState and thus to the program itself through the LuaManager.
	list_t* llist = list(luaListFileName).release();
	std::unique_ptr<list_t> luaList(llist);
	for(list_t::file_const_iterator itr = luaList->file().begin(); itr != luaList->file().end(); ++itr)
	{
		if(luaL_dofile(luaState,(*itr).c_str()))
		{
			std::cout << "Lua Error!" << std::endl;
		}
	}

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
	if(lua_isnil(luaState,-1)) { std::cout << "Lua can't find function name!" << std::endl; }
	if(!lua_isfunction(luaState,-1)) { std::cout << "Lua function not pushed to stack!!" << std::endl; }
}

void LuaManager::callFunction(int paramNum,int retNum)
{
	int err = lua_pcall(luaState,paramNum,retNum,0);
	if(err != 0) 
	{ 
		std::cout << "Lua Error! Code: " << err << std::endl;
		if(lua_isstring(luaState,-1))
		{
			std::cout << lua_tostring(luaState,-1) << std::endl;
		}
	}
	
}
//<- Grouped functions

void LuaManager::pushFunctionArg(boost::variant<int,double,std::string> arg)
{
	argVisitor visit;
	boost::apply_visitor(visit,arg);

	bool num,rat,str;
	visit.getType(&num,&rat,&str);
	if(num) { lua_pushnumber(luaState,visit.numeric); }
	if(rat) { lua_pushnumber(luaState,visit.rational); }
	if(str) { lua_pushstring(luaState,visit.string.c_str()); }

	return;
}

void LuaManager::pushFunctionArgVector(const Ogre::Vector3& vector)
{
	lua_newtable(luaState);
	int top = lua_gettop(luaState);

	//next two lines in psuedo code: table[1] = vector.x;
	//the next values would then be like: table[2] = vector.y;
	for(int i = 0; i < 3; ++i)
	{
		lua_pushnumber(luaState,i+1);
		lua_pushnumber(luaState,vector[i]);
		lua_settable(luaState,top);
		
	}
}

void LuaManager::pushFunctionArgVector(const btVector3& vector)
{
	lua_newtable(luaState);
	int top = lua_gettop(luaState);

	for(int i = 0; i < 3; ++i)
	{
		lua_pushnumber(luaState,i+1);
		lua_pushnumber(luaState,vector.m_floats[i]);
		lua_settable(luaState,top);
	}
}

void LuaManager::addEntity(const std::string& name,LevelData::BaseEntity* entity)
{
	std::pair<std::string,LevelData::BaseEntity*> p;

	p.first = name;
	p.second = entity;

	_entities.insert(p);

	return;
}

void LuaManager::purgeEntities()
{
	_entities.clear();
}

void LuaManager::activateEntity(const std::string& name,bool value)
{
	_entities[name]->activate(value);
}

void LuaManager::addDataPointer(const std::string& name,void* dataPtr)
{
	_data[name] = dataPtr;
}

void LuaManager::removeDataPointer(const std::string& name)
{
	auto found = _data.find(name);
	if(found == _data.end())
	{
		return;
	}
	else
	{
		_data.erase(found);
	}
}

void LuaManager::purgeData()
{
	_data.clear();
}

void* LuaManager::_getData(const std::string& name)
{
	auto found = _data.find(name);
	if(found != _data.end())
	{
		return (*found).second;
	}
	else
	{
		return NULL;
	}
}

LuaManager::~LuaManager()
{
	//clean up lua.
	lua_close(luaState);
}

//==============================

void argVisitor::getType(bool* num,bool* rat,bool* str)
{
	*num = false;
	*rat = false;
	*str = false;
	if(numeric != 0)
	{
		*num = true;
		return;
	}
	
	if( fabs(rational - 0.0) > std::numeric_limits<double>::epsilon())
	{
		*rat = true;
		return;
	}

	if(string != "")
	{
		*str = true;
		return;
	}

	return;
}

void argVisitor::operator()(const int& i)
{
	numeric = i;
}

void argVisitor::operator()(const std::string& str)
{
	string = str;
}

void argVisitor::operator()(const double& d)
{
	rational = d;
}

//=============================

Ogre::Vector3 getVectorFromLua(lua_State* lua,int tableIndex)
{
	if(lua_istable(lua,tableIndex))
	{
		Ogre::Vector3 ret;
		for(int i = 1; i < 4; ++i)
		{
			lua_pushnumber(lua,i);
			lua_gettable(lua,tableIndex);
			ret[i-1] = static_cast<float>(lua_tonumber(lua,-1));
			lua_pop(lua,1);
		}

		return ret;
	}
	else
	{
		std::cout << "Lua Error: Index provided is not a table." << std::endl;
		return Ogre::Vector3::ZERO;
	}
}

//==============================

// var = activate(entName,value)
int activate(lua_State* lua)
{
	//number of arguments. NOT ZERO-INDEXED
	int argNum = lua_gettop(lua);

	std::string entName;
	bool value = true;

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

// var = changeEntityName(oldName,newName)
int changeEntityName(lua_State* lua)
{
	int argNum = lua_gettop(lua);

	std::string oldName,newName;

	if(argNum == 2)
	{
		if(lua_isstring(lua,1))
		{
			oldName = lua_tostring(lua,1);
		}
		else
		{
			lua_pushboolean(lua,0);
			return 0;
		}

		if(lua_isstring(lua,2))
		{
			oldName = lua_tostring(lua,2);
		}
		else
		{
			lua_pushboolean(lua,0);
			return 0;
		}
	}

	LevelData::BaseEntity* entity = LuaManager::getSingleton().getEntity(oldName);
	LuaManager::getSingleton().removeEntity(oldName);
	LuaManager::getSingleton().addEntity(newName,entity);
	entity->setName(newName);

	lua_pushboolean(lua,1);

	return 1;
}

// printDebug(...)
int printDebug(lua_State* lua)
{
	int argNum = lua_gettop(lua);

	std::string str;

	for(int i = 1; i <= argNum; ++i)
	{
		if(lua_isstring(lua,i))
		{
			str = lua_tostring(lua,i);
			std::cout << str;
		}
		else
		{
			if(lua_isnumber(lua,i) || lua_isboolean(lua,i))
			{
				double num = lua_tonumber(lua,i);
				std::cout << num;
			}
			else
			{
				if(lua_istable(lua,i))
				{
					std::cout << "(table)";
				}
				else
				{
					if(lua_isnil(lua,i))
					{
						std::cout << "(nil)";
					}
					else
					{
						std::cout << "(not printable)";
					}
				}
			}
		}
	}

	std::cout << std::endl;
	
	return 0;
}

int dummy(lua_State* lua)
{
	std::cout << "Dummy function..." << std::endl;
	return 0;
}

// var = distanceCheck(v1,v2,dist)
int distanceCheck(lua_State* lua)
{
	Ogre::Vector3 v1,v2;
	double dist;
	bool success = true;

	v1 = getVectorFromLua(lua,1);

	v2 = getVectorFromLua(lua,2);

	if(v1 == Ogre::Vector3::ZERO || v2 == Ogre::Vector3::ZERO)
	{
		std::cout << "Lua Error: distanceCheck parameter error." << std::endl;
		success = false;
	}

	if(lua_isnumber(lua,3))
	{
		dist = lua_tonumber(lua,3);
		success = dist > v1.squaredDistance(v2);
	}
	else
	{
		std::cout << "Script error: distanceCheck called with non-numeric parameter. Parameter #:3" << std::endl;
		success = false;
	}

	//std::cout << success << std::endl;
	lua_pushinteger(lua,static_cast<int>(success));

	return 1;
}

// var = getPlayerPosition()
int getPlayerPosition(lua_State* lua)
{
	Ogre::Vector3 pos;
	void* ptr = NULL;

	//some error checking...
	ptr = LuaManager::getSingleton()._getData("playerPosition");
	if(ptr != NULL)
	{
		pos = *(static_cast<Ogre::Vector3*>(ptr));
	}
	else
	{
		pos = Ogre::Vector3::ZERO;
	}

	lua_newtable(lua);
	int top = lua_gettop(lua);

	for(int i = 0; i < 3; ++i)
	{
		lua_pushinteger(lua,i+1);
		lua_pushnumber(lua,pos[i]);
		lua_settable(lua,top);
	}

	return 1;
}

// var = getEntityPosition("entityName","entityType")
int getEntityPosition(lua_State* lua)
{
	Ogre::Vector3 ret;

	if(lua_isstring(lua,1) && lua_isstring(lua,2))
	{
		std::string entity = lua_tostring(lua,1);
		LevelData::BaseEntity* ent = LuaManager::getSingleton().getEntity(entity);
		std::string type = lua_tostring(lua,2);
		if(type == "NPC")
		{
			ret = static_cast<NPCCharacter*>(ent)->getPosition();
		}
		if(type == "Enemy")
		{
			//ret = static_cast<EnemyCharacter*>(ent)->getPosition();
		}
		//..other types?
	}

	lua_newtable(lua);
	int top = lua_gettop(lua);

	for(int i = 0; i < 3; i++)
	{
		lua_pushinteger(lua,i+1);
		lua_pushnumber(lua,ret[i]);
		lua_settable(lua,top);
	}

	return 1;
}

// var = getNearestEntity(position,type)
int getNearestEntity(lua_State* lua)
{
	Ogre::Vector3 position;

	if(lua_istable(lua,1))
	{
		position = getVectorFromLua(lua,1);
	}
	else
	{
		position = Ogre::Vector3::ZERO;
	}

	if(position == Ogre::Vector3::ZERO)
	{
		lua_pushnil(lua);
		return 1;
	}

	std::string type = lua_tostring(lua,2);
	if(type == "" || type.find("Zone") != type.npos)
	{
		lua_pushnil(lua);
		return 1;
	}

	int ltype;
	if(type == "NPC") { ltype = LevelData::NPC; }
	if(type == "Enemy") { ltype = LevelData::ENEMY; }
	if(type == "Door") { ltype = LevelData::DOOR; }

	std::string closest = "";
	Ogre::Vector3 entPos;
	float closestDist = 500.0f,dist = 500.0f;
	std::map<std::string,LevelData::BaseEntity*>* ents = LuaManager::getSingleton()._getEntities();
	auto itr = ents->begin();
	while(itr != ents->end())
	{
		if(itr->second->getType() == ltype)
		{
			switch(ltype)
			{
			case LevelData::NPC:
				entPos = static_cast<NPCCharacter*>(itr->second)->getPosition();
				dist = position.squaredDistance(entPos);
				if(dist < closestDist && dist != 0.0f)
				{
					closest = itr->second->getName();
					closestDist = dist;
				}
				break;
			case LevelData::ENEMY:
				//dist = position.squardedDistance(static_cast<EnemyCharacter*>(itr->second)->getPosition());
				if(dist < closestDist && dist != 0.0f)
				{
					closest = itr->second->getName();
					closestDist = dist;
				}
				break;
			case LevelData::DOOR:
				//do this later.
				break;
			}
		}

		++itr;
	}

	lua_pushstring(lua,closest.c_str());

	return 1;
}