#pragma once
#include "stdafx.h"

class DeBuff
{
protected:
	float Trigger;	//´¥·¢¸ÅÂÊ
public:
	sf::Clock debuffTimer;
	sf::Int32 debuffTimerMax;
	int times;
	DeBuff();
	virtual ~DeBuff();

	virtual void addDebuff(int addTimes = 3);

	const bool getDebuffTimerDone() const;
	void resetDebuffTimer();

	virtual void update() = 0;
};

