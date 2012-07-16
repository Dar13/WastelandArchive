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
	SoundStruct(){sound = 0; is3D = false; isLooping = false; type = MUSIC;}
	FMOD::Sound* sound;
	std::string name;
	SOUND_TYPE type;
	bool is3D;
	bool isLooping;
};

//Forward declaration of class.
class configuration_t;

//whoopsie
typedef SoundStruct sSound;

class SoundManager
{
public:
	~SoundManager(){Shutdown();}
	bool Setup();
	bool Update(configuration_t* currentConfiguration);
	void Shutdown();

	//Sound functions
	void createSound(sSound& sound,const std::string& fileName);

	FMOD::Channel* playSound(const sSound& sound);

	void updateChannels(std::vector<FMOD::Channel*>* channelVector);

	void destroySound(sSound& sound);

	//Music functions
	//functions that would apply to sounds used as music.
	void startMusic();
	void addMusicToPlaylist(const sSound& sound);
	void stopMusic(bool clearAllMusic = false);
	void setMusicFade(bool fade = false) { _musicFade = fade; }
	void setMusicFadeVolume(float volume) { if(_musicFade) { _musicFadeVolume = volume; } }
	float getDefaultMusicVolume() { return _mscVolume; }

private:
	//void _fadeTransition();
	void _reportError();

	FMOD::System* _system;
	FMOD_RESULT _errResult;

	std::vector<FMOD::Sound*> _musicPlayList;
	std::vector<FMOD::Sound*>::iterator _musicItr;
	FMOD::Channel* _musicChannel;

	float _sfxVolume;
	float _mscVolume;
	float _charVolume;

	bool _musicFade;
	float _musicFadeVolume;
};

#endif