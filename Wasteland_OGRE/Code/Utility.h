#include "StdAfx.h"

#ifndef _UTILITY_H_
#define _UTILITY_H_

/*! \brief Euler class from Kojack
*/
// Euler class for Ogre
// Euler class for Ogre, v2.0
// Author: Kojack
// License: Do whatever you want with it.
namespace Ogre
{
	class Euler
	{
	public:
		// Constructor which takes yaw, pitch and roll values.
		Euler(Ogre::Radian y, Ogre::Radian p = Ogre::Radian(0.0f), Ogre::Radian r = Ogre::Radian(0.0f)): mYaw(y), mPitch(p), mRoll(r), mChanged(true) 
		{
		}
 
		// Constructor which takes yaw, pitch and roll values as reals (radians).
		Euler(Ogre::Real y = 0.0f, Ogre::Real p = 0.0f, Ogre::Real r = 0.0f): mYaw(Ogre::Radian(y)), mPitch(Ogre::Radian(p)), mRoll(Ogre::Radian(r)), mChanged(true) 
		{
		}
 
		// Get the Yaw angle.
		inline Ogre::Radian getYaw() 
		{
			return mYaw;
		}
 
		// Get the Pitch angle.
		inline Ogre::Radian getPitch() 
		{
			return mPitch;
		}
 
		// Get the Roll angle.
		inline Ogre::Radian getRoll() 
		{
			return mRoll;
		}
 
		// Set the yaw.
		inline Euler &setYaw(Ogre::Radian y) 
		{
			mYaw = y; 
			mChanged = true; 
			return *this;
		}
 
		// Set the pitch.
		inline Euler &setPitch(Ogre::Radian p) 
		{
			mPitch = p; 
			mChanged = true; 
			return *this;
		}
 
		// Set the roll.
		inline Euler &setRoll(Ogre::Radian r) 
		{
			mRoll = r; 
			mChanged = true; 
			return *this;
		}
 
		// Apply a relative yaw. (Adds angle to current yaw)
		inline Euler &yaw(Ogre::Radian y) 
		{
			mYaw += y; 
			mChanged = true; 
			return *this;
		}
 
		// Apply a relative pitch. (Adds angle to current pitch)
		inline Euler &pitch(Ogre::Radian p) 
		{
			mPitch += p; 
			mChanged = true; 
			return *this;
		}
 
		// Apply a relative roll. (Adds angle to current roll)
		inline Euler &roll(Ogre::Radian r) 
		{
			mRoll += r; 
			mChanged = true; 
			return *this;
		}
 
		// Get a vector pointing forwards.
		inline Ogre::Vector3 getForward() 
		{
			return toQuaternion() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		}
 
		// Get a vector pointing to the right.
		inline Ogre::Vector3 getRight() 
		{
			return toQuaternion() * Ogre::Vector3::UNIT_X;
		}
 
		// Get a vector pointing up.
		inline Ogre::Vector3 getUp() 
		{
			return toQuaternion() * Ogre::Vector3::UNIT_Y;
		}
 
		// Calculate the quaternion of the euler object.
		// The result is cached, it is only recalculated when the component euler angles are changed.
		inline Ogre::Quaternion toQuaternion() 
		{
			if(mChanged) 
			{
				mCachedQuaternion = Ogre::Quaternion(mYaw, Ogre::Vector3::UNIT_Y) * Ogre::Quaternion(mPitch, Ogre::Vector3::UNIT_X) * Ogre::Quaternion(mRoll, Ogre::Vector3::UNIT_Z); 
				mChanged = false;
			}
			return mCachedQuaternion;
		}
 
		// Casting operator. This allows any ogre function that wants a Quaternion to accept a Euler instead.
		inline operator Ogre::Quaternion() 
		{
			return toQuaternion();
		}
 
		// Set the yaw and pitch to face in the given direction.
		// The direction doesn't need to be normalised.
		// setYaw - if false, the yaw isn't changed.
		// setPitch - if false, the pitch isn't changed.
		// Roll is always unaffected.
		Euler &setDirection(const Ogre::Vector3 &v, bool setYaw = true, bool setPitch = true)
		{
			Ogre::Vector3 d(v.normalisedCopy());
			if(setPitch)
				mPitch = Ogre::Math::ASin(d.y);
			if(setYaw)
				mYaw = Ogre::Math::ATan2(-d.x, -d.z);//+Ogre::Math::PI/2.0;
			mChanged = setYaw||setPitch;
 
			return *this;
		}
 
