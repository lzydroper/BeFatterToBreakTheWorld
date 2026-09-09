#include "Game.h"

void Game::init_window()
{
	videomode = sf::VideoMode(800, 600);
	window.create(videomode, "Game Test", sf::Style::Titlebar | sf::Style::Close);
	window.setTitle(L"肥破苍穹");
	window.setFramerateLimit(60);
	//icon
	icon.loadFromFile("Image/icon.png");
	texture_icon.loadFromImage(icon);
	sprite_icon.setTexture(texture_icon);
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	//view
	view = sf::View(sf::Vector2f(player.getPosition().x,300), sf::Vector2f(window.getSize()));
}

void Game::init_variable()
{
	//gamelogic
	is_gameover = false;
	edge_bottom = 600.f;
}

void Game::init_font()
{
	if (!this->font.loadFromFile("Font/consola.ttf"))
	{
		std::cout << "ERROR::GAME::INITFONT::Failed to load font !\n";
	}
}

bool Game::is_to_the_edge() const
{
	//Left
	if (player.getGlobalBounds().left <= 0.f)
	{
		return true;
	}
	else if (player.getGlobalBounds().left + player.getGlobalBounds().width >= 1200.f)
	{
		return true;
	}
	return false;
}

Game::Game()
{
	//init function
	init_window();
	init_variable();
	init_font();
}

Game::~Game()
{
}

const bool Game::running() const
{
	return window.isOpen();
}

const bool Game::gameover() const
{
	return is_gameover;
}

void Game::update()
{
	update_poll_event();

	update_player();

	update_collision();
}

void Game::update_poll_event()
{
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:								//点X关闭
			window.close();
			std::cout << "click X to close the game \n";
			break;
		case sf::Event::KeyPressed:							//按下Esc关闭
			if (event.key.code == sf::Keyboard::Escape)
			{
				window.close();
				std::cout << "press Esc to close the game \n";
			}
			break;
		case sf::Event::KeyReleased:
			if (
				event.key.code == sf::Keyboard::Left	||
				event.key.code == sf::Keyboard::Right	||
				event.key.code == sf::Keyboard::Up		||
				event.key.code == sf::Keyboard::Down)
				player.reset_animation_timer();
			break;
		default:
			break;
		}
	}
}

void Game::update_player()
{
	//update the player
	player.update(window);
}

void Game::update_collision()
{
	//collision of edge of bottom
	if (player.getGlobalBounds().top + player.getGlobalBounds().height >= edge_bottom)
	{
		//碰撞后速度归零，将y坐标停留在底部
		player.reset_velocity_y();
		player.set_position(player.getPosition().x, edge_bottom - player.getGlobalBounds().height);
	}
}

void Game::render()
{
	/*
		@ return void

		 - render basic window
	*/

	window.clear(sf::Color::White);

	//锁定视角在player上
	//在边界以前锁定视角，出边界不锁定
	if (!is_to_the_edge())
	{
		view.setCenter(player.getPosition().x + player.getGlobalBounds().width/2, 300);
		window.setView(view);
	}

	//render stuffs
	backgroud.render(window);
	player.render(window);

	window.display();
}
