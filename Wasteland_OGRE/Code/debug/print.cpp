#include "StdAfx.h"
#include "print.h"

#include "..\OgreManager.h"

template<> DebugPrint* Ogre::Singleton<DebugPrint>::ms_Singleton = 0;

DebugPrint::DebugPrint()
{
	_textBox = NULL;
	_rectangle = NULL;
	_overlay = NULL;
	_node = NULL;
	_visible = false;
	_used = false;
	_printCount = 0;
}

DebugPrint::~DebugPrint()
{
	//cleanup
	Clean();
}

//!Has to be called from separate app states. Requires scene manager
void DebugPrint::Setup(Ogre::SceneManager* scene)
{
	_used = true;

	_rectangle = new Ogre::Rectangle2D(true);
	_rectangle->setCorners(-1,1,1,.5);
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
	if(_used)
	{
		_used = false;
		if(_rectangle)
		{
			delete _rectangle;
		}
		if(_node)
		{
			//taken care of by app-state scene manager.
			//delete _node;
		}
		//thank god I'm using CEGUI for a GUI. This would really screw me up.
		Ogre::OverlayManager::getSingleton().destroyAll();
	}
}

//Private, so it can't be abused.
//Saves a couple of string copy-overs and such.
void DebugPrint::printVar()
{
	//All but std::string overload eventually comes here.
	//making sure it counts
	_printCount = 1;
	//assumes usage of internal conversion string
	_prints.push_back(convString);
	convString="";
	convStream.str("");
}

void DebugPrint::printVar(std::string text)
{
	//tell the update function to print something.
	_printCount = 1;
	//hold the requested printed string in a vector
	_prints.push_back(text);
}

void DebugPrint::printVar(int integer)
{
	//convert into string.
	//clear the stream first.
	convStream.clear();
	convStream << integer;
	convString = convStream.str();
	//conversion done, moving on.
	printVar();	
}

void DebugPrint::printVar(float num)
{
	//convert into string...
	convStream << num;
	convString = convStream.str();
	//conversion done, send to std::string overload
	printVar();
}

void DebugPrint::printVar(bool flag)
{
	//basically true or false.
	if(flag)
	{
		printVar("true");
	}
	else
	{
		printVar("false");
	}
}

void DebugPrint::printVar(const char* strPtr)
{
	//makes it easy for us.
	convString.assign(strPtr);
	printVar();
}

void DebugPrint::printVar(void* vector3,int vecType)
{
	Ogre::Vector3 tmpO;
	btVector3 tmpB;
	float x,y,z;
	switch(vecType)
	{
	case OGRE:
		tmpO = *((Ogre::Vector3*)vector3);
		x = tmpO.x;
		y = tmpO.y;
		z = tmpO.z;
		break;
	case BULLET:
		tmpB = *((btVector3*)vector3);
		x = tmpB.x();
		y = tmpB.y();
		z = tmpB.z();
		break;
	default:
		//invalid type, exit.
		return;
		break;
	};

	//convert numbers into string
	convStream << x;
	convStream << ",";
	convStream << y;
	convStream << ",";
	convStream << z;
	convString = convStream.str();
	printVar();
}

void DebugPrint::Update()
{
	if(_visible != _rectangle->isVisible())
	{
		//update rectangle(?) visibility
		_rectangle->setVisible(_visible);
	}
	if(_printCount == 0)
	{
		_visible = false;
		return;
	}
	else
	{
		_visible = true;
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