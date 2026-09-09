#include "Game.h"

int main()
{
	//init random seed
	srand(time(0));

	//init game engine
	Game game;

	//game loop
	while (game.running() && !game.gameover())
	{
		//update
		game.update();

		//render
		game.render();
	}

	return 0;
}