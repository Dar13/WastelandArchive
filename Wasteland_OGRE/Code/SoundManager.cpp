#include "StdAfx.h"

#include "SoundManager.h"
#include <fmod_errors.h>
#include "debug\console.h"
#include "interfaces\configuration.hxx"

#include "Utility.h"

bool SoundStruct::operator==(const SoundStruct& rhs)
{
	if(name == rhs.name && 
	   is3D == rhs.is3D && 
	   isLooping == rhs.isLooping && 
	   sound == rhs.sound && 
	   type == rhs.type)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool SoundStruct::operator==(const std::string& name)
{
	if(this->name == name)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool SoundStruct::operator!=(const SoundStruct& rhs)
{
	if( name != rhs.name ||
		is3D != rhs.is3D ||
		isLooping != rhs.isLooping ||
		sound != rhs.sound ||
		type != rhs.type)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool SoundStruct::operator!=(const std::string& name)
{
	if(this->name != name)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool SoundManager::Setup()
{
	bool success = true;
	FMOD_RESULT res;
	res = FMOD::System_Create(&_system);
	std::string resStr;
	if(res != FMOD_OK)
	{
		_errResult = res;
		_reportError();
		success = false;
		return success;
	}

	_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
	_system->setDSPBufferSize(1024,10);

	res = _system->init(250,FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if(res != FMOD_OK)
	{
		_errResult = res;
		_reportError();
		success = false;
		return success;
	}

	_musicPlaying = false;

	return success;
}

bool SoundManager::Update(configuration_t* currentConfiguration)
{
	bool ret = true;

	_errResult = _system->update();
	if(_errResult != FMOD_OK)
	{
		std::string s = FMOD_ErrorString(_errResult);
		std::cout << "FMOD error! : " << s << std::endl;
		ret = false;
	}

	//somehow get the new configuration settings.
	if(currentConfiguration != nullptr)
	{
		_charVolume = static_cast<float>(currentConfiguration->volume().character()) / 100.0f;
		_mscVolume = static_cast<float>(currentConfiguration->volume().music()) / 100.0f;
		_sfxVolume = static_cast<float>(currentConfiguration->volume().soundfx()) / 100.0f;
	}


	if(_musicPlaying)
	{
		//update the music stuff.
		unsigned int position;
		unsigned int length;

		if(!_musicFade)
		{
			_musicChannel->setVolume( _mscVolume );
		}
		else
		{
			_musicChannel->setVolume(_musicFadeVolume);
		}

		_musicChannel->getPosition(&position,FMOD_TIMEUNIT_MS);
		_musicPlayList.front()->getLength(&length,FMOD_TIMEUNIT_MS);
		if((length - position) <= 100)
		{
			_musicChannel->stop();
			sSound tmp;
			tmp.name = "menuMusic";
			if( (_musicItr + 1) == _musicPlayList.end())
			{
				_musicItr = _musicPlayList.begin();
			}
			else
			{
				++_musicItr;
			}
			tmp.sound = (*_musicItr);
			tmp.type = MUSIC;
			_musicChannel = playSound(tmp);
		}
	}
	
	return ret;
}

bool SoundManager::Update(configuration_t* currentConfiguration,Ogre::Camera* cameraListener)
{
	bool ret = true;

	_errResult = _system->update();
	if(_errResult != FMOD_OK)
	{
		_reportError();
		ret = false;
	}

	if(currentConfiguration != nullptr)
	{
		_charVolume = static_cast<float>(currentConfiguration->volume().character()) / 100.0f;
		_mscVolume = static_cast<float>(currentConfiguration->volume().music()) / 100.0f;
		_sfxVolume = static_cast<float>(currentConfiguration->volume().soundfx()) / 100.0f;
	}

	if(_musicPlaying)
	{
		//update the music stuff.
		unsigned int position;
		unsigned int length;

		if(!_musicFade)
		{
			_musicChannel->setVolume( _mscVolume );
		}
		else
		{
			_musicChannel->setVolume(_musicFadeVolume);
		}

		_musicChannel->getPosition(&position,FMOD_TIMEUNIT_MS);
		_musicPlayList.front()->getLength(&length,FMOD_TIMEUNIT_MS);
		if((length - position) <= 100)
		{
			_musicChannel->stop();
			sSound tmp;
			tmp.name = "menuMusic";
			if( (_musicItr + 1) == _musicPlayList.end())
			{
				_musicItr = _musicPlayList.begin();
			}
			else
			{
				++_musicItr;
			}
			tmp.sound = (*_musicItr);
			tmp.type = MUSIC;
			_musicChannel = playSound(tmp);
		}
	}

	if(cameraListener != nullptr)
	{
		FMOD_VECTOR pos,vel,fwd,up;
		pos = Utility::ogreToFMOD(cameraListener->getRealPosition());
		vel = Utility::ogreToFMOD(Ogre::Vector3(0.0f,0.0f,0.0f));
		fwd = Utility::ogreToFMOD(cameraListener->getRealDirection().normalisedCopy());
		up = Utility::ogreToFMOD(cameraListener->getRealUp().normalisedCopy());
		_system->set3DListenerAttributes(0,&pos,&vel,&fwd,&up);
	}

	return ret;
}

void SoundManager::Shutdown()
{
	_system->release();
	return;
}

void SoundManager::createSound(sSound& sound,const std::string& fileName)
{
	FMOD::Sound* nSound = nullptr;

	unsigned int mode = 0;
	if(sound.isLooping)
	{
		mode = FMOD_LOOP_NORMAL;
	}
	if(sound.is3D)
	{
		if(mode == 0)
		{
			mode = FMOD_3D;
		}
		else
		{
			mode = mode | FMOD_3D;
		}
	}

	switch(sound.type)
	{
	case SFX:
		_errResult = _system->createSound(fileName.c_str(),mode | FMOD_SOFTWARE,0,&nSound);
		if(_errResult != FMOD_OK)
		{
			_reportError();
		}
		break;
	case MUSIC:
		//default to stream due to larger nature of music files.
		_errResult = _system->createStream(fileName.c_str(),mode | FMOD_SOFTWARE,0,&nSound);
		if(_errResult != FMOD_OK)
		{
			_reportError();
		}
		break;
	case CHARACTER:
		_errResult = _system->createSound(fileName.c_str(),mode | FMOD_SOFTWARE,0,&nSound);
		if(_errResult != FMOD_OK)
		{
			_reportError();
		}
		break;
	}
	sound.sound = nSound;
	return;
}

FMOD::Channel* SoundManager::playSound(const sSound& sound)
{
	FMOD::Channel* channel;
	_errResult = _system->playSound(FMOD_CHANNEL_FREE,sound.sound,true,&channel);
	if(_errResult != FMOD_OK)
	{
		_reportError();
	}

	switch(sound.type)
	{
	case SFX:
		channel->setVolume(_sfxVolume);
		break;
	case MUSIC:
		channel->setVolume(_mscVolume);
		break;
	case CHARACTER:
		channel->setVolume(_charVolume);
		break;
	}

	channel->setPaused(false);

	return channel;
}

FMOD::Channel* SoundManager::playSound(const sSound& sound,Ogre::Vector3& position,Ogre::Vector3& velocity)
{
	FMOD_VECTOR pos = Utility::ogreToFMOD(position);
	FMOD_VECTOR vel = Utility::ogreToFMOD(velocity);
	return playSound(sound,pos,vel);
	
}

FMOD::Channel* SoundManager::playSound(const sSound& sound,FMOD_VECTOR& position,FMOD_VECTOR& velocity)
{
	FMOD::Channel* channel;
	_errResult = _system->playSound(FMOD_CHANNEL_FREE,sound.sound,true,&channel);
	if(_errResult != FMOD_OK)
	{
		_reportError();
	}

	switch(sound.type)
	{
	case SFX:
		channel->setVolume(_sfxVolume);
		break;
	case MUSIC:
		channel->setVolume(_mscVolume);
		break;
	case CHARACTER:
		channel->setVolume(_charVolume);
		break;
	}

	channel->set3DAttributes(&position,&velocity);

	channel->setPaused(false);

	return channel;
}

void SoundManager::updateChannels(std::vector<FMOD::Channel*>* channelVector)
{
	for(auto itr = channelVector->begin(); itr != channelVector->end(); ++itr)
	{
		bool playing = false;
		(*itr)->isPlaying(&playing);
	}
}

void SoundManager::destroySound(sSound& sound)
{
	sound.sound->release();
	sound.sound = 0;
}

void SoundManager::startMusic()
{
	if(_musicPlayList.empty() == false)
	{
		_musicItr = _musicPlayList.begin();
		sSound music;
		music.is3D = false;
		music.isLooping = false;
		music.name = "menuMusic";
		music.sound = (*_musicItr);
		music.type = MUSIC;
		_musicChannel = playSound(music);
		_musicPlaying = true;
	}
}

void SoundManager::addMusicToPlaylist(const sSound& sound)
{
	_musicPlayList.push_back(sound.sound);
}

void SoundManager::stopMusic(bool clearAllMusic)
{
	_errResult = _musicChannel->stop();
	if(_errResult != FMOD_OK)
	{
		_reportError();
	}

	if(clearAllMusic)
	{
		for(_musicItr = _musicPlayList.begin(); _musicItr != _musicPlayList.end(); ++_musicItr)
		{
			FMOD::Sound* sTmp = (*_musicItr);
			_errResult = sTmp->release();
			if(_errResult != FMOD_OK)
			{
				_reportError();
			}
		}
		_musicPlayList.clear();
		_musicItr = _musicPlayList.begin();
	}

	_musicPlaying = false;
}

/*
void _fadeTransition()
{
	return;
}
*/

inline void SoundManager::_reportError()
{
	std::string s = FMOD_ErrorString(_errResult);
	std::cout << "FMOD Error!" << " : " << s << std::endl;
}