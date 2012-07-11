#include "StdAfx.h"

#include "interfaces\weapon.hxx"

#include "EWS.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "GraphicsManager.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

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
	SELECT,
	IDLE,
	MOVE,
	RELOAD,
	STARTFIRE,
	ENDFIRE,
	AUTOFIRE,
	FIRE
};

enum GUN_SOUND
{
	NO_SOUND = 0,
	FIRE,
	RELOAD,
	PUTAWAY,
	DRYFIRE,
	ALTRELOAD,
	ALTFIRE
};

struct EquippableObject;

struct sSoundFrame
{
	GUN_SOUND gunSound;
	std::vector<int> frames;
};

class baseEquippable
{
public:
	void setEquipped(const bool isequipped) { _isEquipped = isequipped; }
	void setWeapon(const bool isweapon) { _isWeapon = isweapon; }

	bool getEquipped() const { return _isEquipped; }
	bool getIsWeapon() const { return _isWeapon; }
protected:
	bool _isEquipped;
	bool _isWeapon;
	
};

class cGunData : public baseEquippable,Ogre::FrameListener
{
public:
	cGunData(GUN_TYPE type,GUN_NAME name,int magazineSize,int numMags);
	cGunData(const baseEquippable& base,GUN_TYPE type,GUN_NAME name, int magazineSize,int numMags);

	void fire(); //fires one round
	void reload();

	void setMagazineSize(const int& magSize);

	int getGunType();
	int getGunName();

	int getNumofMags();
	int getMagAmmo();

	bool isReloadNeeded();

	void setAnimationFrames(Ogre::Entity* entity);

	void setSoundFrames(weapon_t* Weapon);

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

private:
	//Gun description
	GUN_TYPE _type;
	GUN_NAME _name;
	int _magazineSize; //shouldn't be changed except in constructor

	int _ammoNotInMag;
	int _currentMagazineAmmo;

	bool _fireAnimEnded;
	bool _reloadAnimEnded;

	bool _reloadNeeded;

	std::vector<sSoundFrame> _soundFrames;
	Ogre::AnimationStateSet* _animationSet;
	Ogre::AnimationState* _currentAnimation;
};

class Player
{
public:
	Player();
	~Player();

	void Setup(std::string file);
	bool Update(InputManager* input,PhysicsManager* physics,EWSManager* ews,const OgreTransform& transform);
	void Clean(bool reuse = false);

	void placeEWS(EWSManager* ews,PhysicsManager* physics,const OgreTransform& transform);

	int getHealth();

private:
	bool _firingWeapon;
	bool _reloadingWeapon;

	int _health;

	std::vector<EquippableObject>::iterator _currentEquippable;
	std::vector<EquippableObject> _equippables;
};

#endif