#pragma once
#include "Gui.h"
/*
	升级后进入该界面，有一个随机数界面，返回一数组，另一边会选取前三个作为目标
	每次升级调用打乱函数
*/
class LevelUpPause
{
private:
	std::vector<int>& levelup_chioce;

	sf::VideoMode& vm;
	sf::Font& font;

	sf::RectangleShape background;
	sf::RectangleShape container[3];
	sf::Texture option[6];
	gui::Button* buttons[6];
public:
	int selected;
	LevelUpPause(sf::VideoMode& vm, sf::Font& font, std::vector<int>& levelup_chioce);
	~LevelUpPause();

	//Functions
	void shuffle();
	const bool isButtonPressed();
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};

