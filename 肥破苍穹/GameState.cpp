#include "stdafx.h"
#include "GameState.h"

void GameState::initDeferredRender()
{
	this->renderTexture.create(
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height
	);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(
		sf::IntRect(
			0, 
			0, 
			this->stateData->gfxSettings->resolution.width, 
			this->stateData->gfxSettings->resolution.height
		)
	);
}

//Initializer functions
void GameState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width / 2),
			static_cast<float>(this->stateData->gfxSettings->resolution.height / 2)
		)
	);

	this->view.setCenter(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width) / 2.f,
			static_cast<float>(this->stateData->gfxSettings->resolution.height) / 2.f
		)
	);
}

void GameState::initKeybinds()
{
	std::ifstream ifs("Config/gamestate_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();
}

void GameState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void GameState::initTextures()
{
	if (!this->textures["PLAYER_SHEET"].loadFromFile("Resources/Images/Sprites/Player/PLAYER_SHEET2.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_TEXTURE";
	}

	if(!this->textures["RAT1_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/rat1_60x64.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_RAT1_TEXTURE";
	}

	if (!this->textures["BIRD1_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/bird1_61x57.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_BIRD1_TEXTURE";
	}

	if (!this->textures["SPIDER_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/spider1_60x64.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_SPIDER_TEXTURE";
	}

	if (!this->textures["SCORPOION_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/scorpion1_60x64.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_SCORPOION_TEXTURE";
	}

	if (!this->textures["SLIME_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/slime1_60x64.png"))
	{
		std::cout << "ERROR::GAME_STATE::COULD_NOT_LOAD_SLIME_TEXTURE\n";
	}
}

void GameState::initPauseMenu()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	this->pmenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font);

	this->pmenu->addButton("QUIT", gui::p2pY(74.f, vm), gui::p2pX(13.f, vm), gui::p2pY(6.f, vm), gui::calcCharSize(vm), "Quit");

	//添加设置按钮
	this->pmenu->addButton("SETTING", gui::p2pY(24.f, vm), gui::p2pX(13.f, vm), gui::p2pY(6.f, vm), gui::calcCharSize(vm), "Settings");
}

void GameState::initLevelupPause()
{
	this->levelupPause = new LevelUpPause(this->stateData->gfxSettings->resolution, this->font, this->levelup_chioce);
}

void GameState::initShaders()
{
	if (!this->core_shader.loadFromFile("Resources/Shader/vertex_shader.vert", "Resources/Shader/fragment_shader.frag"))
	{
		std::cout << "ERROR::GAMESTATE::COULD NOT LOAD SHADER." << "\n";
	}
}

void GameState::initKeyTime()
{
	this->keyTimeMax = 0.3f;
	this->keyTimer.restart();
}

#if test
void GameState::initDebugText()
{
	this->debugText.setFont(this->font);
	this->debugText.setFillColor(sf::Color::White);
	this->debugText.setCharacterSize(16);
	this->debugText.setPosition(15.f, this->window->getSize().y / 2.f);
}
#endif // test


void GameState::initPlayers()
{
	this->player = new Player(220, 220, this->textures["PLAYER_SHEET"]);
	this->player->getAttributeComponent()->length_basic = this->player->getWeapon()->weapon_sprite.getScale().y;
	this->player->getAttributeComponent()->length = this->player->getAttributeComponent()->length_basic;
}

void GameState::initPlayerGUI()
{
	this->playerGUI = new PlayerGUI(this->player, this->stateData->gfxSettings->resolution);


	//一开始关闭Tab
	this->playerGUI->toggleCharacterTab();
}

void GameState::initEnemySystem()
{
	this->enemySystem = new EnemySystem(this->activeEnemies, this->textures, *this->player);
}

void GameState::initTileMap()
{
	this->tileMap = new TileMap("Resources/map.slmp");
}

void GameState::initSystems()
{
	this->tts = new TextTagSystem("Fonts/PixellettersFull.ttf");
}

void GameState::initAudio()
{
	audioManager->bgm->play("GameFight");
	audioManager->setVolume(this->stateData->gfxSettings->volumelevel);
}

//Constructors / Destructors
GameState::GameState(StateData* state_data)
	: State(state_data)
{
	state = GAME;
	levelup_chioce = { 0, 1, 2, 3, 4, 5 };

	this->initDeferredRender();
	this->initView();
	this->initKeybinds();
	this->initFonts();
	this->initTextures();
	this->initPauseMenu();
	this->initLevelupPause();
	this->initShaders();
	this->initKeyTime();
#if test
	this->initDebugText();
#endif // test


	this->initPlayers();
	this->initPlayerGUI();
	this->initEnemySystem();
	this->initTileMap();
	this->initSystems();

	this->initAudio();
}

GameState::~GameState()
{
	delete this->pmenu;
	delete this->gameover;
	delete this->levelupPause;
	delete this->player;
	delete this->playerGUI;
	delete this->enemySystem;
	delete this->tileMap;
	delete this->tts;

	for (size_t i = 0; i < this->activeEnemies.size(); i++)
	{
		delete this->activeEnemies[i];
	}
}

const bool GameState::getKeyTime()
{	
	if (this->keyTimer.getElapsedTime().asSeconds() >= this->keyTimeMax)
	{
		this->keyTimer.restart();
		return true;
	}

	return false;	
}

//Functions
void GameState::updateView(const float & dt)
{
	this->view.setCenter(
		std::floor(this->player->getPosition().x + (static_cast<float>(this->mousePosWindow.x) - static_cast<float>(this->stateData->gfxSettings->resolution.width / 2)) / 10.f),
		std::floor(this->player->getPosition().y + (static_cast<float>(this->mousePosWindow.y) - static_cast<float>(this->stateData->gfxSettings->resolution.height / 2)) / 10.f)
	);
	
	if (this->tileMap->getMaxSizeF().x >= this->view.getSize().x)
	{
		if (this->view.getCenter().x - this->view.getSize().x / 2.f < 0.f)
		{
			this->view.setCenter(0.f + this->view.getSize().x / 2.f, this->view.getCenter().y);
		}
		else if (this->view.getCenter().x + this->view.getSize().x / 2.f > this->tileMap->getMaxSizeF().x)
		{
			this->view.setCenter(this->tileMap->getMaxSizeF().x - this->view.getSize().x / 2.f, this->view.getCenter().y);
		}
	}

	if (this->tileMap->getMaxSizeF().y >= this->view.getSize().y)
	{
		if (this->view.getCenter().y - this->view.getSize().y / 2.f < 0.f)
		{
			this->view.setCenter(this->view.getCenter().x, 0.f + this->view.getSize().y / 2.f);
		}
		else if (this->view.getCenter().y + this->view.getSize().y / 2.f > this->tileMap->getMaxSizeF().y)
		{
			this->view.setCenter(this->view.getCenter().x, this->tileMap->getMaxSizeF().y - this->view.getSize().y / 2.f);
		}
	}

	this->viewGridPosition.x = static_cast<int>(this->view.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	this->viewGridPosition.y = static_cast<int>(this->view.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void GameState::updateInput(const float & dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
	{
		if (!this->paused)
		{
			this->pauseState();
			state = PAUSE;
			std::cout << "state为：" << state << "\n";
			this->audioManager->bgm->pause();
		}
		else
		{
			this->unpauseState();
			state = GAME;
			std::cout << "state为：" << state << "\n";
			this->audioManager->bgm->play();
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CHEAT"))) && this->getKeytime())
	{
		//作弊模式，后端显示角色各种数据及其编号，玩家先输入编号，然后再输入需要改变的数值，最后应用改变
		/*
		<< "编号：0 , " << "等级: " << this->level << "\n"
		<< "编号：1 , " << "生命: " << this->hp << "\n"
		<< "编号：2 , " << "力量: " << this->liliang << "\n"
		<< "编号：3 , " << "敏捷: " << this->minjie << "\n"
		<< "编号：4 , " << "防御: " << this->fangyu << "\n"
		<< "编号：5 , " << "幸运: " << this->luck << "\n"
		<< "编号：6 , " << "剑长: " << this->length << "\n"
		<< "编号：7 , " << "经验: " << this->exp << "\n"
		<< "编号：8 , " << "退出修改\n";
		*/
		AttributeComponent* a = this->player->getAttributeComponent();
		std::cout << "当前数据为：\n" << a->debugPrint();
		int code = 0;
		while (code != 8)
		{
			std::cout << "请输入需要修改的编号：";
			std::cin >> code;
			std::cout << "请输入需要修改的数值：";
			int value = 0;
			if (code != 8)
			{
				std::cin >> value;
			}
			switch (code)
			{
			case 0:
				a->level = value;
				a->expNext = 75 + (50 * pow(a->level, 3) - 150 * pow(a->level, 2) + (UINT64)400 * a->level) / 12;
				a->reset = true;
				break;
			case 1:
				a->point_h = value - a->hpMax;
				a->hp = value;
				break;
			case 2:
				a->point_l = value - a->liliang;
				break;
			case 3:
				a->point_m = value - a->minjie;
				break;
			case 4:
				a->point_f = value - a->fangyu;
				break;
			case 5:
				a->point_x = value - a->luck;
				break;
			case 6:
				a->point_s = value - a->length;
				break;
			case 7:
				a->exp = value;
				a->updateLevel();
				a->reset = true;
				break;
			case 8:
				std::cout << "退出修改\n";
				break;
			default:
				std::cout << "编号错误\n";
				break;
			}
			this->player->getWeapon()->weapon_sprite.setScale(this->player->getWeapon()->weapon_sprite.getScale().x, a->length);
			this->player->getMovementComponent()->maxVelocity = 140.f + a->minjie * 7.f;
			a->updateStats(true);
			std::cout << "......正在生效......\n";
		}
	}
}

void GameState::updatePlayerInput(const float & dt)
{
	//Update player input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))
		this->player->move(-1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))
		this->player->move(1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))))
	{
		this->player->move(0.f, -1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))
	{
		this->player->move(0.f, 1.f, dt);
	}
}

void GameState::updatePlayerGUI(const float & dt)
{
	this->playerGUI->update(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("TOGGLE_PLAYER_TAB_CHARACTER"))) && this->getKeyTime())
	{
		this->playerGUI->toggleCharacterTab();
	}
}

void GameState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("QUIT"))	
	{
		this->endState();
		audioManager->bgm->stop();
		audioManager->bgm->play("MainMenu");
		audioManager->setVolume(this->stateData->gfxSettings->volumelevel);
	}

	//尝试加入设置
	//Settings
	if (this->pmenu->isButtonPressed("SETTING"))
	{
		this->states->push(new SettingsState(this->stateData));
	}
}

void GameState::updateLevelupPause()
{
	if (this->levelupPause->isButtonPressed())
	{
		AttributeComponent* a = this->player->getAttributeComponent();
		switch (this->levelupPause->selected)
		{
		case 0:
			a->point_h += 50;
			a->hp += 50;
			break;
		case 1:
			a->point_l += 5;
			break;
		case 2:
			a->point_m += 5;
			break;
		case 3:
			a->point_f += 5;
			break;
		case 4:
			a->point_x += 5;
			break;
		case 5:
			a->point_s += 0.5;
			break;
		default:
			std::cout << "LEVELUP::PAUSE::错误选择，选择代码：" << this->levelupPause->selected << "\n";
			break;
		}
		a->updateStats(true);
		this->player->getWeapon()->weapon_sprite.setScale(this->player->getWeapon()->weapon_sprite.getScale().x, a->length);
		this->player->getMovementComponent()->maxVelocity = 140.f + a->minjie * 7.f;
		state = GAME;
	}
}

void GameState::updateGameover()
{
	if (this->gameover->isButtonPressed("QUIT"))
	{
		this->endState();
		audioManager->bgm->stop();
		audioManager->bgm->play("MainMenu");
		audioManager->setVolume(this->stateData->gfxSettings->volumelevel);
	}
}

void GameState::updateTileMap(const float & dt)
{
	this->tileMap->updateWorldBoundsCollision(this->player, dt); 
	this->tileMap->updateTileCollision(this->player, dt);
	this->tileMap->updateTiles(this->player, dt, *this->enemySystem);
}

void GameState::updatePlayer(const float & dt)
{
	this->player->update(dt, this->mousePosView, this->view);


	//更新debuff
	if (player->poison->times > 0) 
	{
		if (player->poison->getDebuffTimerDone())
		{
			player->poison->resetDebuffTimer();
			player->poison->times--;

			int dmg = player->poison->dmg;
			this->player->loseHP(dmg);
			this->tts->addTextTag(DEBUFF_TAG, player->getPosition().x - 30.f, player->getPosition().y, dmg, "-", "HP");
			this->audioManager->bgs->play("Hurt");
		}
	}

	if (player->retard->debuffTimerMax > 0)
	{
		if (!player->retard->getDebuffTimerDone())
		{
			//还在debuff时间内，减速
			this->player->getMovementComponent()->maxVelocity = (140.f + this->player->attributeComponent->minjie * 7.f) * 0.65f;
			if(player->retard->getDebuffDisplayTimerDone())
			{
				player->retard->resetDebuffDisplayTimer();
				this->tts->addTextTag(DEBUFF_TAG, player->getPosition().x - 30.f, player->getPosition().y, "↓", "↓", "↓");
			}
		}
		else
		{
			//debuff时间结束，清零
			this->player->getMovementComponent()->maxVelocity = 140.f + this->player->attributeComponent->minjie * 7.f;
			player->retard->debuffTimerMax = 0;
			player->retard->resetDebuffTimer();
		}
	}
}

void GameState::updateCombatAndEnemies(const float & dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->player->getWeapon()->getAttackTimer())
		this->player->setInitAttack(true);

	unsigned index = 0;
	for (auto *enemy : this->activeEnemies)
	{
		enemy->update(dt, this->mousePosView, this->view);

		this->tileMap->updateWorldBoundsCollision(enemy, dt);
		this->tileMap->updateTileCollision(enemy, dt);

	  this->updateCombat(enemy, index, dt);

		//DANGEROUS!!!
		if (enemy->isDead())
		{
			std::cout << "这只怪的血量是：" << enemy->attributeComponent->hpMax << "\n";
			this->player->gainEXP(enemy->getGainExp());
			this->audioManager->bgs->play("mobDeath");
			this->audioManager->bgs->play("GainExp");
			this->tts->addTextTag(EXPERIENCE_TAG, this->player->getPosition().x - 40.f, this->player->getPosition().y - 30.f, static_cast<int>(enemy->getGainExp()), "+", "EXP");

			this->enemySystem->removeEnemy(index);
			continue;
		}
		else if (enemy->getDespawnTimerDone())
		{
			this->enemySystem->removeEnemy(index);
			continue;
		}

		++index;
	}

	this->player->setInitAttack(false);
}

