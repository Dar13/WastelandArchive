#include "StdAfx.h"

#include "GunData.h"

cGunData::cGunData(GUN_TYPE type,GUN_NAME name,int magazineSize,int numMags)
{
	_type = type;
	_name = name;
	_magazineSize = magazineSize;

	_ammoNotInMag = (numMags * _magazineSize) - _magazineSize;
	_currentMagazineAmmo = _magazineSize;

	_reloadNeeded = false;

	_isWeapon = true;
	_isEquipped = false;

	_playingAnim = 0;

	_moving = false;
	_firing = false;
	_reloading = false;
}

cGunData::cGunData(const baseEquippable& base,GUN_TYPE type,GUN_NAME name,int magazineSize,int numMags)
{
	_isWeapon = base.getIsWeapon();
	_isEquipped = base.getEquipped();
	
	_type = type;
	_name = name;
	_magazineSize = magazineSize;

	_ammoNotInMag = (numMags * _magazineSize) - _magazineSize;
	_currentMagazineAmmo = _magazineSize;

	_reloadNeeded = false;

	_playingAnim = 0;

	_moving = false;
	_firing = false;
	_reloading = false;
	_mouseHeld = false;
	_firingOverride = false;
}

void cGunData::fire()
{
	if(_reloadNeeded)
	{
		reload();
	}
	else
	{
		if(!_firing)
		{
			_firing = true;
			_mouseHeld = false;
			_currentMagazineAmmo--;
		}
		else
		{
			_mouseHeld = true;
			if(_fireRate > 1 && _fireRateCount >= _fireRate)
			{
				_fireRateCount = 0;
				_currentMagazineAmmo--;
			}
		}

		if(_currentMagazineAmmo == 0)
		{
			_reloadNeeded = true;
		}
	}
}

void cGunData::reload()
{
	if(_reloadNeeded)
	{
		//empty magazine, full reload
		if(_ammoNotInMag < _magazineSize && _ammoNotInMag > 0)
		{
			int tmp = _magazineSize - _ammoNotInMag;
			_ammoNotInMag -= tmp;
			_currentMagazineAmmo = tmp;
		}
		else if(_ammoNotInMag <= 0)
		{
			//can't reload, nothing to reload it with
			_reloading = false;
			return;
		}
		else
		{
			_ammoNotInMag -= _magazineSize;
			_currentMagazineAmmo = _magazineSize;
		}
		
		_reloading = true;
	}
	else
	{
		//not a full reload
		_ammoNotInMag -= (_magazineSize - _currentMagazineAmmo);
		_currentMagazineAmmo += (_magazineSize - _currentMagazineAmmo);
		_reloading = true;
	}

	_reloadNeeded = false;
}

