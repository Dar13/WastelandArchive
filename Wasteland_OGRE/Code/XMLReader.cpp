#include "XMLReader.h"

template<> XMLReader* Ogre::Singleton<XMLReader>::ms_Singleton = 0;

void* XMLReader::readFile(std::string filename,const std::string& typeToLoad)
{
	if(typeToLoad == "Object")
	{
		std::auto_ptr<object_t> obj = object(filename);
		object_t* objPtr = obj.get();
		obj.reset(NULL); //no destructor called.
		return (void*)objPtr;
	}
	//Just a catch-all if statement.
	if(typeToLoad == "NULL" || typeToLoad == "")
	{
		return NULL;
	}
}