void GameState::updateCombat(Enemy* enemy, const int index, const float & dt)
{
	if (this->player->getInitAttack()
		&& enemy->getGlobalBounds().intersects(this->player->getWeapon()->weapon_sprite.getGlobalBounds())
		//&& enemy->getDamageTimerDone()
		)
	{
		this->audioManager->bgs->play("mobHurt");

		std::string crap;
		int dmg = static_cast<int>(this->player->getDamage());
		if (rand() % 100 < this->player->getAttributeComponent()->luck)
		{
			std::cout << "吸血前hp:" << this->player->attributeComponent->hp << "\n";
			std::cout << "吸血量:" << dmg * ((50 + this->player->attributeComponent->luck) / 100.f) << "\n";
			this->player->attributeComponent->gainHP(dmg * ((50 + this->player->attributeComponent->luck) / 100.f));
			std::cout << "吸血后hp:" << this->player->attributeComponent->hp << "\n";

			dmg *= 2;
			crap = "Strike!";
		}
		
		enemy->loseHP(dmg);
		//enemy->resetDamageTimer();
		this->tts->addTextTag(DEFAULT_TAG, enemy->getPosition().x, enemy->getPosition().y, dmg, crap, "");	
	}

	//Check for enemy damage
	if (enemy->getGlobalBounds().intersects(this->player->getGlobalBounds()) && this->player->getDamageTimer())
	{
		if (enemy->getEnemyType() == SPIDER)
		{
			//A到以后，尝试增加debuff
			player->poison->addDebuff(50);
		}
		if (enemy->getEnemyType() == SLIME)
		{
			Slime* s = dynamic_cast<Slime*>(enemy);
			//A到以后，尝试增加debuff
			player->retard->addDebuff(s->type);
		}

		int dmg = enemy->getAttributeComp()->damageMax - this->player->getAttributeComponent()->fangyu;
		if (dmg < 1)
			dmg = 1;
		this->player->loseHP(dmg);
		this->tts->addTextTag(NEGATIVE_TAG, player->getPosition().x - 30.f, player->getPosition().y, dmg, "-", "HP");
		this->audioManager->bgs->play("Hurt");
	}
}

