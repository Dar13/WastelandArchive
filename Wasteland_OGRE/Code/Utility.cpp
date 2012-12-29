#include "StdAfx.h"
#include "Utility.h"

void LuaDataVisitor::operator()(const double& d)
{
#ifdef _DEBUG
	assert(_setBool || _setStr || _setNum);
#endif
	number = d;
	_setNum = true;
}

void LuaDataVisitor::operator()(const std::string& s)
{
#ifdef _DEBUG
	assert(_setBool || _setStr || _setNum);
#endif
	string = s;
	_setStr = true;
}

void LuaDataVisitor::operator()(const bool& b)
{
#ifdef _DEBUG
	assert(_setBool || _setStr || _setNum);
#endif
	boolean = b;
	_setBool = true;
}

bool LuaDataVisitor::check(DataTypes type)
{
	if( (_setBool == false) && (_setStr == false) && (_setNum == false) )
	{
		return false;
	}

	switch(type)
	{
	case BOOLEAN:
		return _setBool;
		break;
	case STRING:
		return _setStr;
		break;
	case DOUBLE:
		return _setNum;
		break;
	default:
		return false;
		break;
	}
}

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
											  Ogre::Vector3 originalDirection)
{
	Ogre::Vector3 source = node->getOrientation() * originalDirection;
	if(ignoreY)
	{
		source.y = 0;
	}

	node->rotate(source.getRotationTo(target));

	return;
}

Ogre::Vector3 Utility::vector3_lerp(Ogre::Vector3& source,Ogre::Vector3& destination, float timeInMs,float lengthInSecs)
{
	return (source + (destination - source) * (timeInMs / (lengthInSecs * 1000.0f)));
}

FMOD_VECTOR Utility::ogreToFMOD(const Ogre::Vector3& v)
{
	FMOD_VECTOR outV;
	outV.x = v.x;
	outV.y = v.y;
	outV.z = v.z;
	return outV;
}

bool Utility::getIntFromLuaData(LuaData& ld,int& i)
{
	LuaDataVisitor v;
	ld.data.apply_visitor(v);

	if(v.check(LuaDataVisitor::DOUBLE))
	{
		i = static_cast<int>(v.number);
		return true;
	}
	else
	{
		return false;
	}
}

bool Utility::getDoubleFromLuaData(LuaData& ld,double& d)
{
	LuaDataVisitor v;
	ld.data.apply_visitor(v);

	if(v.check(LuaDataVisitor::DOUBLE))
	{
		d = v.number;
		return true;
	}
	else
	{
		return false;
	}
}

bool Utility::getStringFromLuaData(LuaData& ld, std::string& str)
{
	LuaDataVisitor v;
	ld.data.apply_visitor(v);

	if(v.check(LuaDataVisitor::STRING))
	{
		str = v.string;
		return true;
	}
	else
	{
		return false;
	}
}

bool Utility::getBooleanFromLuaData(LuaData& ld, bool& b)
{
	LuaDataVisitor v;
	ld.data.apply_visitor(v);

	if(v.check(LuaDataVisitor::BOOLEAN))
	{
		b = v.boolean;
		return true;
	}
	else
	{
		return false;
	}
}