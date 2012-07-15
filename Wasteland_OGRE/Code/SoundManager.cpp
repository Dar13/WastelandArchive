#include "StdAfx.h"

#include "SoundManager.h"
#include <fmod_errors.h>
#include "debug\console.h"
#include "interfaces\configuration.hxx"

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

	res = _system->init(500,FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if(res != FMOD_OK)
	{
		_errResult = res;
		_reportError();
		success = false;
		return success;
	}

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
	_charVolume = static_cast<float>(currentConfiguration->volume().character()) / 100.0f;
	_mscVolume = static_cast<float>(currentConfiguration->volume().music()) / 100.0f;
	_sfxVolume = static_cast<float>(currentConfiguration->volume().soundfx()) / 100.0f;

	//update the music stuff.
	unsigned int position;
	unsigned int length;
	_musicChannel->getPosition(&position,FMOD_TIMEUNIT_MS);
	_musicChannel->setVolume( _mscVolume );
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
	
	return ret;
}

void SoundManager::Shutdown()
{
	_system->release();
	return;
}

void SoundManager::createSound(sSound& sound,const std::string& fileName)
{
	FMOD::Sound* nSound;

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
		_errResult = _system->createSound(fileName.c_str(),mode | FMOD_SOFTWARE | FMOD_VIRTUAL_PLAYFROMSTART,0,&nSound);
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
		_errResult = _system->createSound(fileName.c_str(),mode | FMOD_SOFTWARE | FMOD_VIRTUAL_PLAYFROMSTART,0,&nSound);
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
	_errResult = _system->playSound(FMOD_CHANNEL_FREE,sound.sound,false,&channel);
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
	std::cout << "FMOD Error!" << std::endl << s << std::endl;
}