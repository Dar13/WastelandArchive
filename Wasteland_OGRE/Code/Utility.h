#include "StdAfx.h"

#ifndef _UTILITY_H_
#define _UTILITY_H_

/*! \brief Return structure for player data. Made for use by the EW system
*/
struct sPlayerData
{
	sPlayerData() : health(0),ammoNotInMag(0),ammoInMag(0) {}
	sPlayerData(int Health,int ammoMag,int ammoNotMag) 
		: health(Health),ammoNotInMag(ammoNotMag),ammoInMag(ammoMag) {}
	int health;
	int ammoNotInMag;
	int ammoInMag;
	//other information...

	bool operator!=(const sPlayerData& data) const
	{
		if(health != data.health) { return true; }
		if(ammoNotInMag != data.ammoNotInMag) { return true; }
		if(ammoInMag != data.ammoInMag) { return true; }

		return false;
	}
};

namespace Utility
{

	Ogre::Vector3 convert_btVector3(const btVector3& v);
	btVector3 convert_OgreVector3(const Ogre::Vector3& v);
	Ogre::Quaternion convert_btQuaternion(const btQuaternion& q);
	btQuaternion convert_OgreQuaternion(const Ogre::Quaternion& q);
	Ogre::Matrix3 convert_btMatrix3(const btMatrix3x3 &m);
	btMatrix3x3 convert_OgreMatrix3(const Ogre::Matrix3 &m);

	std::string vector3_toStr(const Ogre::Vector3& v);
	std::string vector3_toStr(const btVector3& v);
};

#endif