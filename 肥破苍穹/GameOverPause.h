#ifndef GAMEOVERPAUSE_H
#define GAMEOVERPAUSE_H
//GameOverPause
#include"Gui.h"

class GameOverPause
{
private:
	sf::VideoMode& vm;
	sf::Font& font;

	sf::RectangleShape background;
	sf::Texture backgroundTexture;

	std::map<std::string, gui::Button*> buttons;

public:
	GameOverPause(sf::VideoMode& vm, sf::Font& font, bool isWin);
	virtual ~GameOverPause();

	//Accessor
	std::map<std::string, gui::Button*>& getButtons();

	//Functions
	const bool isButtonPressed(const std::string key);
	void addButton(const std::string key,
		const float y, const float width, const float height, const unsigned char_size, const std::string text);
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};

#endif