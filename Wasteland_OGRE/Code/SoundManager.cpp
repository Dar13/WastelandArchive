#include "StdAfx.h"

#include "SoundManager.h"
#include <fmod_errors.h>
#include "debug\console.h"

bool SoundManager::Setup()
{
	bool success = true;
	FMOD_RESULT res;
	res = FMOD::System_Create(&_system);
	std::string resStr;
	if(res != FMOD_OK)
	{
		_reportError();
		success = false;
		return success;
	}

	res = _system->init(500,FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if(res != FMOD_OK)
	{
		_reportError();
		success = false;
		return success;
	}

	return success;
}

bool SoundManager::Update()
{
	bool ret = true;

	_errResult = _system->update();
	if(_errResult != FMOD_OK)
	{
		std::string s = FMOD_ErrorString(_errResult);
		VirtualConsole::getSingletonPtr()->put("FMOD Error!\n" + s);
		ret = false;
	}

	//somehow get the new configuration settings.

	//update the music stuff.
	bool p = true;
	_musicChannel->isPlaying(&p);
	if(p == false)
	{
		_musicChannel->stop();
		sSound tmp;
		tmp.name = "menuMusic";
		tmp.sound = _musicPlayList.front();
		tmp.type = MUSIC;
		_musicChannel = playSound(tmp);
		if(_musicPlayList.size() > 2)
		{
			_musicPlayList.front()->release();
			_musicPlayList.pop();
		}
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

	//set channel volume based on configuration values stored in class.

}

FMOD::Channel* SoundManager::playSound(const sSound& sound)
{
	FMOD::Channel* channel;
	_errResult = _system->playSound(FMOD_CHANNEL_FREE,sound.sound,false,&channel);
	if(_errResult != FMOD_OK)
	{
		_reportError();
	}

	return channel;
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
		sSound music;
		music.is3D = false;
		music.isLooping = false;
		music.name = "menuMusic";
		music.sound = _musicPlayList.front();
		music.type = MUSIC;
		_musicChannel = playSound(music);
	}
}

void SoundManager::addMusicToPlaylist(const sSound& sound)
{
	_musicPlayList.push(sound.sound);
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
		int size = _musicPlayList.size();
		for(int i = 0; i < size; i++)
		{
			FMOD::Sound* sTmp = _musicPlayList.front();
			_errResult = sTmp->release();
			if(_errResult != FMOD_OK)
			{
				_reportError();
			}
			_musicPlayList.pop();
		}
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
	VirtualConsole::getSingletonPtr()->put("FMOD Error!\n" + s);
}