#include "GameOverPause.h"
#include "stdafx.h"

GameOverPause::GameOverPause(sf::VideoMode& vm, sf::Font& font, bool isWin)
	:vm(vm), font(font)
{
	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));

	if (!isWin)
	{
		if (!backgroundTexture.loadFromFile("Resources/Images/Backgrounds/bg_dead.jfif"))
			throw "ERROR::PauseMenu::FAILED_TO_LOAD_BACKGROUND_TEXTURE::DEAD";
	}
	else
	{
		if (!backgroundTexture.loadFromFile("Resources/Images/Backgrounds/bg_win.jpg"))
			throw "ERROR::PauseMenu::FAILED_TO_LOAD_BACKGROUND_TEXTURE::WIN";
	}

	this->background.setTexture(&backgroundTexture);

	this->addButton("QUIT", gui::p2pY(74.f, vm), gui::p2pX(13.f, vm), gui::p2pY(6.f, vm), gui::calcCharSize(vm), "Quit");

}

GameOverPause::~GameOverPause()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

std::map<std::string, gui::Button*>& GameOverPause::getButtons()
{
	return this->buttons;
}

//Functions
const bool GameOverPause::isButtonPressed(const std::string key)
{
	return this->buttons[key]->isPressed();
}

void GameOverPause::addButton(
	const std::string key,
	const float y,
	const float width,
	const float height,
	const unsigned char_size,
	const std::string text)
{
	float x = static_cast<float>(vm.width) / 2.f - width / 2.f;

	this->buttons[key] = new gui::Button(
		x, y, width, height,
		&this->font, text, char_size,
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 100), sf::Color(150, 150, 150, 150), sf::Color(20, 20, 20, 50)
	);
}

void GameOverPause::update(const sf::Vector2i& mousePosWindow)
{
	for (auto& i : this->buttons)
	{
		i.second->update(mousePosWindow);
	}
}

void GameOverPause::render(sf::RenderTarget& target)
{
	target.draw(this->background);

	for (auto& i : this->buttons)
	{
		i.second->render(target);
	}

}