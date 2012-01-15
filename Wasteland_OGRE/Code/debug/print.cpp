#include "StdAfx.h"
#include "print.h"

#include "..\OgreManager.h"

template<> DebugPrint* Ogre::Singleton<DebugPrint>::ms_Singleton = 0;

DebugPrint::DebugPrint()
{
	_textBox = NULL;
	_rectangle = NULL;
	_overlay = NULL;
}

DebugPrint::~DebugPrint()
{
	//cleanup
}

//!Has to be called from separate app states. Requires scene manager
void DebugPrint::Setup(Ogre::SceneManager* scene)
{
	_rectangle = new Ogre::Rectangle2D(true);
	_rectangle->setCorners(-1,1,1,0);
	_rectangle->setMaterial("debug/print");
	_rectangle->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
	_rectangle->setBoundingBox(Ogre::AxisAlignedBox(-100000.0*Ogre::Vector3::UNIT_SCALE,100000.0*Ogre::Vector3::UNIT_SCALE));
	_node = scene->getRootSceneNode()->createChildSceneNode("debugPrint");
	_node->attachObject(_rectangle);

	_textBox = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea","DbgPrintText");
	_textBox->setCaption("");
	_textBox->setMetricsMode(Ogre::GMM_RELATIVE);
	_textBox->setPosition(0,0);
	_textBox->setParameter("font_name","Console");
	_textBox->setParameter("colour_top","1 1 1");
	_textBox->setParameter("colour_bottom","1 1 1");
	_textBox->setParameter("char_height","0.03");

	_overlay = Ogre::OverlayManager::getSingleton().create("DbgPrint");
	_overlay->add2D((Ogre::OverlayContainer*)_textBox);
	_overlay->show();
}

void DebugPrint::Clean()
{
	delete _rectangle;
	delete _node;
	delete _textBox;
	delete _overlay;
}

void DebugPrint::printVar(std::string text)
{
	//tell the update function to print something.
	_printCount++;
	//hold the requested printed string in a vector
	_prints.push_back(text);
}

void DebugPrint::Update()
{
	if(_printCount == 0)
	{
		return;
	}
	_textBox->setCaption(""); //clears the textbox, nothing sticks around
	size_t vSize = _prints.size();
	std::string bigString = "";
	for(size_t i = 0; i<vSize; ++i)
	{
		//compile into one big string for caption.
		bigString.append(_prints[i]);
		bigString.append("\n");
	}
	_textBox->setCaption(bigString);
	_prints.clear(); //reset vector
}