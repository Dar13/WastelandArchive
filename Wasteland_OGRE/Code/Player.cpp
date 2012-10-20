#include "StdAfx.h"
#include "interfaces\list.hxx"
#include "interfaces\weapon.hxx"
#include "Player.h"

#include "InputManager.h"
#include "Utility.h"

#include "GameManager.h"

#include "debug\print.h"

Player::Player()
{
	_firingWeapon = false;
	_reloadingWeapon = false;

	_health = 100;
}

Player::~Player()
{
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

void Player::Setup(const std::string& file,GraphicsManager* graphics,Ogre::SceneNode* equipNode)
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
}

bool Player::Update(InputManager* input,PhysicsManager* physics,EWSManager* ews,const OgreTransform& transform)
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
	obj.node->setPosition(0.0f,0.0f,-.5f);
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