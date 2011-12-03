
#ifndef _XML_READER_H_
#define _XML_READER_H_

#include <OgreSingleton.h>
#include "Tree.h"
#include "interfaces\entity.hxx"
#include "interfaces\object.hxx"

#include <iostream>

class XMLReader : public Ogre::Singleton<XMLReader>
{
public:
	XMLReader();

	//! Returns void pointer, to enable some flexibility in passing back object pointers. Just have to cast it back to the desired class [(object_t*)ptr]
	void* readFile(std::string filename,const std::string& typeToLoad);

private:
	

	XMLReader(const XMLReader&);
	XMLReader& operator=(const XMLReader&);
};

#endif