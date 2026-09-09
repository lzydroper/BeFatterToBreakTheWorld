#pragma once
#include "stdafx.h"
/*
	
*/
class BackgroundMusic
{
private:
	std::string activeMusic;
	std::map<std::string, sf::Music> music;
public:
	BackgroundMusic();
	~BackgroundMusic();

	//func
	void load(std::string key,const std::string path);
	void play(std::string key = "NULL", bool setLoop = true);
	void pause(std::string key = "NULL");
	void stop(std::string key = "NULL");

	void setVolmue(int volmue_level, std::string key = "NULL");

};