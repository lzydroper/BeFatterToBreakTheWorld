#pragma once
#include "stdafx.h"
#include "BackgroundMusic.h"
#include "SoundEffect.h"

class AudioManager
{
public:
	BackgroundMusic* bgm;
	SoundEffect* bgs;

	AudioManager();
	~AudioManager();

	//init
	void init_bgm();
	void init_bgs();

	void setVolume(int volmue_level);
	void randomMusic(std::string musicType, int choices, bool setLoop = false);
};

