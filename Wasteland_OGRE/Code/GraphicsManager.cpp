#include "StdAfx.h"

#include "GraphicsManager.h"

GraphicsManager::GraphicsManager()
{
	_Root=0;
	_Window=0;
	_Log=0;
	_Timer=0;
	_windowHandle = 0;
}

bool GraphicsManager::Setup()
{
	Ogre::LogManager* logMgr = NULL;
	bool retVal=true;
	try{
	//Set up the Ogre logging system
	logMgr = new Ogre::LogManager();
	_Log=Ogre::LogManager::getSingleton().createLog("Ogre.log",true,true);
	_Log->setLogDetail(Ogre::LL_BOREME);
	_Log->setDebugOutputEnabled(true);

	//Ogre root setup
	_Root = new Ogre::Root("","");

	//Locking in Direct3D9, just for convenience's sake.
#if defined(_DEBUG)
	_Root->loadPlugin("RenderSystem_Direct3D9_d");
#else
	_Root->loadPlugin("RenderSystem_Direct3D9");
#endif

	//rendersystem crap
	const Ogre::RenderSystemList &renderSystem = _Root->getAvailableRenderers();
	Ogre::RenderSystemList::const_iterator &r_it = renderSystem.begin();
	_Root->setRenderSystem(*r_it);
	_Root->initialise(false);

	//Load the rest of the plugins(May add more later on).
#if defined(_DEBUG)
	_Root->loadPlugin("Plugin_CgProgramManager_d");
	_Root->loadPlugin("Plugin_OctreeSceneManager_d");
#else
	_Root->loadPlugin("Plugin_CgProgramManager");
	_Root->loadPlugin("Plugin_OctreeSceneManager");
#endif

	//Will have this read-in from a config file, but for now it'll be hard-coded.
	Ogre::NameValuePairList options;
	options["resolution"] = "1920x1080";
	options["fullscreen"] = "false";
	options["vsync"] = "true";
	options["FSAAHint"] = "Quality";
	options["FSAA"] = "4x";
	options["monitorIndex"] = "0";

	_Window = _Root->createRenderWindow("WasTeLanD - DEBUG",1920,1080,false,&options);
	
	//Leave the SceneManager, Camera/Viewport stuff for the appstates to deal with.

	//Get the window handle, need it for OIS.
	_Window->getCustomAttribute("WINDOW",&_windowHandle);

	_Timer = _Root->getTimer();
	}
	catch(Ogre::Exception& e)
	{
		retVal = false;
		MessageBoxA(NULL,e.getFullDescription().c_str(),"EXCEPTION!",MB_OK | MB_ICONERROR);
		if(logMgr != NULL)
		{
			delete logMgr;
		}
	}
	catch(std::bad_alloc &ba)
	{
		OutputDebugString(ba.what());
		if(logMgr != NULL)
		{
			delete logMgr;
		}
	}

	_Root->addFrameListener(this);

	return retVal;
}

bool GraphicsManager::addResources(std::string& filename)
{
	//return value
	bool retVal=true; //honestly don't know what to check for to make this false. Catch an exception?

	//Takes an XML struct that holds all the files that refer to specific resource locations.
	//i.e. general_resgroup.xml holds the information for the General resource group for Ogre.

	//no idea if this'll work(the exception catching that is).
	list_t* reslist = list(filename).release();

	for(list_t::file_const_iterator itr = reslist->file().begin(); itr != reslist->file().end(); ++itr)
	{
		//have to make sure to delete this pointer.
		resource_t* res = resource((*itr)).release(); //get pointer to resource to load.
		std::string grpName = res->GroupName(); //get the group name.
		for(resource_t::location_const_iterator inner_itr = res->location().begin(); inner_itr != res->location().end(); ++inner_itr)
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation((*inner_itr).FileName(),(*inner_itr).Type(),grpName,(*inner_itr).Recursive());
		}
		delete res;
	}
	delete reslist;

	//Initialize resGroups.
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return retVal;
}

