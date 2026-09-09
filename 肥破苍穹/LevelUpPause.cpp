#include "LevelUpPause.h"

LevelUpPause::LevelUpPause(sf::VideoMode& vm, sf::Font& font, std::vector<int>& levelup_chioce)
	:vm(vm), font(font), levelup_chioce(levelup_chioce)
{
	selected = -1;
	//init button
	for (auto& i : buttons)
		i = new gui::Button(
			0, 0, gui::p2pX(13.f, vm), gui::p2pY(6.f, vm),
			&this->font, "Select", gui::calcCharSize(vm),
			sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
			sf::Color(70, 70, 70, 50), sf::Color(150, 150, 150, 50), sf::Color(20, 20, 20, 50)
		);

	//Init background
	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(vm.width),
			static_cast<float>(vm.height)
		)
	);
	this->background.setFillColor(sf::Color(20, 20, 20, 100));

	//Init container
	for (int i = 0; i < 3; i++)
	{
		container[i].setSize(
			sf::Vector2f(
				static_cast<float>(vm.width) / 4.f,
				static_cast<float>(vm.height) - gui::p2pY(9.f, vm)
			)
		);
		container[i].setFillColor(sf::Color::White);
		container[i].setPosition(
			static_cast<float>(vm.width) * 0.25f * (i + 1) - container[i].getSize().x / 2.f,
			30.f
		);
	}

	//Init Texture
	if (!option[0].loadFromFile("Resources/Images/LevelupOption/0.png"))
		throw "ERROR::PauseMenu::FAILED_TO_LOAD_TEXTURE::SHENGMING";
	if (!option[1].loadFromFile("Resources/Images/LevelupOption/1.png"))
		throw "ERROR::PauseMenu::FAILED_TO_LOAD_TEXTURE::LILIANG";
	if (!option[2].loadFromFile("Resources/Images/LevelupOption/2.png"))
		throw "ERROR::PauseMenu::FAILED_TO_LOAD_TEXTURE::MINJIE";
	if (!option[3].loadFromFile("Resources/Images/LevelupOption/3.png"))
		throw "ERROR::PauseMenu::FAILED_TO_LOAD_TEXTURE::FANGYU";
	if (!option[4].loadFromFile("Resources/Images/LevelupOption/4.png"))
		throw "ERROR::PauseMenu::FAILED_TO_LOAD_TEXTURE::XINGYUN";
	if (!option[5].loadFromFile("Resources/Images/LevelupOption/5.png"))
		throw "ERROR::PauseMenu::FAILED_TO_LOAD_TEXTURE::JIANCHANG";
}

LevelUpPause::~LevelUpPause()
{
	for (int i = 0; i < 6; i++)
		delete buttons[i];
}

void LevelUpPause::shuffle()
{
	//打乱数字
	std::mt19937 engine(time(NULL));
	std::shuffle(levelup_chioce.begin(), levelup_chioce.end(), engine);
	//根据生成的随机数，修改对应Button位置，修改texture
	float x;
	for (int i = 0, k = 0; i < 3; i++)
	{
		k = levelup_chioce[i];
		x = container[i].getPosition().x + container[i].getSize().x / 2.f - gui::p2pX(13.f, vm) / 2.f;
		buttons[k]->setPositioin(x, gui::p2pY(80.f, vm));
		container[i].setTexture(&option[k]);
	}
}

const bool LevelUpPause::isButtonPressed()
{
	for (int i = 0, k = 0; i < 3; i++)
	{
		k = levelup_chioce[i];
		if (buttons[k]->isPressed())
		{
			selected = k;
			return true;
		}
	}
	selected = -1;
	return false;
}

void LevelUpPause::update(const sf::Vector2i& mousePosWindow)
{
	for (int i = 0, k = 0; i < 3; i++)
	{
		k = levelup_chioce[i];
		buttons[k]->update(mousePosWindow);
	}
}

void LevelUpPause::render(sf::RenderTarget& target)
{
	target.draw(background);

	for (int i = 0, k = 0; i < 3; i++)
	{
		target.draw(container[i]);
		k = levelup_chioce[i];
		buttons[k]->render(target);
	}
}
