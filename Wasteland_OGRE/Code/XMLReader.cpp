#include "XMLReader.h"

template<> XMLReader* Ogre::Singleton<XMLReader>::ms_Singleton = 0;

void XMLReader::readFile(std::string filename,Node* root)
{
	
}