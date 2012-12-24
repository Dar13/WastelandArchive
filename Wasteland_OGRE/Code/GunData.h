#include "StdAfx.h"

#include "AnimationBlender.h"
#include "SoundManager.h"
#include "InputManager.h"

#include "Utility.h"

#ifndef _GUN_DATA_H_
#define _GUN_DATA_H_

struct sGunShot
{
	int gunName;
	int damage;
	double accuracyRadius;
};

struct sSoundFrame
{
	int gunSound;
	int frame;
	float relativePosition; //relative position to animation.
};

struct sGunSound
{
	sSoundFrame soundInfo;
	sSound sound;
};

class baseEquippable
{
public:
	~baseEquippable() {};
	void setEquipped(const bool isequipped) { _isEquipped = isequipped; }
	void setWeapon(const bool isweapon) { _isWeapon = isweapon; }

	bool getEquipped() const { return _isEquipped; }
	bool getIsWeapon() const { return _isWeapon; }
protected:
	bool _isEquipped;
	bool _isWeapon;
};

/*! \brief Return structure for weapons and equipable items
*/
struct EquippableObject
{
	Ogre::SceneNode* node;
	baseEquippable* equip;
};

class cGunData;

struct WeaponObject
{
	Ogre::SceneNode* node;
	cGunData* weapon;
};

/*! \brief Class that handles all gun-related data/manipulation tasks.
*/
class cGunData : public baseEquippable, public Ogre::FrameListener
{
public:
	enum GUN_TYPE
	{
		NO_TYPE = 0,
		PISTOL,
		ASSAULT,
		SMG,
		SHOTGUN,
		HEAVY
	};

	enum GUN_NAME
	{
		NO_NAME = 0,
		M9,
		G36C
	};

	enum GUN_ANIM
	{
		NO_ANIM = 0,
		ANIM_SELECT,
		ANIM_IDLE,
		ANIM_MOVE,
		ANIM_RELOAD,
		ANIM_STARTFIRE,
		ANIM_ENDFIRE,
		ANIM_AUTOFIRE,
		ANIM_FIRE,
		ANIM_PUTAWAY
	};

	enum GUN_SOUND
	{
		NO_SOUND = 0,
		SND_FIRE,
		SND_RELOAD,
		SND_PUTAWAY,
		SND_DRYFIRE,
		SND_ALTRELOAD,
		SND_ALTFIRE
	};

	//! Constructor with some required data.
	/*
		\param type Type of gun as defined by cGunData::GUN_TYPE
		\param name Name of gun as defined by cGunData::GUN_NAME
		\param magazineSize Size of gun magazine
		\param numMags Number of starting magazines
	*/
	cGunData(GUN_TYPE type,GUN_NAME name,int magazineSize,int numMags);
	//! Overloaded constructor
	/*!
		\param base Allows custom initialization of base class
		\param type Type of gun as defined by cGunData::GUN_TYPE
		\param name Name of gun as defined by cGunData::GUN_NAME
		\param magazineSize Size of gun magazine
		\param numMags Number of starting magazines
	*/
	cGunData(const baseEquippable& base,GUN_TYPE type,GUN_NAME name, int magazineSize,int numMags);
	~cGunData();

	//! Allows outside classes to fire the gun that this class represents
	void fire();
	//! Allows outside classes to reload the gun that this class represents
	void reload();
	//! Allows outside classes to ADS the gun that this class represents
	void aimDownSights();
	//! Sets the moving state of the gun(necessary for animation).
	void setMoving(bool moving) { _moving = moving; }
	//! Gets the moving state of the gun.
	bool isMoving() { return _moving; }
	//! Sets the firing status of the gun(sometimes necessary for animation purposes).
	void setFiring(bool firing) { _firing = firing; if(!firing){_firingOverride = false;} }
	//! Gets the firing status of the gun that this class represents.
	bool isFiring() { return _firing; }
	//! Gets the reloading status of the gun that this class represents
	bool isReloading() { return _reloading; }