bool GraphicsManager::Render()
{
	bool retVal=true;
	try
	{
		_Root->renderOneFrame();
	}
	catch(Ogre::Exception& e)
	{
		//Not going to proclaim it, just silently shutdown.
		//MessageBoxA(NULL,e.getFullDescription().c_str(),"Exception!",MB_OK | MB_ICONERROR); //this is proclaiming it.
		OutputDebugString(e.getFullDescription().c_str());
		retVal=false;
	}

	return retVal;
}

void GraphicsManager::Shutdown()
{
	clearAnimationStates();
	_Root->shutdown();
	delete _Root;
}

/*
Options list: (defined in object.xsd)
note: most all properties are floats or strings. Only special fields get integers.
*/

Ogre::SceneNode* GraphicsManager::createSceneNode(Ogre::SceneManager* scene, 
								 object_t* objectInfo,Ogre::SceneNode* parent)
{
	Ogre::SceneNode* node = NULL;
	
	//gets node type.
	std::string type = objectInfo->type();
	//get position information
	Ogre::Vector3 pos;
	pos.x = objectInfo->positionX();
	pos.y = objectInfo->positionY();
	pos.z = objectInfo->positionZ();
	//get orientation information
	Ogre::Vector3 point;
	point.x = objectInfo->pointX();
	point.y = objectInfo->pointY();
	point.z = objectInfo->pointZ();
	
	//create node.
	if(parent == NULL)
	{
		node = scene->getRootSceneNode()->createChildSceneNode("node" + objectInfo->name(),pos);
	}
	else
	{
		node = parent->createChildSceneNode("node" + objectInfo->name(),pos);
	}
	//that way, it doesn't default to looking at the origin.
	if(point != Ogre::Vector3::ZERO)
	{
		node->lookAt(point,Ogre::SceneNode::TS_WORLD);
	}

	if(type == "entity")
	{
		Ogre::Entity* ent = scene->createEntity("ent" + objectInfo->name(),objectInfo->fileName(),objectInfo->resGroup());
		ent->setCastShadows(objectInfo->shadows());
		node->attachObject(ent);
	}

	if(type == "light")
	{
		Ogre::Light* light = scene->createLight("light" + objectInfo->name());
		//you'd think this would be easy to do. Instead there's a whole function for it.
		setLightRange(light,(Ogre::Real)objectInfo->lightRadius());
		//set the color.
		light->setDiffuseColour(Ogre::ColourValue((Ogre::Real)objectInfo->lightColorRed(),
												  (Ogre::Real)objectInfo->lightColorGreen(),
												  (Ogre::Real)objectInfo->lightColorBlue(),1.0f));
		//dunno if this is needed.
		switch(objectInfo->lightType())
		{
		case Ogre::Light::LT_POINT:
			//in case of needing to set other properties later on.
			break;
		case Ogre::Light::LT_SPOTLIGHT:
			//in case of needing to set other properties later on.
			break;
		case Ogre::Light::LT_DIRECTIONAL:
			//in case of needing to set other properties later on.
			break;
		};
	}

	if(objectInfo->childName() != "NULL")
	{
		//add the node whose name == childName(if it exists) to the current node's children list.
		Ogre::Node* child = scene->getRootSceneNode()->removeChild(objectInfo->childName());
		node->addChild(child);
	}

	return node;
}

Ogre::SceneNode* GraphicsManager::createSceneNode(Ogre::SceneManager* scene,weapon_t* objectInfo,Ogre::SceneNode* parent)
{
	Ogre::SceneNode* node = NULL;

	Ogre::Vector3 pos = Ogre::Vector3::ZERO;
	pos.y = 1.5f;

	if(parent == NULL)
	{
		node = scene->getRootSceneNode()->createChildSceneNode("node" + objectInfo->name(),pos);
	}
	else
	{
		node = parent->createChildSceneNode("node" + objectInfo->name(),pos);
	}

	//only used for weapons(look at the parameter types, dummy)
	Ogre::Entity* ent = scene->createEntity("ent" + objectInfo->name(),objectInfo->model(),"Models");
	ent->setCastShadows(true);
	node->attachObject(ent);

	return node;
}

