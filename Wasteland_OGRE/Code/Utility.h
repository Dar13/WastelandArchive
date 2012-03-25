#include "StdAfx.h"

#ifndef _UTILITY_H_
#define _UTILITY_H_

namespace Utility
{
	Ogre::Vector3 convert_btVector3(const btVector3& v);
	btVector3 convert_OgreVector3(const Ogre::Vector3& v);
	Ogre::Quaternion convert_btQuaternion(const btQuaternion& q);
	btQuaternion convert_OgreQuaternion(const Ogre::Quaternion& q);
};

#endif