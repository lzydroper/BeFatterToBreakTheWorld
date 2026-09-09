#include "Slime.h"

void Slime::initVariables()
{
	enemyType = SLIME;

	this->attributeComponent->hp *= (type / 4.f) + 1;
	this->attributeComponent->hpMax *= (type / 4.f) + 1;
	this->attributeComponent->damageMax *= (type / 12.f) + 1;
}

void Slime::initAnimations()
{
	this->animationComponent->addAnimation("IDLE", 25.f, 0, 0, 2, 0, 60, 64);
	this->animationComponent->addAnimation("WALK_DOWN", 25.f, 0, 0, 2, 0, 60, 64);
	this->animationComponent->addAnimation("WALK_LEFT", 25.f, 0, 1, 2, 1, 60, 64);
	this->animationComponent->addAnimation("WALK_RIGHT", 25.f, 0, 2, 2, 2, 60, 64);
	this->animationComponent->addAnimation("WALK_UP", 25.f, 0, 3, 2, 3, 60, 64);
	//this->animationComponent->addAnimation("ATTACK", 5.f, 0, 2, 1, 2, 60, 64);
}

void Slime::initAI()
{

}

void Slime::initGUI()
{
	this->hpBar.setFillColor(sf::Color::Red);
	this->hpBar.setSize(sf::Vector2f(60.f, 10.f));
	this->hpBar.setPosition(this->sprite.getPosition());
}

Slime::Slime(float x, float y, sf::Texture& texture_sheet, EnemySpawnerTile& enemy_spawner_tile, Entity& player)
	: Enemy(enemy_spawner_tile)
{
	type = rand() % 4 + 1;
	this->sprite.setScale(type, type);

	this->createHitboxComponent(this->sprite, 18.f, 45.f, 18.f, 22.f);
	float maxvelocity = 30.f + 30.f * player.attributeComponent->level * (type / 8.f) + rand() % 30;
	this->createMovementComponent(maxvelocity, 1500.f, 1200.f);
	this->createAnimationComponent(texture_sheet);
	this->createAttributeComponent(player.attributeComponent->level);

	this->generateAttributes(this->attributeComponent->level);

	this->setPosition(x, y);
	this->initAnimations();

	this->initVariables();
	this->initGUI();

	this->follow = new AIFollow(*this, player);
}

Slime::~Slime()
{
	delete this->follow;
}

void Slime::updateAnimation(const float& dt)
{
	if (this->movementComponent->getState(IDLE))
	{
		this->animationComponent->play("IDLE", dt);
	}
	else if (this->movementComponent->getState(MOVING_LEFT))
	{
		this->animationComponent->play("WALK_LEFT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_RIGHT))
	{
		this->animationComponent->play("WALK_RIGHT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_UP))
	{
		this->animationComponent->play("WALK_UP", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_DOWN))
	{
		this->animationComponent->play("WALK_DOWN", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	}

}

void Slime::update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view)
{
	Enemy::update(dt, mouse_pos_view, view);

	this->movementComponent->update(dt);

	this->hpBar.setSize(sf::Vector2f(30.f * type * (static_cast<float>(this->attributeComponent->hp) / this->attributeComponent->hpMax), 10.f));
	this->hpBar.setPosition(this->hitboxComponent->getPosition().x + this->hitboxComponent->getGlobalBounds().width / 2 - hpBar.getSize().x / 2, this->hitboxComponent->getPosition().y - this->hitboxComponent->getGlobalBounds().height / 2 - hpBar.getSize().y);

	//this->updateAttack();

	this->updateAnimation(dt);

	this->hitboxComponent->update();

	this->follow->update(dt);
}

void Slime::render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox)
{
	if (shader)
	{
		shader->setUniform("hasTexture", true);
		shader->setUniform("lightPos", light_position);
		target.draw(this->sprite, shader);
	}
	else
	{
		target.draw(this->sprite);
	}

	target.draw(this->hpBar);

	if (show_hitbox)
		this->hitboxComponent->render(target);
}
