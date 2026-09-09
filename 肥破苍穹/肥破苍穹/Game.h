#pragma once
#include "head_shit.h"
#include "Player.h"
#include "Background.h"

class Game
{
private:
	//window
	sf::VideoMode videomode;
	sf::RenderWindow window;
	sf::Image icon;
	sf::Texture texture_icon;
	sf::Sprite sprite_icon;
	sf::Event event;
	sf::View view;

	//resource
	sf::Font font;

	//character
	Player player;

	//background
	Background backgroud;

	//enemy


	//game logic
	bool is_gameover;
	float edge_bottom;

	//init func
	void init_window();
	void init_variable();
	void init_font();

	//environment
	bool is_to_the_edge() const;
public:
	//C & D
	Game();
	~Game();

	//accessor
	const bool running() const;
	const bool gameover() const;


	//modifier

	//function
	void update();
	void update_poll_event();
	void update_player();
	void update_collision();

	void render();

};