#include "StdAfx.h"

#include <fmod.hpp>

#include <string>
#include <map>
#include <queue>

#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

enum SOUND_TYPE
{
	MUSIC = 0,
	SFX,
	CHARACTER
};

class SoundStruct
{
public:
	SoundStruct(){sound = 0; is3D = false; isLooping = false;}
	FMOD::Sound* sound;
	std::string name;
	SOUND_TYPE type;
	bool is3D;
	bool isLooping;
};

//whoopsie
typedef SoundStruct sSound;

class SoundManager
{
public:
	~SoundManager(){Shutdown();}
	bool Setup();
	bool Update();
	void Shutdown();

	//Sound functions
	void createSound(sSound& sound,const std::string& fileName);

	FMOD::Channel* playSound(const sSound& sound);

	void destroySound(sSound& sound);

	//Music functions
	//functions that would apply to sounds used as music.
	void startMusic();
	void addMusicToPlaylist(const sSound& sound);
	void stopMusic(bool clearAllMusic = false);

private:
	void _fadeTransition();
	void _reportError();

	FMOD::System* _system;
	FMOD_RESULT _errResult;

	std::queue<FMOD::Sound*> _musicPlayList;
	FMOD::Channel* _musicChannel;

	int _sfxVolume;
	int _mscVolume;
	int _charVolume;
};

#endif