void GameState::updatePlayerEvent()
{
	AttributeComponent* a = this->player->getAttributeComponent();
	//更新Player死亡、升级、胜利
	while (a->exp >= a->expNext)
	{
		++a->level;
		a->exp -= a->expNext;
		a->expNext = (50 * pow(a->level, 3) - 150 * pow(a->level, 2) + (UINT64)400 * a->level) / 12;

		a->reset = true;
		a->updateStats(true);
		//速度加快
		this->player->getMovementComponent()->maxVelocity = 140.f + a->minjie * 7.f;

		std::cout << "LevelUp\n";
		this->audioManager->bgs->play("LevelUp");

		state = LEVELUP;
		std::cout << "state为：" << state << "\n";
		this->levelupPause->shuffle();
	}

	//死亡
	if (a->isDead() && state == GAME)
	{
		state = GAMEOVER;
		std::cout << "state为：" << state << "\n";

		gameover = new GameOverPause(this->stateData->gfxSettings->resolution, this->font, false);

		audioManager->bgm->stop();
		audioManager->randomMusic("Gameover", 4);
		audioManager->setVolume(this->stateData->gfxSettings->volumelevel);
	}

	//胜利
	if (a->level >= 11 && (state == GAME || state == LEVELUP))
	{
		state = GAMEOVER;
		std::cout << "state为：" << state << "\n";

		gameover = new GameOverPause(this->stateData->gfxSettings->resolution, this->font, true);

		audioManager->bgm->stop();
		audioManager->randomMusic("Victory", 3);
		audioManager->setVolume(this->stateData->gfxSettings->volumelevel);
	}
}

