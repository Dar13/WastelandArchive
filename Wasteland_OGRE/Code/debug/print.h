#include "StdAfx.h"

#ifndef _DEBUG_PRINT_
#define _DEBUG_PRINT_

enum DBG_VECTOR_TYPE
{
	NONE = 0,
	OGRE,
	BULLET
};

class DebugPrint : public Ogre::Singleton<DebugPrint>
{
public:
	DebugPrint();
	~DebugPrint();

	void printVar(std::string text);
	void printVar(float num);
	void printVar(int integer);
	void printVar(bool flag);
	void printVar(const char* strPtr);
	void printVar(void* vector3,int vecType);

	void Setup(Ogre::SceneManager* scene); //required.
	void Update();
	void Clean();

	bool isActive(){return _used;}
	
	bool isVisible();
	void setVisible(bool visible = true);

private:
	void printVar(); //assumes usage of internal convString variable
	DebugPrint(const DebugPrint&){}
	DebugPrint& operator= (const DebugPrint&);

	bool _visible;
	
	bool _used;
	
	//ogre stuff
	Ogre::Rectangle2D* _rectangle;
	Ogre::SceneNode* _node;
	Ogre::OverlayElement* _textBox;
	Ogre::Overlay* _overlay;

	//conversion thingies
	std::stringstream convStream;
	std::string convString;

	int _printCount;
	std::vector<std::string> _prints;
};

#endif