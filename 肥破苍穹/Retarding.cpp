#include "Retarding.h"

Retarding::Retarding()
{
	this->Trigger = 15.f;
	this->debuffTimerMax = 0;
	this->DebuffTime = 5000;
	this->DebuffDisplayTime = 500;
	times = 0;
}

Retarding::~Retarding()
{
}

const bool Retarding::getDebuffDisplayTimerDone() const
{
	return this->DebuffDisplayTimer.getElapsedTime().asMilliseconds() >= this->DebuffDisplayTime;
}

void Retarding::resetDebuffDisplayTimer()
{
	this->DebuffDisplayTimer.restart();
}

void Retarding::addDebuff(int type)
{
	if ((rand() % 100) <= (Trigger * type))
	{
		this->debuffTimerMax += DebuffTime;
	}
}

void Retarding::update()
{
}