bool cGunData::frameStarted(const Ogre::FrameEvent& evt)
{
	_animBlender.addTime(evt.timeSinceLastFrame);

	int sourceAnim = getAnimID(_animBlender.getSource()->getAnimationName());
	int targetAnim;
	
	//sometimes the target is 0.
	if(_animBlender.getTarget() != nullptr)
	{
		targetAnim = getAnimID(_animBlender.getTarget()->getAnimationName());
	}
	else
	{
		targetAnim = cGunData::NO_ANIM;
	}

	if(_firing && !_reloading)
	{
		if(_fireRate > 1)
		{
			//auto weapons, burst weapons
		}
		else
		{
			//single-fire weapons
			//left mouse button is clicked, but not held yet
			if(!_mouseHeld)
			{
				
				if(sourceAnim != cGunData::ANIM_FIRE)
				{
					if(targetAnim != cGunData::ANIM_FIRE)
					{
						_animBlender.blend("fire",AnimationBlender::BlendWhileAnimating,.2f,false);
						_soundChannel->stop();
						_soundChannel = _soundMgr->playSound(_sounds[SND_FIRE].sound);
						std::cout << "fire" << std::endl;
					}
				}
				else
				{
					//not even sure this is necessary
					_firingOverride = true;
				}
			}
		}
	}

	//Doesn't matter if the player is moving or not. Only if he's firing the weapon.
	if(_reloading && !_firing)
	{
		//if the source is reload then job is done.
		if(sourceAnim != cGunData::ANIM_RELOAD)
		{
			//checks for non-reloading animations.
			if(targetAnim != cGunData::ANIM_RELOAD)
			{
				_animBlender.blend("reload",AnimationBlender::BlendSwitch,.2f,false);
			}
		}
		else
		{
			if(_animBlender.complete)
			{
				_reloading = false;
			}
		}
	}

	bool hasEntered = false;
	//Need a small delay in some cases when switching from firing animation to moving.
	if(_moving && sourceAnim == cGunData::ANIM_FIRE && !isAnimationAlmostDone(_animBlender.getSource()))
	{
		_moving = false;
		std::cout << "Should be set to false" << std::endl;
		hasEntered = true;
	}

	//for moving animation.
	if(_moving && !_firing && !_reloading)
	{
		if(hasEntered) { std::cout << "WTF" << std::endl; }
		if(sourceAnim != cGunData::ANIM_MOVE)
		{
			if(targetAnim != cGunData::NO_ANIM)
			{
				_animBlender.blend("move",AnimationBlender::BlendSwitch,.2f,true);
			}
			
			if(targetAnim != cGunData::ANIM_MOVE)
			{
				_animBlender.blend("move",AnimationBlender::BlendSwitch,.2f,true);
			}
		}
	}

	//nothing else is going on, go to idle
	if(!_reloading && (!_firing || _firingOverride) && !_moving)
	{
		if(hasEntered) { std::cout << "SHIT" << std::endl; }
		if(sourceAnim != cGunData::ANIM_IDLE)
		{
			if(targetAnim != cGunData::NO_ANIM)
			{
				_animBlender.blend("idle",AnimationBlender::BlendSwitch,.2f,true);
			}
			else if(targetAnim != cGunData::ANIM_IDLE)
			{
				_animBlender.blend("idle",AnimationBlender::BlendSwitch,.2f,true);
			}
			else
			{
				//debug statement.
				std::cout << "idle not activated!" << "REL:" << _reloading << " MOV:" << _moving << " FIR:" << _firing << std::endl;
			}
		}
	}
	else
	{
		//for debug statements and such.
	}
	
	//for some of the other functions in the class.
	_playingAnim = getAnimID(_animBlender.getSource()->getAnimationName());
	
	//going to put sound here.
	//Seems easier for me to do.
	int sndID = getCorrespondingSoundID(_playingAnim);
	float animRelPos = _animBlender.getSource()->getTimePosition() / _animBlender.getSource()->getLength();
	bool check = false;
	_soundChannel->isPlaying(&check);
	if(!check) { _soundPlayed = false; }

	if( abs(_sounds[sndID].soundInfo.relativePosition - animRelPos) < 0.075f && 
		_sounds[sndID].soundInfo.relativePosition != 0)
	{
		//std::cout << "SndRelPos:" << _sounds[sndID].soundInfo.relativePosition;
		//std::cout << " AnimRelPos: " << animRelPos << std::endl;
		if(_sounds[sndID].sound.sound != nullptr && !_soundPlayed)
		{
				std::cout << "Sound is played" << std::endl;
				_soundChannel->stop();
				_soundChannel = _soundMgr->playSound(_sounds[sndID].sound);
				_soundPlayed = true;
		}
	}
	
	return true;
}

bool cGunData::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	//not needed right now I think.
	return true;
}

bool cGunData::frameEnded(const Ogre::FrameEvent& evt)
{
	//not needed right now.
	return true;
}

void cGunData::setSoundFrames(weapon_t* Weapon,SoundManager* Sound)
{
	_soundMgr = Sound;
	for(auto itr = Weapon->soundFrames().soundframe().begin();
		itr != Weapon->soundFrames().soundframe().end();
		++itr)
	{
		sSoundFrame soundF;
		soundF.gunSound = getSoundID(itr->sound());
		soundF.frame = static_cast<int>((*itr).frame());

		for(auto _itr = Weapon->animationFrames().animationFrame().begin();
			_itr != Weapon->animationFrames().animationFrame().end();
			++_itr)
		{
			int id = getAnimID((*_itr).animationName());
			if(isCorrespondAnimSoundID(id,soundF.gunSound))
			{
				int animLen = static_cast<int>((*_itr).end() - (*_itr).begin());
				int relPos = soundF.frame - static_cast<int>((*_itr).begin());
				soundF.relativePosition = static_cast<float>(relPos) / static_cast<float>(animLen);
			}
		}

		sGunSound sg;
		sg.soundInfo = soundF;

		sSound s;
		s.is3D = true;
		s.isLooping = false;
		s.name = itr->sound();
		s.type = SFX;

		std::string fileName;
		switch(soundF.gunSound)
		{
		case SND_FIRE:
			fileName = Weapon->sounds().fire();
			break;
		case SND_RELOAD:
			fileName = Weapon->sounds().reload();
			break;
		case SND_PUTAWAY:
			fileName = Weapon->sounds().putaway();
			break;
		case SND_DRYFIRE:
			fileName = Weapon->sounds().dryfire();
			break;
		case SND_ALTRELOAD:
			fileName = Weapon->sounds().altreload();
			break;
		case SND_ALTFIRE:
			fileName = Weapon->sounds().altfire();
			break;
		default:
			fileName = "NULL";
			break;
		}

		if(fileName != "NULL")
			Sound->createSound(s,fileName);
		else
			s.sound = NULL;

		sg.sound = s;
		_sounds[soundF.gunSound] = sg;
	}
	std::cout << "Soundframes set for " << Weapon->name() << std::endl;
}

