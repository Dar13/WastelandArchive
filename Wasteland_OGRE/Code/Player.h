#include "StdAfx.h"

#include "interfaces\weapon.hxx"

#include "InputManager.h"
#include "PhysicsManager.h"
#include "GraphicsManager.h"
#include "SoundManager.h"

#include "EWS.h"

#include "Utility.h"
#include "AnimationBlender.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

int getAnimID(const std::string& name);
int getSoundID(const std::string& sound);
bool correspondAnimSoundID(int animID,int soundID);
int getCorrespondSoundID(int animID);
bool isAnimAlmostEnded(Ogre::AnimationState* anim);

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
	//! Sets the moving state of the gun(necessary for animation).
	void setMoving(bool moving) { _moving = moving; }
	//! Gets the moving state of the gun.
	bool isMoving() { return _moving; }
	//! Sets the firing status of the gun(sometimes necessary for animation purposes).
	void setFiring(bool firing) { _firing = firing; if(!firing){_firingOverride = false;} }
	//! Gets the firing status of the gun that this class represents.
	bool isFiring() { return _firing; }

	//! Sets the magazine size for the gun.
	void setMagazineSize(const int& magSize);
	//! Gets the magainze size of the gun.
	int getMagazineSize() { return _magazineSize; }
	//! Sets the damage per bullet fired.
	void setDamagePerBullet(const int& damage) { _damagePerBullet = damage; }
	//! Sets the accuracy radius of the gun
	void setAccuracyRadius(const int& accuracy) { _accuracyRadius = accuracy; }
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

private:
	//Gun data
	GUN_TYPE _type;
	GUN_NAME _name;
	int _magazineSize;
	int _damagePerBullet;
	int _accuracyRadius;
	int _effectiveRange;
	int _fireRate;
	int _fireRateCount;

	int _ammoNotInMag;
	int _currentMagazineAmmo;

	int _playingAnim;

	bool _firing,_reloading,_moving;
	bool _mouseHeld,_firingOverride;

	bool _reloadNeeded;

	bool _soundPlayed;

	SoundManager* _soundMgr; // required.
	std::map<int,sGunSound> _sounds;
	FMOD::Channel* _soundChannel;
	
	AnimationBlender _animBlender;
	
};

/*! \brief Handles the player instance in the current application state.

*/
class Player
{
public:
	//! Default constructor
	Player();
	//! Destructor
	~Player();
	
	//! Sets up the player instance.
	void Setup(const std::string& file,GraphicsManager* graphics,Ogre::SceneNode* equipNode);
	//! Updates the player instance, including the weapons and scripts associated with it.
	bool Update(InputManager* input,PhysicsManager* physics,EWSManager* ews,const OgreTransform& transform);
	//! Cleans up the player instance's memory and such.
	void Clean(bool reuse = false);

	//! Places the EWS system. Really more of a utility function than a publicly accessible one.
	void placeEWS(EWSManager* ews,PhysicsManager* physics,const OgreTransform& transform);

	//! Adds an equippable object to the player instance. Can be a weapon or something else.
	void addEquippableObject(const EquippableObject& object);

	//! Equips an equippable object. (This will be overloaded eventually)
	void equipObject(const EquippableObject& obj);

	EquippableObject& getEquippedObject() { return _equippables[_curEquippable]; }
	
	//! Returns basic player data including health and ammo data.
	sPlayerData getPlayerData();

	//! Sets the scene node that all equippable nodes will be attached to.
	void setEquipNode(Ogre::SceneNode* node) { _equipNode = node; }

private:
	
	bool _firingWeapon;
	bool _reloadingWeapon;

	int _health;

	SoundManager* _soundMgr;

	Ogre::SceneNode* _equipNode;

	int _curEquippable;
	std::vector<EquippableObject> _equippables;
};

#endif