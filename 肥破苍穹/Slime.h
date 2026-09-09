#pragma once
#include "stdafx.h"
#include "Enemy.h"
/*
	能上减速Buff，拥有随机体型及对应的血量
*/
enum SlimeType { SMALL = 1, MIDDLE, LARGE, SUPERHUGE };
class Slime :public Enemy
{
private:
	void initVariables();
	void initAnimations();
	void initAI();
	void initGUI();

	sf::RectangleShape hpBar;

	AIFollow* follow;

public:
	short type;
	Slime(float x, float y, sf::Texture& texture_sheet, EnemySpawnerTile& enemy_spawner_tile, Entity& player);
	virtual ~Slime();

	//Functions
	void updateAnimation(const float& dt);
	void update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view);

	void render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox);
};