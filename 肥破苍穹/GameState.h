#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "PauseMenu.h"
#include "GameOverPause.h"
#include "LevelUpPause.h"
#include "TileMap.h"
#include "PlayerGUI.h"
#include "Items.h"
#include "TextTagSystem.h"
#include "SettingsState.h"

class GameState :
	public State
{
private:
	sf::View view;
	sf::Vector2i viewGridPosition;
	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	sf::Font font;
	PauseMenu* pmenu;

	GameOverPause* gameover;
	enum gameState { GAME = 0, PAUSE, LEVELUP, GAMEOVER };
	gameState state;
	std::vector<int> levelup_chioce;
	LevelUpPause* levelupPause;

	sf::Shader core_shader;

	sf::Clock keyTimer;
	float keyTimeMax;

	sf::Text debugText;

	Player* player;
	PlayerGUI* playerGUI;
	sf::Texture texture;
	
	std::vector<Enemy*> activeEnemies;
	EnemySystem *enemySystem;

	TileMap* tileMap;

	//Systems
	TextTagSystem* tts;

	//Functions
	void initDeferredRender();
	void initView();
	void initKeybinds();
	void initFonts();
	void initTextures();
	void initPauseMenu();
	void initLevelupPause();
	void initShaders();
	void initKeyTime();
#if test
	void initDebugText();
#endif // test


	void initPlayers();
	void initPlayerGUI();
	void initEnemySystem();
	void initTileMap();
	void initSystems();

	void initAudio();

public:
	GameState(StateData* state_data);
	virtual ~GameState();

	//Accessors
	const bool getKeyTime();

	//Functions
	void updateView(const float& dt);
	void updateInput(const float& dt);
	void updatePlayerInput(const float& dt);
	void updatePlayerGUI(const float& dt);
	void updatePauseMenuButtons();
	void updateLevelupPause();
	void updateGameover();
	void updateTileMap(const float& dt);
	void updatePlayer(const float& dt);
	void updateCombatAndEnemies(const float& dt);
	void updateCombat(Enemy* enemy, const int index, const float& dt);
	void updatePlayerEvent();
#if test
	void updateDebugText(const float& dt);
#endif // test

	void update(const float& dt);

	void render(sf::RenderTarget* target = NULL);
};

#endif