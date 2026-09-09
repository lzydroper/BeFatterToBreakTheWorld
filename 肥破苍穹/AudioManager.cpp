#include "AudioManager.h"

AudioManager::AudioManager()
{
	bgm = new BackgroundMusic();
	bgs = new SoundEffect();

	init_bgm();
	init_bgs();
}

AudioManager::~AudioManager()
{
	delete bgm;
	delete bgs;
}

void AudioManager::init_bgm()
{
	//load bgm
	bgm->load("MainMenu", "Resources/Music/MainMenu.mp3");
	bgm->load("PauseMenu", "Resources/Music/PauseMenu.mp3");
	bgm->load("EditorState", "Resources/Music/EditorState.mp3");
	bgm->load("GameFight", "Resources/Music/GameFight.mp3");
	bgm->load("Gameover1", "Resources/Music/Gameover1.mp3");
	bgm->load("Gameover2", "Resources/Music/Gameover2.mp3");
	bgm->load("Gameover3", "Resources/Music/Gameover3.mp3");
	bgm->load("Gameover4", "Resources/Music/Gameover4.mp3");
	bgm->load("Victory1", "Resources/Music/Victory1.mp3");
	bgm->load("Victory2", "Resources/Music/Victory2.mp3");
	bgm->load("Victory3", "Resources/Music/Victory3.mp3");
}

void AudioManager::init_bgs()
{
	//load bgs
	bgs->load("Hurt", "Resources/Sound/hurt.mp3");
	bgs->load("LevelUp", "Resources/Sound/levelUp.mp3");
	bgs->load("GainExp", "Resources/Sound/gainExp.mp3");
	bgs->load("mobHurt", "Resources/Sound/mobHurt.mp3");
	bgs->load("mobDeath", "Resources/Sound/mobDeath.mp3");
}

void AudioManager::setVolume(int volmue_level)
{
	bgm->setVolmue(volmue_level);
	bgs->setVolmue(volmue_level);
}

void AudioManager::randomMusic(std::string musicType, int choices, bool setLoop)
{
	std::stringstream ss;

	ss << musicType << (rand() % choices) + 1;

	bgm->play(ss.str(), setLoop);
}
