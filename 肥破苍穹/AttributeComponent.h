#pragma once
class AttributeComponent
{
public:
	//Leveling
	int level;
	int exp;
	int expNext;

	//Attributes
	int liliang;
	int minjie;
	int fangyu;
	int luck;
	float length;
	float length_basic;
	//AttributesLevelUpPoint
	int point_l;
	int point_m;
	int point_f;
	int point_x;
	int point_h;
	float point_s;
	
	//Stats
	int hp;
	int hpMax;
	int damageMin;
	int damageMax;
	bool reset;

	//Con / Des
	AttributeComponent(int level);
	virtual ~AttributeComponent();

	//Functions
	std::string debugPrint() const;

	void loseHP(const int hp);
	void gainHP(const int hp);
	void loseEXP(const int exp);
	void gainExp(const int exp);

	const bool isDead() const;

	void updateStats(bool isPlayer = false);
	void updateLevel();

	void update();
};