		// Normalise the selected rotations to be within the +/-180 degree range.
		// The normalise uses a wrap around, so for example a yaw of 360 degrees becomes 0 degrees, 
		// and -190 degrees becomes 170.
		// normYaw, normPitch, normRoll - only angles set to true are normalised.
		inline void normalise(bool normYaw = true, bool normPitch = true, bool normRoll = true)
		{
			if(normYaw)
			{
				Ogre::Real yaw = mYaw.valueRadians();
				if(yaw < -Ogre::Math::PI)
				{
					yaw = fmod(yaw, Ogre::Math::PI * 2.0f);
					if(yaw < -Ogre::Math::PI)
					{
						yaw += Ogre::Math::PI * 2.0f;
					}
					mYaw = yaw;
					mChanged = true;
				}
				else if(yaw > Ogre::Math::PI)
				{
					yaw = fmod(yaw, Ogre::Math::PI * 2.0f);
					if(yaw > Ogre::Math::PI)
					{
						yaw -= Ogre::Math::PI * 2.0f;
					}
					mYaw = yaw;
					mChanged = true;
				}
			}
			if(normPitch)
			{
				Ogre::Real pitch = mPitch.valueRadians();
				if(pitch < -Ogre::Math::PI)
				{
					pitch = fmod(pitch, Ogre::Math::PI * 2.0f);
					if(pitch < -Ogre::Math::PI)
					{
						pitch += Ogre::Math::PI * 2.0f;
					}
					mPitch = pitch;
					mChanged = true;
				}
				else if(pitch > Ogre::Math::PI)
				{
					pitch = fmod(pitch, Ogre::Math::PI * 2.0f);
					if(pitch > Ogre::Math::PI)
					{
						pitch -= Ogre::Math::PI * 2.0f;
					}
					mPitch = pitch;
					mChanged = true;
				}
			}
			if(normRoll)
			{
				Ogre::Real roll= mRoll.valueRadians();
				if(roll < -Ogre::Math::PI)
				{
					roll = fmod(roll, Ogre::Math::PI * 2.0f);
					if(roll < -Ogre::Math::PI)
					{
						roll += Ogre::Math::PI * 2.0f;
					}
					mRoll = roll;
					mChanged = true;
				}
				else if(roll > Ogre::Math::PI)
				{
					roll = fmod(roll, Ogre::Math::PI * 2.0f);
					if(roll > Ogre::Math::PI)
					{
						roll -= Ogre::Math::PI * 2.0f;
					}
					mRoll = roll;
					mChanged = true;
				}
			}
		}
 
		// Return the relative euler angles required to rotate from the current forward direction to the specified dir vector.
		// The result euler can then be added to the current euler to immediately face dir.
		// setYaw, setPitch - only the angles set to true are calculated. If false, the angle is set to 0.
		// shortest - if false, the full value of each angle is used. If true, the angles are normalised and the shortest
		// rotation is found to face the correct direction. For example, when false a yaw of 1000 degrees and a dir of
		// (0,0,-1) will return a -1000 degree yaw. When true, the same yaw and dir would give 80 degrees (1080 degrees faces
		// the same way as (0,0,-1).
		// The rotation won't flip upside down then roll instead of a 180 degree yaw.
		inline Euler getRotationTo(const Ogre::Vector3 &dir, bool setYaw = true, bool setPitch = true, bool shortest = true)
		{
			Euler t1;
			Euler t2;
			t1.setDirection(dir, setYaw, setPitch);
			t2=t1-*this;
			if(shortest && setYaw)
			{
				t2.normalise();
			}
			return t2;
		}
 
		// Clamp the yaw angle to a range of +/-limit.
		inline Euler &limitYaw(const Ogre::Radian &limit)
		{
			if(mYaw > limit)
			{
				mYaw = limit;
				mChanged = true;
			}
			else if(mYaw < -limit)
			{
				mYaw = -limit;
				mChanged = true;
			}
			return *this;
		}
 
