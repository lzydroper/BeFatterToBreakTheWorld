#pragma once
#include "head_shit.h"

/*
	3÷÷£¨sprite°¢renderstate°¢rendertexture
	Õµ¿¡”√sprite

*/

class Background
{
private:
	//variable
	sf::Sprite background;
	sf::Texture texture;


	//init function
	void init_varible();

public:
	Background();
	~Background();

	//Function
	void update(sf::RenderTarget& target);

	void render(sf::RenderTarget& target);
};

