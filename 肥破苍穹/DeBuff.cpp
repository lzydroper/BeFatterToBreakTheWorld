#include "DeBuff.h"

DeBuff::DeBuff()
{
	this->Trigger = 0.f;
	this->times = 0;
	this->debuffTimerMax = 1000;
}

DeBuff::~DeBuff()
{

}

void DeBuff::addDebuff(int addTimes)
{
	if ((rand() % 100) <= Trigger)
	{
		times += (rand() % addTimes) + 1;
		//std::cout << "加buff了！当前buff次数：" << times << "\n";
	}
}

const bool DeBuff::getDebuffTimerDone() const
{
	return this->debuffTimer.getElapsedTime().asMilliseconds() >= this->debuffTimerMax;
}

void DeBuff::resetDebuffTimer()
{
	this->debuffTimer.restart();
}
