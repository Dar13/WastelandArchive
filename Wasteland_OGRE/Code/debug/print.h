#include "StdAfx.h"

#ifndef _DEBUG_PRINT_
#define _DEBUG_PRINT_

class DebugPrint : Ogre::Singleton
{
public:
	DebugPrint();
	~DebugPrint();

	void printVar(std::string text);

	void Setup(Ogre::SceneManager* scene); //required.
	void Update();
	void Clean();
	
	bool isVisible();
	void setVisible(bool visible = true);

private:
	DebugPrint(const DebugPrint&){}
	DebugPrint& operator= (const DebugPrint&);

	bool _visible;
	
	//ogre stuff
	Ogre::Rectangle2D* _rectangle;
	Ogre::SceneNode* _node;
	Ogre::OverlayElement* _textBox;
	Ogre::Overlay* _overlay;

	int _printCount;
	std::vector<std::string> _prints;
}

#endif