#include "stdafx.h"
#include "AttributeComponent.h"

AttributeComponent::AttributeComponent(int level)
{
	this->reset = true;
	this->level = level;
	this->exp = 0;
	this->expNext = (50 * pow(this->level, 3) - 150 * pow(this->level, 2) + (UINT64)400 * this->level) / 12;

	this->liliang = 1;		//ÉËº¦
	this->minjie = 1;		//ËÙ¶È
	this->fangyu = 1;		//ÊÜÉË
	this->luck = 1;			//±©»÷
	this->length = 0;		//½£µÄ³¤¶È
	this->length_basic = 0;

	point_l = 0;
	point_m = 0;
	point_f = 0;
	point_x = 0;
	point_h = 0;
	point_s = 0;

	this->updateLevel();
	this->updateStats();
}

AttributeComponent::~AttributeComponent()
{
	
}

//Functions
std::string AttributeComponent::debugPrint() const
{
	std::stringstream ss;

	ss	<< "±àºÅ£º0 , " << "µÈ¼¶: " << this->level << "\n"
		<< "±àºÅ£º1 , " << "ÉúÃü: " << this->hp << "\n"
		<< "±àºÅ£º2 , " << "Á¦Á¿: " << this->liliang << "\n"
		<< "±àºÅ£º3 , " << "Ãô½Ý: " << this->minjie << "\n"
		<< "±àºÅ£º4 , " << "·ÀÓù: " << this->fangyu << "\n"
		<< "±àºÅ£º5 , " << "ÐÒÔË: " << this->luck << "\n"
		<< "±àºÅ£º6 , " << "½£³¤: " << this->length << "£¨±¶Êý£©\n"
		<< "±àºÅ£º7 , " << "¾­Ñé: " << this->exp << "\n"
		<< "±àºÅ£º8 , " << "ÍË³öÐÞ¸Ä\n";

	return ss.str();
}

void AttributeComponent::loseHP(const int hp)
{
	this->hp -= hp;

	if (this->hp < 0)
		this->hp = 0;
}

void AttributeComponent::gainHP(const int hp)
{
	this->hp += hp;

	if (this->hp > this->hpMax)
		this->hp = this->hpMax;
}

void AttributeComponent::loseEXP(const int exp)
{
	this->exp -= exp;

	if (this->exp < 0)
		this->exp = 0;
}

void AttributeComponent::gainExp(const int exp)
{
	this->exp += exp;

	//this->updateLevel();
}

const bool AttributeComponent::isDead() const
{
	return this->hp <= 0;
}

void AttributeComponent::updateStats(bool isPlayer)
{
	if (!isPlayer)
	{
		hpMax = level * 20;
		damageMin = (level - 1) * 3 + 2;
		damageMax = (level - 1) * 5 + 5;
	}
	else
	{
		this->liliang	= level - 1 + point_l + 0;
		this->minjie	= level - 1 + point_m + 0;
		this->fangyu	= level - 1 + point_f + 2;
		this->luck		= level - 1 + point_x + 5;
		this->length	= length_basic + point_s;
		hpMax			= level * 25 + 25 + point_h;
		damageMin		= (level - 1) * 2 + liliang * 1.5 + 2;
		damageMax		= (level - 1) * 5 + liliang * 1.5 + 5;
	}

	if (reset)
	{
		this->hp = this->hpMax;
		reset = false;
	}
}

void AttributeComponent::updateLevel()
{
	while (this->exp >= this->expNext)
	{
		++this->level;
		this->exp -= this->expNext;
		this->expNext = (50 * pow(this->level, 3) - 150 * pow(this->level, 2) + (UINT64)400 * this->level) / 12;

	}
}

void AttributeComponent::update()
{
	this->updateLevel();
}