	//! Returns whether or not the mouse is held in relation to this gun
	bool isMouseHeld() { return _mouseHeld; }

	bool shouldDamage()
	{
		if(_type == PISTOL || _type == SHOTGUN)
		{
			return !_mouseHeld;
		}

		if(_fireRate > 1)
		{
			//based on firerate and progression of animation.
		}

		//when in doubt, say no.
		return false;
	}

	//! Sets the magazine size for the gun.
	void setMagazineSize(const int& magSize);
	//! Gets the magainze size of the gun.
	int getMagazineSize() { return _magazineSize; }
	//! Sets the damage per bullet fired.
	void setDamagePerBullet(const int& damage) { _damagePerBullet = damage; }
	//! Gets the damage per bullet fired.
	int getDamagePerBullet() { return _damagePerBullet; }
	//Accuracy radius = radius of base of firing-cone at 50 meters distance.
	//! Sets the accuracy radius of the gun
	void setAccuracyRadius(const int& accuracy) { _accuracyRadius = accuracy; }
	//! Gets the accuracy of the gun
	int getAccuracyRadius() { return _accuracyRadius; }
	//! Sets the effective range of the gun
	void setEffectiveRange(const int& range) { _effectiveRange = range; }
	//! Sets the fire rate of the gun(1 = 1 bullet per mouse-click, >1 = number of bullets fired per second(?) )
	void setFireRate(const int& firerate) { _fireRate = firerate; }

	//! Gets the gun type.
	int getGunType() { return _type; }
	//! Gets the gun name.
	int getGunName() { return _name; }

	//! Gets the number of magazines left for the gun.
	int getNumOfMags() { return _ammoNotInMag / _magazineSize; }
	//! Gets the amount of ammo left in the current magazine.
	int getMagAmmo() { return _currentMagazineAmmo; }

	//! Gets the current reload status.
	bool isReloadNeeded() { return _reloadNeeded; }

	//! Utility function, called to help set-up the gun.
	void setAnimationFrames(Ogre::Entity* entity);
	//! Utility function, called to help set-up the gun.
	void setSoundFrames(weapon_t* Weapon,SoundManager* Sound);

	//! Interface for frameListener. Contains most of animation logic.
	bool frameStarted(const Ogre::FrameEvent& evt);
	//! Interface for frameListener
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	//! Interface for frameListener
	bool frameEnded(const Ogre::FrameEvent& evt);

	sGunShot getGunshotData() { sGunShot shot; 
								shot.gunName = _name; 
								shot.damage = _damagePerBullet; 
								shot.accuracyRadius = _accuracyRadius; 
								return shot; }

	//Here's some useful static functions
	static int getAnimID(const std::string& name);
	static int getSoundID(const std::string& sound);
	static bool isCorrespondAnimSoundID(int animID,int soundID);
	static int getCorrespondingSoundID(int animID);
	static bool isAnimationAlmostDone(Ogre::AnimationState* anim);

	std::string _getGunNameString() { return _gunEntityName; }

private:
	//Gun data
	GUN_TYPE _type;
	GUN_NAME _name;
	int _magazineSize;
	int _damagePerBullet;
	//! Radius of firing-cone at 50 meters
	int _accuracyRadius;
	int _effectiveRange;
	int _fireRate;
	int _fireRateCount;

	int _ammoNotInMag;
	int _currentMagazineAmmo;

	int _playingAnim;

	bool _firing,_reloading,_moving,_ADS;
	bool _mouseHeld,_firingOverride;

	bool _reloadNeeded;

	bool _soundPlayed;

	SoundManager* _soundMgr; // required.
	std::map<int,sGunSound> _sounds;
	FMOD::Channel* _soundChannel;

	std::string _gunEntityName;
	
	AnimationBlender _animBlender;
	
};

#endif