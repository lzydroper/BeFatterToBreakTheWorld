#include "Background.h"

void Background::init_varible()
{
	//init texture
	if (!texture.loadFromFile("Image/Background.png"))
		std::cout << "ERROR::BACKGROUND::Failed to load file Background.png \n";

	//init background
	background.setTexture(texture);
	background.setScale(5.1, 2.2);
	//理论活动范围：-300~1500
	//需要考虑图的大小
	background.setPosition(900 - background.getGlobalBounds().width / 2, 0);
}

Background::Background()
{
	init_varible();
}

Background::~Background()
{
}

void Background::update(sf::RenderTarget& target)
{
}

void Background::render(sf::RenderTarget& target)
{
	target.draw(background);
}
