#pragma once
#include "DeBuff.h"
/*
	ÖÐ¶¾Ð§¹û
*/
class Poisoning:public DeBuff
{
public:
	int dmg;
	Poisoning();
	~Poisoning();

	void update();
};