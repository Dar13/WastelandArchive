
#ifndef _XML_READER_H_
#define _XML_READER_H_

#include <OgreSingleton.h>
#include "Tree.h"

class XMLReader : public Ogre::Singleton<XMLReader>
{
public:
	XMLReader();

	void readFile(std::string filename, Node* root);

private:
	

	XMLReader(const XMLReader&);
	XMLReader& operator=(const XMLReader&);
};

#endif