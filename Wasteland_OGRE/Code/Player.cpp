#include "StdAfx.h"
#include "interfaces\list.hxx"
#include "interfaces\weapon.hxx"
#include "Player.h"

#include "InputManager.h"
#include "Utility.h"

#include "GameManager.h"

#include "debug\print.h"

#include "LuaManager.h"

Player::Player()
	: _firingWeapon(false),
	  _reloadingWeapon(false),
	  _health(100),
	  _damageInterface(nullptr),
	  _equipNode(nullptr),
	  _soundMgr(nullptr)
{
}

Player::~Player()
{
	//Delete this, since the appstate gives you control of it.
	delete _damageInterface;

	//clean up equippables
	for(auto itr = _equippables.begin(); itr != _equippables.end(); ++itr)
	{
		if(itr->equip->getIsWeapon())
		{
			delete static_cast<cGunData*>(itr->equip);
		}
		else
		{
			delete itr->equip;
		}
	}
}

void Player::Setup(const std::string& file,GraphicsManager* graphics,Ogre::SceneNode* equipNode,DamageInterface* damageInterface)
{
	if(equipNode != nullptr)
	{
		_equipNode = equipNode;
	}

	if(_equippables.size() == 1 && _equipNode != nullptr)
	{
		//equip the one weapon/thing
		equipObject(_equippables[_curEquippable]);
	}

	for(auto itr = _equippables.begin(); itr != _equippables.end(); ++itr)
	{
		if(itr->equip->getIsWeapon())
		{
			graphics->getRoot()->addFrameListener(static_cast<cGunData*>(itr->equip));
		}
	}

	_damageInterface = damageInterface;
}

bool Player::Update(InputManager* input,
					PhysicsManager* physics,
					EWSManager* ews,
					const OgreTransform& transform)
{
	if(input->isCFGKeyPressed(InputManager::ENVWARNSYS))
	{
		placeEWS(ews,physics,transform);
	}

	cGunData* gun = nullptr;

	if(_equippables[_curEquippable].equip->getIsWeapon())
	{
		gun = static_cast<cGunData*>(_equippables[_curEquippable].equip);
	}

	if(gun != nullptr)
	{	
		bool movkey = false;
		movkey = (input->isCFGKeyPressed(InputManager::FORWARD) || 
			      input->isCFGKeyPressed(InputManager::BACKWARD)||
				  input->isCFGKeyPressed(InputManager::RIGHT) ||
				  input->isCFGKeyPressed(InputManager::LEFT));

		if(input->isMBPressed(OIS::MB_Left))
		{
			//shoot gun
			std::cout << "MB_Left pressed..." << std::endl;
			gun->fire();
			//check for collisions with enemies if first time through
			if(!gun->isMouseHeld())
			{
				Ogre::SceneManager* scene = _equipNode->getCreator();
				Ogre::Vector3 dir = transform.direction;
				Ogre::Ray ray(transform.position,dir);

				Ogre::RaySceneQuery* rayQuery = scene->createRayQuery(ray);
				rayQuery->setSortByDistance(true);

				Ogre::RaySceneQueryResult results = rayQuery->execute();

				auto shot =  [this,gun] (Ogre::RaySceneQueryResultEntry& entry) 
				{
					if(entry.movable != nullptr)
					{
						std::string name = entry.movable->getName().substr(3,std::string::npos);
						LevelData::BaseEntity* ent = LuaManager::getSingleton().getEntity(name);
						if(ent != nullptr && (ent->getType() == LevelData::NPC || ent->getType() == LevelData::ENEMY))
						{
							_damageInterface->registerShotAtEnemy(gun->getGunshotData(),name);
						}
					}
				};
				std::for_each(results.begin(),results.end(),shot);

				scene->destroyQuery(rayQuery);
			}
		}
		else
		{
			gun->setFiring(false);
		}

		if(input->isCFGKeyPressed(InputManager::RELOAD))
		{
			gun->reload();
		}

		if(movkey)
		{
			gun->setMoving(true);
		}
		else
		{
			if(gun->isMoving())
			{
				gun->setMoving(false);
			}
		}
	}

	if(_damageInterface != nullptr)
	{
		//this clears out the added-up damage in the interface.
		_health -= static_cast<int>(_damageInterface->getTotalDamagePlayer());
	}

	return true;
}

