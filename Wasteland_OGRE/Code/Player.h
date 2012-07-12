#include "StdAfx.h"

#include "interfaces\weapon.hxx"

#include "EWS.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "GraphicsManager.h"

#include "AnimationBlender.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

int getAnimID(const std::string& name);

struct EquippableObject;

struct sSoundFrame
{
	int gunSound;
	std::vector<int> frames;
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
	bool _fireAnimStarted;
	bool _reloadAnimEnded;
	bool _reloadAnimStarted;

	bool _firing,_reloading,_moving;

	bool _reloadNeeded;

	std::vector<sSoundFrame> _soundFrames;
	AnimationBlender _animBlender;
	
};

class Player
{
public:
	Player();
	~Player();
	
	void Setup(const std::string& file);
	bool Update(InputManager* input,PhysicsManager* physics,EWSManager* ews,const OgreTransform& transform);
	void Clean(bool reuse = false);

	void placeEWS(EWSManager* ews,PhysicsManager* physics,const OgreTransform& transform);

	void addEquippableObject(const EquippableObject& object);

	void equipObject(const EquippableObject& obj);

	int getHealth();

	void setEquipNode(Ogre::SceneNode* node) { _equipNode = node; }

private:
	bool _firingWeapon;
	bool _reloadingWeapon;

	int _health;

	Ogre::SceneNode* _equipNode;

	std::vector<EquippableObject>::iterator _currentEquippable;
	std::vector<EquippableObject> _equippables;
};

#endif