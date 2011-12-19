#include <Ogre.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>

#include <OIS.h>

#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>

#include <vector>
#include <map>
#include <algorithm>
#include <string>

#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#endif 