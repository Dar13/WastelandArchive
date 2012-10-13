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

Ogre::Matrix3 Utility::convert_btMatrix3(const btMatrix3x3 &m)
{
	Ogre::Matrix3 mat;
	mat.SetColumn(0,Utility::convert_btVector3(m[0]));
	mat.SetColumn(1,Utility::convert_btVector3(m[1]));
	mat.SetColumn(2,Utility::convert_btVector3(m[2]));
	return mat;
}

btMatrix3x3 Utility::convert_OgreMatrix3(const Ogre::Matrix3 &m)
{
	btMatrix3x3 mat;
	Ogre::Quaternion quat;
	quat.FromRotationMatrix(m);
	mat.setRotation(Utility::convert_OgreQuaternion(quat));
	return mat;
}

std::string Utility::vector3_toStr(const Ogre::Vector3& v)
{
	return Ogre::StringConverter::toString(v) + '\n';
}

std::string Utility::vector3_toStr(const btVector3& v)
{
	std::stringstream c;
	c << v.x() << "," << v.y() << "," << v.z() << std::endl;
	return c.str();
}

void Utility::vector3_toFloatPtr(const Ogre::Vector3& vector,float* result)
{
	result[0] = vector[0];
	result[1] = vector[1];
	result[2] = vector[2];
}

void Utility::floatPtr_toVector3(const float* vector,Ogre::Vector3& result)
{
	result.x = vector[0];
	result.y = vector[1];
	result.z = vector[2];
}

std::wstring Utility::stringToWString(const std::string& str)
{
	std::wstring ret(str.begin(),str.end());
	ret.assign(str.begin(),str.end());
	return ret;
}

std::string Utility::wstringToString(const std::wstring& str)
{
	std::string ret(str.begin(),str.end());
	ret.assign(str.begin(),str.end());
	return ret;
}

void Utility::fixMinMax(Ogre::Vector3& min,Ogre::Vector3& max)
{
	if(min.x > max.x) { std::swap(min.x,max.x); }
	if(min.y > max.y) { std::swap(min.y,max.y); }
	if(min.z > max.z) { std::swap(min.z,max.z); }
	return;
}

void Utility::rotateToTarget(Ogre::SceneNode* node,
											  const Ogre::Vector3& target,
											  bool ignoreY,
											  Ogre::Vector3& originalDirection)
{
	Ogre::Vector3 source = node->getOrientation() * originalDirection;
	if(ignoreY)
	{
		source.y = 0;
	}

	node->rotate(source.getRotationTo(target));

	return;
}