//This works, but isn't documented very well...
//Might come back and document it later, but not right now...
//In other words...!!!MAGIC DON'T TOUCH!!!
void GraphicsManager::getMeshInformation(const Ogre::MeshPtr* const meshptr,
                        size_t &vertex_count,
                        Ogre::Vector3* &vertices,
                        size_t &index_count,
                        unsigned long* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

	Ogre::Mesh* mesh = 0;
	if(!meshptr->isNull())
	{
		mesh = meshptr->getPointer();
	}
	else
	{
		return;
	}
 
    vertex_count = index_count = 0;
 
    // Calculate how many vertices and indices we're going to need
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
        // Add the indices
        index_count += submesh->indexData->indexCount;
    }
 
    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];
 
    added_shared = false;
 
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
 
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
 
            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //Ogre::Real* pReal;
            float* pReal;
 
            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }
 
            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }
 
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
 
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
 
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
 
        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                                          static_cast<unsigned long>(offset);
            }
        }
 
        ibuf->unlock();
        current_offset = next_offset;
    }
}

//Framelistener methods and any helpers that affect them.
void GraphicsManager::addAnimationState(Ogre::AnimationState* anim)
{
	_animations.push_back(anim);
}

void GraphicsManager::removeAnimationState(Ogre::AnimationState* anim)
{
	for(std::vector<Ogre::AnimationState*>::iterator itr = _animations.begin(); itr != _animations.end(); ++itr)
	{
		if( (*itr) == anim )
		{
			_animations.erase(itr);
			itr = _animations.end();
		}
	}
	//alternate?
	/*
	auto itr = std::find(_animations.begin(),_animations.end(),anim);
	if(itr != _animations.end())
	{
		_animations.erase(itr);
	}
	*/
}

void GraphicsManager::clearAnimationStates()
{
	_animations.clear();
}

bool GraphicsManager::frameStarted(const Ogre::FrameEvent& evt)
{
	//update any animations that need it.
	for(std::vector<Ogre::AnimationState*>::iterator itr = _animations.begin(); itr != _animations.end(); ++itr)
	{
		(*itr)->addTime(evt.timeSinceLastFrame);
	}

	return true;
}

bool GraphicsManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	return true;
}

bool GraphicsManager::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}

void GraphicsManager::setLightRange(Ogre::Light* l, Ogre::Real range)
{
	l->setAttenuation(range,(Ogre::Real)1.0f,(Ogre::Real)4.5f/range,75.0f/(range*range));
	return;
}

Ogre::ColourValue GraphicsManager::getColorFromHex(int hexColor, float alpha)
{
	float r,g,b,a;
	a = 1.0f; //no transparency.

	r = ((hexColor >> 16) & 0xFF) / 255.0f;
	g = ((hexColor >> 8) & 0xFF) / 255.0f;
	b = (hexColor & 0xFF) / 255.0f;

	return Ogre::ColourValue(r,g,b,a);
}

Ogre::Quaternion GraphicsManager::eulerToQuat(Ogre::Radian rX,Ogre::Radian rY,Ogre::Radian rZ)
{
	Ogre::Quaternion qX,qY,qZ,qT;
	qX.FromAngleAxis(rX,Ogre::Vector3::UNIT_X);
	qY.FromAngleAxis(rY,Ogre::Vector3::UNIT_Y);
	qZ.FromAngleAxis(rZ,Ogre::Vector3::UNIT_Z);

	qT = qX * qY;
	return (qT * qZ);
}

