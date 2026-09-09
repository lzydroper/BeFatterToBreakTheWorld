#include "SoundEffect.h"

SoundEffect::SoundEffect()
{
	
}

SoundEffect::~SoundEffect()
{
}

void SoundEffect::load(std::string key, const std::string path)
{
	if (!Buffer[key].loadFromFile(path))
		std::cout << "载入bgs失败fuck：载入" << key << "从目录" << path << "\n";
	else
	{
		Sound[key].setBuffer(Buffer[key]);
	}
}

void SoundEffect::play(std::string key)
{
	Sound[key].play();
}

void SoundEffect::setVolmue(int volmue_level)
{
	for (auto& i : Sound)
		i.second.setVolume(volmue_level * 20);
}
