#include "StdAfx.h"
#include "XMLReader.h"

template<> XMLReader* Ogre::Singleton<XMLReader>::ms_Singleton = 0;

void* XMLReader::readFile(std::string filename,const std::string& typeToLoad)
{
	if(typeToLoad == "object")
	{
		std::auto_ptr<object_t> obj = object(filename);
		object_t* objPtr = obj.release();
		obj.reset(NULL); //no destructor called.
		return (void*)objPtr;
	}
	else if(typeToLoad == "list")
	{
		std::auto_ptr<list_t> l = list(filename);
		list_t* listPtr = l.release();
		l.reset(NULL);
		return (void*)listPtr;
	}
	//Just a catch-all if statement.
	else
	{
		return NULL;
	}
}