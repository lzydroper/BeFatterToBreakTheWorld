#include "BackgroundMusic.h"

BackgroundMusic::BackgroundMusic()
{
}

BackgroundMusic::~BackgroundMusic()
{
}

void BackgroundMusic::load(std::string key, const std::string path)
{
	if (!music[key].openFromFile(path))
		std::cout << "载入bgm失败fuck：载入" << key << "从目录" << path << "\n";
	else
		std::cout << "载入bgm：" << key << "从目录" << path << "\n";
}

void BackgroundMusic::play(std::string key, bool setLoop)
{
	if (key == "NULL")
		key = activeMusic;
	if (!key.empty())
	{
		music[key].play();
		music[key].setLoop(setLoop);
		std::cout << "播放bgm：" << key << "\n";
	}
	else
	{
		std::cout << "BackgroundMusic::play::没有正在播放的音乐！\n";
	}

	activeMusic = key;
}

void BackgroundMusic::pause(std::string key)
{
	if (key == "NULL")
		key = activeMusic;
	if (!key.empty())
		music[key].pause();
	else
		std::cout << "BackgroundMusic::pause::没有正在播放的音乐！\n";
}

void BackgroundMusic::stop(std::string key)
{
	if (key == "NULL")
		key = activeMusic;
	if (!key.empty())
		music[key].stop();
	else
		std::cout << "BackgroundMusic::stop::没有正在播放的音乐！\n";

	std::cout << "关闭bgm：" << key << "\n";

	activeMusic.clear();
}

void BackgroundMusic::setVolmue(int volmue_level, std::string key)
{
	if (key == "NULL")
		key = activeMusic;
	if (!key.empty())
		music[key].setVolume(volmue_level * 20);
	else
		std::cout << "BackgroundMusic::setVolmue::没有正在播放的音乐！\n";

	std::cout << "music" << key << "volmue_level is :" << volmue_level << "\n";
}