		// Clamp the pitch angle to a range of +/-limit.
		inline Euler &limitPitch(const Ogre::Radian &limit)
		{
			if(mPitch > limit)
			{
				mPitch = limit;
				mChanged = true;
			}
			else if(mPitch < -limit)
			{
				mPitch = -limit;
				mChanged = true;
			}
			return *this;
		}
 
		// Clamp the roll angle to a range of +/-limit.
		inline Euler &limitRoll(const Ogre::Radian &limit)
		{
			if(mRoll > limit)
			{
				mRoll = limit;
				mChanged = true;
			}
			else if(mRoll < -limit)
			{
				mRoll = -limit;
				mChanged = true;
			}
			return *this;
		}
 
		// stream operator, for printing the euler component angles to a stream
		inline friend std::ostream &operator<<(std::ostream &o, const Euler &e)
		{
			o << "<Y:" << e.mYaw << ", P:" << e.mPitch << ", R:" << e.mRoll << ">";
			return o;
		}
 
		// Add two euler objects.
		inline Euler operator+(const Euler &rhs) const
		{
			return Euler(mYaw + rhs.mYaw, mPitch + rhs.mPitch, mRoll + rhs.mRoll);
		}
 
		// Subtract two euler objects. This finds the difference as relative angles.
		inline Euler operator-(const Euler &rhs) const
		{
			return Euler(mYaw - rhs.mYaw, mPitch - rhs.mPitch, mRoll - rhs.mRoll);
		}
 
		// Interpolate the euler angles by rhs. 
		inline Euler operator*(Ogre::Real rhs) const
		{
			return Euler(mYaw * rhs, mPitch * rhs, mRoll * rhs);
		}
 
		// Interpolate the euler angle by lhs. 
		inline friend Euler operator*(Ogre::Real lhs, const Euler &rhs)
		{
			return Euler(lhs * rhs.mYaw, lhs * rhs.mPitch, lhs * rhs.mRoll);
		}
 
		// Multiply two eulers. This has the same effect as multiplying quaternions.
		// The result is a quaternion.
		inline Quaternion operator*(const Euler &rhs) const
		{
			Euler e1(*this), e2(rhs);
			return e1.toQuaternion()*e2.toQuaternion();
		}
 
		// Apply the euler rotation to the vector rhs.
		inline Ogre::Vector3 operator*(const Ogre::Vector3 &rhs)
		{
			return toQuaternion() * rhs;
		}
 
	protected:
		Ogre::Radian mYaw;							// Rotation around the Y axis.
		Ogre::Radian mPitch;							// Rotation around the X axis.
		Ogre::Radian mRoll;							// Rotation around the Z axis.
		Ogre::Quaternion mCachedQuaternion;    // Cached quaternion equivalent of this euler object.
		bool mChanged;									// Is the cached quaternion out of date?
	};
}

/*! \brief Return structure for player data. Made for use by the EnvWarn system
*/
struct sPlayerData
{
	sPlayerData() : health(0),ammoNotInMag(0),ammoInMag(0),magSize(0) {}
	sPlayerData(int Health,int ammoMag,int ammoNotMag,int magSz) 
		: health(Health),ammoNotInMag(ammoNotMag),ammoInMag(ammoMag),magSize(magSz) {}
	int health;
	int ammoNotInMag;
	int magSize;
	int ammoInMag;
	//other information...
	//needed to overload this for some reason.
	bool operator!=(const sPlayerData& data) const
	{
		if(health != data.health) { return true; }
		if(ammoNotInMag != data.ammoNotInMag) { return true; }
		if(ammoInMag != data.ammoInMag) { return true; }

		return false;
	}
};

/*! \brief Allows convenient returns of both Bullet rigid bodies and Ogre SceneNodes.
*/
struct OgreBulletPair
{
	Ogre::SceneNode* ogreNode;
	btRigidBody* btBody;
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

	void vector3_toFloatPtr(const Ogre::Vector3& vector,float* result);
	void floatPtr_toVector3(const float* vect,Ogre::Vector3& result);

	std::wstring stringToWString(const std::string& str);
	std::string wstringToString(const std::wstring& wstr);

	void fixMinMax(Ogre::Vector3& min,Ogre::Vector3& max);

	void rotateToTarget(Ogre::SceneNode* node,
						const Ogre::Vector3& target,
						bool ignoreY = true,
						Ogre::Vector3 originalOrientation = Ogre::Vector3::NEGATIVE_UNIT_Z);
};

#endif