#pragma once
#include "stdafx.h"
class SoundEffect
{
private:
	std::map<std::string, sf::SoundBuffer> Buffer;
	std::map<std::string, sf::Sound> Sound;
public:
	SoundEffect();
	~SoundEffect();

	//
	void load(std::string key, const std::string path);
	void play(std::string key);

	void setVolmue(int volmue_level);
};