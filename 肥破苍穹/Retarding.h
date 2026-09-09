#pragma once
#include "DeBuff.h"
/*
	¼õËÙbuff
*/
class Retarding:public DeBuff
{
public:
	sf::Int32 DebuffTime;
	sf::Int32 DebuffDisplayTime;
	sf::Clock DebuffDisplayTimer;
	Retarding();
	~Retarding();

	const bool getDebuffDisplayTimerDone() const;
	void resetDebuffDisplayTimer();
	void addDebuff(int type);

	void update();
};