void Player::placeEWS(EWSManager* ews,PhysicsManager* physics,const OgreTransform& transform)
{
	btVector3 rayPosition,rayNormal;
	btVector3 rayStart = Utility::convert_OgreVector3(transform.position);
	btVector3 rayEnd;
	Ogre::Vector3 dir = transform.position + (transform.direction * 1000.0f);
	rayEnd = Utility::convert_OgreVector3(dir);
	if(physics->RaycastWorld_Closest(rayStart,rayEnd,rayPosition,rayNormal))
	{
		ews->Place(Utility::convert_btVector3(rayPosition),Utility::convert_btVector3(rayNormal),transform);
	}
}

void Player::equipObject(const EquippableObject& obj)
{
	if(obj.node->getParentSceneNode() != _equipNode)
	{
		obj.node->getParentSceneNode()->removeChild(obj.node);
		_equipNode->addChild(obj.node);
	}
	obj.node->setPosition(0.11f,-0.08f + EQUIP_NODE_OFFSET_Y,-.25f);
	std::cout << obj.node->getPosition() << std::endl;
	std::cout << "Weapon equipped." << std::endl;
}

void Player::addEquippableObject(const EquippableObject& object)
{
	_equippables.push_back(object);
	object.node->scale(Ogre::Vector3(CONV_EAIWEAP_TO_METER));
	_curEquippable = 0; //there's only one...
	std::cout << "Weapon added to equippables." << std::endl;
}

sPlayerData Player::getPlayerData()
{
	if(_equippables[_curEquippable].equip->getIsWeapon())
	{
		return sPlayerData(_health,
						   static_cast<cGunData*>(_equippables[_curEquippable].equip)->getMagAmmo(),
						   static_cast<cGunData*>(_equippables[_curEquippable].equip)->getNumOfMags(),
						   static_cast<cGunData*>(_equippables[_curEquippable].equip)->getMagazineSize());
	}
	else
	{
		//-1 denotes ammo is not applicable
		return sPlayerData(_health,-1,-1,-1);
	}
}

void Player::Clean(bool reuse)
{

	return;
}

void DamageInterface::registerShotAtPlayer(sGunShot gunshot,float distanceSquared)
{
	float newRadius = static_cast<float>(distanceSquared * (gunshot.accuracyRadius / 50.0f));
	if(distanceSquared < 64)
	{
		_damagePlayer.push_back(gunshot.damage);
	}
	else
	{
		if(Ogre::Math::RangeRandom(0.0,(newRadius / 2.0f) + 2.0f) + (newRadius / 2.0f) < newRadius)
		{
			_damagePlayer.push_back(gunshot.damage);
		}
	}
}

void DamageInterface::registerShotAtEnemy(sGunShot gunshot,std::string& enemyName)
{
	_damageEnemy[enemyName] += gunshot.damage;
	std::cout << "Enemy:" << enemyName << " has been hit!" << std::endl;
}

double DamageInterface::getTotalDamagePlayer()
{
	double total = 0.0;
	auto sumUp = [&total] (const double& dmg)
	{
		total += dmg;
	};
	std::for_each(_damagePlayer.begin(),_damagePlayer.end(),sumUp);

	_damagePlayer.clear();

	return total;
}

double DamageInterface::getEnemyDamage(std::string& name)
{
	if(_damageEnemy.find(name) == _damageEnemy.end())
	{
		return 0;
	}
	double dmg = _damageEnemy[name];
	_damageEnemy[name] = 0;
	return dmg;
}