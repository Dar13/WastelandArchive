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
	_Log->setLogDetail(Ogre::LoggingLevel::LL_BOREME);
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

//Utility functions that should be included with the OgreManager

void getMeshInformation(const Ogre::MeshPtr* const meshptr,
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