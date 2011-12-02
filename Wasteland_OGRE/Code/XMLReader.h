
#ifndef _XML_READER_H_
#define _XML_READER_H_

#include <tinyxml.h>
#include <OgreSingleton.h>

class XMLReader : public Ogre::Singleton<XMLReader>
{
public:
	XMLReader();

	void readFile(std::string fileName , std::map<std::string,std::string>& container);

private:
	XMLReader(const XMLReader&);
	XMLReader& operator=(const XMLReader&);
};

#endif