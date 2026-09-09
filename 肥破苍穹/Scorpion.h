#pragma once
#include "stdafx.h"
#include "Enemy.h"
/*
	特点，远离主角时可以隐身，速度很慢
*/
class Scorpion:public Enemy
{
private:
	Entity* player;
	void initVariables();
	void initAnimations();
	void initAI();
	void initGUI();

	sf::RectangleShape hpBar;

	AIFollow* follow;

public:
	Scorpion(float x, float y, sf::Texture& texture_sheet, EnemySpawnerTile& enemy_spawner_tile, Entity& player);
	virtual ~Scorpion();

	//Functions
	void updateAnimation(const float& dt);
	void update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view);

	void render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox);
};

