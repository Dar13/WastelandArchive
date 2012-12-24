#include "StdAfx.h"

#include "interfaces\weapon.hxx"

#include "PhysicsManager.h"
#include "GraphicsManager.h"

#include "EWS.h"

#include "GunData.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

#define CONV_EAIWEAP_TO_METER 0.0254f
#define EQUIP_NODE_OFFSET_Y .675f

#ifndef _PLAYER_INTERFACE_

class DamageInterface;

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
	void Setup(const std::string& file,
			   GraphicsManager* graphics,
			   Ogre::SceneNode* equipNode,
               DamageInterface* damageInterface = nullptr);
	//! Updates the player instance, including the weapons and scripts associated with it.
	bool Update(InputManager* input,
				PhysicsManager* physics,
				EWSManager* ews,
				const OgreTransform& transform);
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

	//! Returns the damage interface, so that the Enemies and Player can use the same one
	DamageInterface* getDamageInterface() { return _damageInterface; }

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

	DamageInterface* _damageInterface;
};

#endif

/*! \brief Callback class to handle being shot by enemies/NPCs

*/
class DamageInterface
{
public:
	void registerShotAtPlayer(sGunShot gunshot,float distanceSquared);
	void registerShotAtEnemy(sGunShot gunshot,std::string& enemyName);
	//! Clears the _damage vector.
	double getTotalDamagePlayer();
	double getEnemyDamage(std::string& name);
private:
	std::vector<double> _damagePlayer;
	std::map<std::string,double> _damageEnemy;
};

#endif