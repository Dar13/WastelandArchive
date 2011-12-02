#include "OgreManager.h"

template<> OgreManager* Ogre::Singleton<OgreManager>::ms_Singleton=0;

OgreManager::OgreManager()
{
	_Root=0;
	_Window=0;
	_Log=0;
	_Timer=0;
}

bool OgreManager::Setup()
{
	bool retVal=true;
	try{
	//Set up the Ogre logging system
	Ogre::LogManager* logMgr = new Ogre::LogManager();
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
	options["resolution"] = "1280x1024";
	options["fullscreen"] = "false";
	options["vsync"] = "false";

	_Window = _Root->createRenderWindow("WasTeLanD - DEBUG",1280,1024,false,&options);
	
	//Leave the SceneManager, Camera/Viewport stuff for the appstates to deal with.

	//Get the window handle, need it for OIS.
	_Window->getCustomAttribute("WINDOW",&_windowHandle);

	_Timer = _Root->getTimer();
	}
	catch(Ogre::Exception& e)
	{
		retVal = false;
		MessageBoxA(NULL,e.getFullDescription().c_str(),"EXCEPTION!",MB_OK | MB_ICONERROR);
	}

	return retVal;
}

bool OgreManager::addResources(std::vector<Ogre::String> &location,std::vector<Ogre::String> &type,std::vector<Ogre::String> &name)
{
	//return value
	bool retVal=true;

	//Invalid inputs
	if(location.size()!=type.size() || location.size()!=name.size())
	{
		return false;
	}

	//Let's make sure this doesn't crash the program eh?
	try
	{
		for(unsigned int i=0; i<location.size(); i++)
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(location[i],type[i],name[i]);
		}
		//Initialise resource groups after adding them.
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}
	catch(Ogre::Exception& e)
	{
		OutputDebugString(e.getFullDescription().c_str());
		retVal=false;
	}

	return retVal;
}

bool OgreManager::Render()
{
	bool retVal=true;
	try
	{
		_Root->renderOneFrame();
	}
	catch(Ogre::Exception& e)
	{
		//Not going to proclaim it, just silently shutdown.
		//MessageBoxA(NULL,e.getFullDescription().c_str(),"Exception!",MB_OK | MB_ICONERROR);
		OutputDebugString(e.getFullDescription().c_str());
		retVal=false;
	}

	return retVal;
}

void OgreManager::Shutdown()
{
	_Root->shutdown();
	delete _Root;
}

/*
Options list:
	node
		- child : Name of child node
	camera
		- ???
		- ???

	entity
		- shadows :  true/false for casting shadows
		- ???

	light
		- lighttype : determines the type of light it is
		- lightradius : radius of light(omni?)
		- lightcolor : color of the light(hex code)
*/

Ogre::SceneNode* OgreManager::createSceneNode(Ogre::SceneManager* scene, 
								 std::map<std::string,std::string> &options, 
								 Ogre::Vector3 &position, 
								 Ogre::Quaternion &rotation)
{
	Ogre::String realName = options["name"];
	Ogre::SceneNode* node = NULL;
	//if this particular field is empty, this function will return NULL.
	if(options["type"] == "camera")
	{
		realName = "cam" + realName; //"camMainPlayer" instead of "MainPlayer"
		Ogre::Camera* cam = scene->createCamera(realName);
		node = scene->getRootSceneNode()->createChildSceneNode(realName,position,rotation);
		node->attachObject(cam);
	}
	if(options["type"] == "entity")
	{
		realName = "ent" + realName; //"entMainPlayer" instead of "MainPlayer"
		Ogre::Entity* ent = scene->createEntity(realName,options["filename"],options["resgroup"]);

		//just to save some space and add a guaranteed default option(if blank, set to false).
		ent->setCastShadows( options["shadows"] == "true" ? true : false );

		node = scene->getRootSceneNode()->createChildSceneNode(realName,position,rotation);
		node->attachObject(ent);
	}
	if(options["type"] == "light")
	{
		//Set up the light
		realName = "light" + realName; //"lightMainLobby" instead of "MainLobby"
		Ogre::Light* light = scene->createLight(realName);
		if(options["lighttype"]=="spotlight")
		{
			light->setType(Ogre::Light::LT_SPOTLIGHT);
			//check for spotlight specific options
			//may add more, but not right now.
			//ToDo: add spotlight specific options!!
			light->setSpotlightRange(Ogre::Degree(10),Ogre::Degree(100),1.0f);
		}
		else if((options["lighttype"]=="point") || (options.find("lighttype") == options.end()))
		{
			//Not sure if there's anything else I need to do here. Probably not.
			light->setType(Ogre::Light::LT_POINT);
		}
		else
		{
			//I need to set the direction, but that's done for me with the SceneNode.
			light->setType(Ogre::Light::LT_DIRECTIONAL);
		}

		//Range of light, or rather the fading of the light as the distance from light to object gets larger.
		//Attenuation can kiss my ass.
		//Checks to make sure options["lightradius"] isn't empty, and that the light isn't directional(cause then the range doesn't matter).
		if(options.find("lightradius") != options.end() && light->getType() != light->LT_DIRECTIONAL)
		{
			int radius = atoi(options["lightradius"].c_str());
			setLightRange(light,radius);
		}
		else
			setLightRange(light,100); //Default radius is 100.

		//light color
		if(options.find("lightcolor") != options.end())
		{
			int hex = atoi(options["lightcolor"].c_str());
			light->setDiffuseColour(getColorFromHex(hex));
		}

		//Create node and attach the light to it.
		node = scene->getRootSceneNode()->createChildSceneNode(realName,position,rotation);
		node->attachObject(light);
	}

	//Node options
	if(options.find("child") != options.end())
	{
		//if this isn't a scene node, I'm fucked.
		Ogre::SceneNode* cNode = (Ogre::SceneNode*)scene->getRootSceneNode()->removeChild(options["child"]);
		//add the node as a child to the current node.
		node->addChild(cNode);
	}
}

//This works, but isn't documented very well...
//Might come back and document it later, but not right now...
//In other words...!!!MAGIC DON'T TOUCH!!!
void OgreManager::getMeshInformation(const Ogre::MeshPtr* const meshptr,
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

void OgreManager::setLightRange(Ogre::Light* l, Ogre::Real range)
{
	l->setAttenuation(range,1.0f,4.5/range,75.0f/(range*range));
	return;
}

Ogre::ColourValue OgreManager::getColorFromHex(int hexColor, float alpha)
{
	float r,g,b,a;
	a = 1.0f; //no transparency.

	r = ((hexColor >> 16) & 0xFF) / 255.0f;
	g = ((hexColor >> 8) & 0xFF) / 255.0f;
	b = (hexColor & 0xFF) / 255.0f;

	return Ogre::ColourValue(r,g,b,a);
}