//I really hope this works. Pain in the ass if it doesn't.
//Returns true if pointP is within triangle ABC.
//Takes x/z coordinates of 3D vector.
bool GraphicsManager::isPointInTriangle2D(const Ogre::Vector3& pointA,const Ogre::Vector3& pointB,const Ogre::Vector3& pointC,const Ogre::Vector3& pointP)
{
	Ogre::Vector3 u,v,w,crossW,crossU,crossV;
	u = pointB - pointA;
	v = pointC - pointA;
	w = pointP - pointA;
	crossW = v.crossProduct(w);
	crossU = v.crossProduct(u);

	if(crossW.dotProduct(crossU) < 0)
		return false;

	crossW = u.crossProduct(w);
	crossV = u.crossProduct(v);

	if(crossW.dotProduct(crossV) < 0)
		return false;

	float denom = crossV.length();
	float r = crossW.length() / denom;
	float t = (u.crossProduct(w).length())/denom;

	return (r <= 1 && t<=1 && (r+t) <= 1);
}

//--------------------------------------------------------------------------------------
//http://www.ogre3d.org/tikiwiki/FadeEffectOverlay
ScreenFader::ScreenFader(const char* overlayName,const char* materialName, ScreenFaderCallback* callback)
{
	try
	{
		_fadeOperation = FADE_NONE;
		_alpha = 0.0;
		_callback = callback;

		Ogre::ResourcePtr resPtr = Ogre::MaterialManager::getSingleton().getByName(materialName);
		Ogre::Material* mat = dynamic_cast<Ogre::Material*>(resPtr.getPointer());

		_textureUnit = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);

		_overlay = Ogre::OverlayManager::getSingleton().getByName(overlayName);
		_overlay->hide();
	}
	catch(Ogre::Exception& e)
	{
		std::cout << "Fader Exception(Ogre) : " << e.what() << std::endl;
	}
	catch(std::exception& e)
	{
		std::cout << "Fader Exception(Unknown) : " << e.what() << std::endl;
	}
}

ScreenFader::~ScreenFader() {}

void ScreenFader::startFadeIn(double duration)
{
	if(duration < 0)
	{
		duration = -duration;
	}
	if(duration < 0.00001)
	{
		duration = 1.0;
	}

	_alpha = 1.0;
	_totalDuration = duration;
	_currentDuration = duration;
	_fadeOperation = FADE_IN;
	_overlay->show();
}

void ScreenFader::startFadeOut(double duration)
{
	if(duration < 0)
	{
		duration = -duration;
	}
	if(duration < 0.00001)
	{
		duration = 1.0;
	}

	_alpha = 0.0;
	_totalDuration = duration;
	_currentDuration = 0.0;
	_fadeOperation = FADE_OUT;
	_overlay->show();
}

void ScreenFader::fade(double timeSinceLastFrame)
{
	if(_fadeOperation != FADE_NONE && _textureUnit)
	{
		_textureUnit->setAlphaOperation(Ogre::LBX_MODULATE,
										Ogre::LBS_MANUAL,
										Ogre::LBS_TEXTURE,
										static_cast<Ogre::Real>(_alpha));

		int timeMult = (_fadeOperation == FADE_IN) ? (-1) : 1;
		_currentDuration += (timeMult * timeSinceLastFrame);

		_alpha = _currentDuration / _totalDuration;
		if(_alpha < 0.0 && timeMult < 0)
		{
			//FADE_IN
			_overlay->hide();
			_fadeOperation = FADE_NONE;
			if(_callback)
			{
				_callback->fadeInCallback();
			}
		}
		else if(_alpha > 1.0f && timeMult > 0)
		{
			//FADE_OUT
			_overlay->hide();
			_fadeOperation = FADE_NONE;
			if(_callback)
			{
				_callback->fadeOutCallback();
			}
		}
		else
		{
			//fade still going on
			if(_callback)
			{
				_callback->updateFade(_currentDuration / _totalDuration);
			}
		}
	}
}