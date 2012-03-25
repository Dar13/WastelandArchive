#include "StdAfx.h"
#include "Utility.h"

Ogre::Vector3 Utility::convert_btVector3(const btVector3& v)
{
	return Ogre::Vector3(v.x(),v.y(),v.z());
}
btVector3 Utility::convert_OgreVector3(const Ogre::Vector3& v)
{
	return btVector3(v.x,v.y,v.z);
}
Ogre::Quaternion Utility::convert_btQuaternion(const btQuaternion& q)
{
	return Ogre::Quaternion(q.w(),q.x(),q.y(),q.z());
}
btQuaternion Utility::convert_OgreQuaternion(const Ogre::Quaternion& q)
{
	return btQuaternion(q.x,q.y,q.z,q.w);
}