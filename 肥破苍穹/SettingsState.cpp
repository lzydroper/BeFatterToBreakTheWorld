#include "stdafx.h"
#include "SettingsState.h"

void SettingsState::initVariables()
{
	this->modes = sf::VideoMode::getFullscreenModes();

	this->volumes = { 0,1,2,3,4,5 };
}

void SettingsState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void SettingsState::initKeybinds()
{
	std::ifstream ifs("Config/mainmenustate_keybinds.ini");

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

void SettingsState::initGui()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	//Background
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(vm.width),
			static_cast<float>(vm.height)
		)
	);

	if (!this->backgroundTexture.loadFromFile("Resources/Images/Backgrounds/bg2.png"))
	{
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}

	this->background.setTexture(&this->backgroundTexture);

	//Buttons
	this->buttons["BACK"] = new gui::Button(
		gui::p2pX(72.f, vm), gui::p2pY(81.5f, vm),
		gui::p2pX(13.f, vm), gui::p2pY(6.f, vm),
		&this->font, "Back", gui::calcCharSize(vm),
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 100, 100), sf::Color(150, 150, 150, 100), sf::Color(20, 20, 20, 100));

	this->buttons["APPLY"] = new gui::Button(
		gui::p2pX(60.f, vm), gui::p2pY(81.5f, vm),
		gui::p2pX(13.f, vm), gui::p2pY(6.f, vm),
		&this->font, "Apply", gui::calcCharSize(vm),
		sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 100, 100), sf::Color(150, 150, 150, 100), sf::Color(20, 20, 20, 100));

	//Modes
	std::vector<std::string> modes_str;
	for (auto it = modes.begin(); it != modes.end(); ++it)
	{
		modes_str.push_back(std::to_string((*it).width) + 'x' + std::to_string((*it).height));

		//找到匹配的序号
		if ((*it) == this->stateData->gfxSettings->resolution)
		{
			default_index = std::distance(modes.begin(), it);
		}
	}

	//Drop down lists
	this->dropDownLists["RESOLUTION"] = new gui::DropDownList(
		gui::p2pX(42.f, vm), gui::p2pY(42.f, vm),
		gui::p2pX(10.4f, vm), gui::p2pY(4.5f, vm),
		font, modes_str.data(), modes_str.size(), default_index
	);

	//音量
	std::vector<std::string> volume_str;
	for (auto it = volumes.begin(); it != volumes.end(); ++it)
	{
		volume_str.push_back(std::to_string((*it) * 20) + '%');

	}
	this->dropDownLists["VOLUME"] = new gui::DropDownList(
		gui::p2pX(42.f, vm), gui::p2pY(52.f, vm),
		gui::p2pX(10.4f, vm), gui::p2pY(4.5f, vm),
		font, volume_str.data(), volume_str.size(), this->stateData->gfxSettings->volumelevel
	);

	//Text init
	this->optionsText.setFont(this->font);
	this->optionsText.setPosition(sf::Vector2f(gui::p2pX(5.2f, vm), gui::p2pY(41.7f, vm)));
	this->optionsText.setCharacterSize(gui::calcCharSize(vm, 70));
	this->optionsText.setFillColor(sf::Color(255, 255, 255, 200));

	//修改设置显示
	this->optionsText.setString(
		"Resolution \n\nVolume"	//( \n\nFullscreen \n\nVsync \n\nAntialiasing \n\n )	无法设置中文
	);
}

void SettingsState::resetGui()
{
	/*
	 * Clears the GUI elements and re-initialises the GUI.
	 *
	 * @return void
	 */

	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	this->buttons.clear();

	auto it2 = this->dropDownLists.begin();
	for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
	{
		delete it2->second;
	}
	this->dropDownLists.clear();

	this->initGui();
}

SettingsState::SettingsState(StateData* state_data)
	: State(state_data)
{
	default_index = 0;

	this->initVariables();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
}

SettingsState::~SettingsState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	
	auto it2 = this->dropDownLists.begin();
	for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
	{
		delete it2->second;
	}
}

//Accessors

//Functions
void SettingsState::updateInput(const float & dt)
{

}

void SettingsState::updateGui(const float & dt)
{
	/*Updates all the gui elements in the state and handle their functionlaity.*/
	//Buttons
	for (auto &it : this->buttons)
	{
		it.second->update(this->mousePosWindow);
	}

	//Button functionality
	//Quit the game
	if (this->buttons["BACK"]->isPressed())
	{
		this->endState();
	}

	//Dropdown lists
	for (auto& it : this->dropDownLists)
	{
		it.second->update(this->mousePosWindow, dt);
	}

	//Apply selected settings
	if (this->buttons["APPLY"]->isPressed())
	{
		//TEST REMOVE LATER
		if (dropDownLists["RESOLUTION"]->isChanged)
		{
			this->stateData->gfxSettings->resolution = this->modes[this->dropDownLists["RESOLUTION"]->getActiveElementId()];
			dropDownLists["RESOLUTION"]->isChanged = false;

			//写入设置
			this->stateData->gfxSettings->saveToFile("Config/graphics.ini");

			//进行修改
			this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title, sf::Style::Default);
		}
		if (dropDownLists["VOLUME"]->isChanged)
		{
			this->stateData->gfxSettings->volumelevel = this->volumes[this->dropDownLists["VOLUME"]->getActiveElementId()];
			dropDownLists["VOLUME"]->isChanged = false;

			//写入设置
			this->stateData->gfxSettings->saveToFile("Config/graphics.ini");

			//进行修改
			this->audioManager->setVolume(this->stateData->gfxSettings->volumelevel);
		}
		

		this->resetGui();
	}

	
	//Dropdown lists functionality
}

void SettingsState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);

	this->updateGui(dt);
}

void SettingsState::renderGui(sf::RenderTarget& target)
{
	for (auto &it : this->buttons)
	{
		it.second->render(target);
	}

	for (auto &it : this->dropDownLists)
	{
		it.second->render(target);
	}
}

void SettingsState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);

	this->renderGui(*target);

	target->draw(this->optionsText);

#if test
	//REMOVE LATER!!!
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(12);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());
	target->draw(mouseText);
#endif // test

}