#include "stdafx.h"
#include "HitboxComponent.h"

HitboxComponent::HitboxComponent(sf::Sprite& sprite, 
	float offset_x, float offset_y, 
	float width, float height)
	: sprite(sprite), offsetX(offset_x), offsetY(offset_y)
{
	this->nextPosition.left = 0.f;
	this->nextPosition.top = 0.f;
	this->nextPosition.width = width * this->sprite.getScale().x;
	this->nextPosition.height = height * this->sprite.getScale().y;

	this->hitbox.setPosition(this->sprite.getPosition().x + offset_x * this->sprite.getScale().x, this->sprite.getPosition().y + offset_y * this->sprite.getScale().y);
	this->hitbox.setSize(sf::Vector2f(width, height));
	this->hitbox.setScale(this->sprite.getScale());
	this->hitbox.setFillColor(sf::Color::Transparent);
	this->hitbox.setOutlineThickness(-1.f);
	this->hitbox.setOutlineColor(sf::Color::Green);

}

HitboxComponent::~HitboxComponent()
{

}

//Accessors
const sf::Vector2f & HitboxComponent::getPosition() const
{
	return this->hitbox.getPosition();
}

const sf::FloatRect HitboxComponent::getGlobalBounds() const
{
	return this->hitbox.getGlobalBounds();
}

const sf::FloatRect & HitboxComponent::getNextPosition(const sf::Vector2f & velocity)
{
	this->nextPosition.left = this->hitbox.getPosition().x + velocity.x;
	this->nextPosition.top = this->hitbox.getPosition().y + velocity.y;

	return this->nextPosition;
}

//Modifiers
void HitboxComponent::setPosition(const sf::Vector2f& position)
{
	this->hitbox.setPosition(position);
	sprite.setPosition(position.x - offsetX * this->sprite.getScale().x, position.y - offsetY * this->sprite.getScale().y);
}

void HitboxComponent::setPosition(const float x, const float y)
{
	setPosition(sf::Vector2f(x, y));
}

//Functions
bool HitboxComponent::intersects(const sf::FloatRect& frect)
{
	return this->hitbox.getGlobalBounds().intersects(frect);
}

void HitboxComponent::update()
{
	//hitbox¸úËæsprite
	hitbox.setPosition(sprite.getPosition().x + offsetX * this->sprite.getScale().x, sprite.getPosition().y + offsetY * this->sprite.getScale().y);
}

void HitboxComponent::render(sf::RenderTarget & target)
{
	target.draw(this->hitbox);
}