void cGunData::setAnimationFrames(Ogre::Entity* entity)
{
	if(entity->getName() == "entM9SE")
	{
		if(entity->getMesh()->getSkeleton()->getBone("EAI2008_ROOT"))
		{
			//entity->getMesh()->getSkeleton()->getBone("EAI2008_ROOT")->setScale(.5f,.5f,.5f);
			std::cout << "Bone scaled" << std::endl;
		}
	}
	_animBlender.setEntity(entity);
	_animBlender.init("idle",true);
	std::cout << "AnimationFrames set for " << entity->getName() << std::endl;

	_gunEntityName = entity->getName();
}

cGunData::~cGunData()
{
	for(auto itr = _sounds.begin(); itr != _sounds.end(); ++itr)
	{
		(*itr).second.sound.sound->release();
	}

	_soundChannel->stop();
}

bool cGunData::isAnimationAlmostDone(Ogre::AnimationState* anim)
{
	Ogre::Real len = anim->getLength(),pos = anim->getTimePosition();

	if( (pos / len) > .9 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

int cGunData::getAnimID(const std::string& name)
{
	if(name == "idle")
	{
		return cGunData::ANIM_IDLE;
	}

	if(name == "startfire")
	{
		return cGunData::ANIM_STARTFIRE;
	}

	if(name == "fire")
	{
		return cGunData::ANIM_FIRE;
	}

	if(name == "endfire")
	{
		return cGunData::ANIM_MOVE;
	}

	if(name == "move")
	{
		return cGunData::ANIM_MOVE;
	}

	if(name == "autofire")
	{
		return cGunData::ANIM_AUTOFIRE;
	}

	if(name == "reload")
	{
		return cGunData::ANIM_RELOAD;
	}

	if(name == "select")
	{
		return cGunData::ANIM_SELECT;
	}

	if(name == "putaway")
	{
		return cGunData::ANIM_PUTAWAY;
	}

	return cGunData::NO_ANIM;
}

int cGunData::getSoundID(const std::string& sound)
{
	if(sound == "DRYFIRE")
	{
		return cGunData::SND_DRYFIRE;
	}
	if(sound == "FIRE")
	{
		return cGunData::SND_FIRE;
	}
	if(sound == "ALTFIRE")
	{
		return cGunData::SND_ALTFIRE;
	}
	if(sound == "ALTRELOAD")
	{
		return cGunData::SND_ALTRELOAD;
	}
	if(sound == "RELOAD")
	{
		return cGunData::SND_RELOAD;
	}
	if(sound == "PUTAWAY")
	{
		return cGunData::SND_PUTAWAY;
	}

	return cGunData::NO_SOUND;
}

bool cGunData::isCorrespondAnimSoundID(int animID,int soundID)
{
	if(animID == cGunData::ANIM_FIRE || animID == cGunData::ANIM_STARTFIRE && soundID == cGunData::SND_FIRE)
	{
		return true;
	}

	if(animID == cGunData::ANIM_RELOAD && soundID == cGunData::SND_RELOAD)
	{
		return true;
	}

	if((animID == cGunData::ANIM_PUTAWAY || animID == cGunData::ANIM_SELECT) && soundID == cGunData::SND_PUTAWAY)
	{
		return true;
	}

	return false;
}

int cGunData::getCorrespondingSoundID(int animID)
{
	switch(animID)
	{
	case cGunData::ANIM_STARTFIRE:
	case cGunData::ANIM_AUTOFIRE:
	case cGunData::ANIM_FIRE:
		return cGunData::SND_FIRE;
		break;
	case cGunData::ANIM_PUTAWAY:
	case cGunData::ANIM_SELECT:
		return cGunData::SND_PUTAWAY;
		break;
	case cGunData::ANIM_RELOAD:
		return cGunData::SND_RELOAD;
		break;
	//have to add support for dryfire and stuff later on.
	default:
		return cGunData::NO_SOUND;
		break;
	}

	return cGunData::NO_SOUND;
}