#if test
void GameState::updateDebugText(const float& dt)
{
	std::stringstream ss;

	ss << "Mouse Pos View: " << this->mousePosView.x << " " << this->mousePosView.y << "\n"
		<< "Active Enemies: " << this->activeEnemies.size() << "\n";

	this->debugText.setString(ss.str());
}
#endif // test


void GameState::update(const float& dt)
{
	this->updateMousePositions(&this->view);
	this->updateKeytime(dt);
	this->updateInput(dt);

#if test
	this->updateDebugText(dt);
#endif // test

	
	if (state == GAME) //Unpaused update
	{
		this->updateView(dt);

		this->updatePlayerInput(dt);

		this->updateTileMap(dt);

		this->updatePlayer(dt);

		this->updatePlayerGUI(dt);

		//Update all enemies
		//CHANGE: Loop outside, and make functions take one enemy at a time
		this->updateCombatAndEnemies(dt);

		//Update systems
		this->tts->update(dt);

		//更新Player死亡、升级
		this->updatePlayerEvent();
	}
	else if(state == PAUSE)//Paused update
	{
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
	else if (state == LEVELUP)
	{
		this->levelupPause->update(this->mousePosWindow);
		this->updateLevelupPause();
	}
	else if (state == GAMEOVER)
	{
		this->gameover->update(this->mousePosWindow);
		this->updateGameover();
	}

	std::stringstream ss;
	ss << "DeBuff: \n";
	if (this->player->poison->times > 0)
	{
		ss << " - Poison : " << this->player->poison->times << " (times)\n";
	}
	if (this->player->retard->debuffTimerMax > 0)
	{
		int times = this->player->retard->debuffTimerMax - this->player->retard->debuffTimer.getElapsedTime().asMilliseconds();
		times /= 1000.f;
		ss << " - Retard : " << times << " (seconds)\n";
	}
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	this->renderTexture.clear();

	this->renderTexture.setView(this->view);

	this->tileMap->render(
		this->renderTexture, 
		this->viewGridPosition, 
		&this->core_shader,
		this->player->getCenter(),
		false
	);

	for (auto *enemy : this->activeEnemies)
	{
		enemy->render(this->renderTexture, &this->core_shader, this->player->getCenter());
	}

	this->player->render(this->renderTexture, &this->core_shader, this->player->getCenter());

	this->tileMap->renderDeferred(this->renderTexture, &this->core_shader, this->player->getCenter());

	this->tts->render(this->renderTexture);

	//Render GUI
	this->renderTexture.setView(this->renderTexture.getDefaultView());
	this->playerGUI->render(this->renderTexture);

	if (state == PAUSE) //Pause menu render
	{
		//this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pmenu->render(this->renderTexture);
	}
	else if (state == LEVELUP)
	{
		this->levelupPause->render(this->renderTexture);
	}
	else if (state == GAMEOVER)
	{
		this->gameover->render(this->renderTexture);
	}

#if test
	//Debug Text
	this->renderTexture.draw(this->debugText);
#endif // test

	//FINAL RENDER
	this->renderTexture.display();
	//this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}

