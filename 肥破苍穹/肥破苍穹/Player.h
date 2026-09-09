#pragma once
#include "head_shit.h"
#define test 0

enum player_animation_state { IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING, FALLING };
enum player_jumping_state { START = 0, JUMP_AIR, FALL_AIR, END};

class Player
{
private:
	//test
#if test
	sf::CircleShape player_pos_lt, player_pos_rb;
#endif // test

	//sprite
	sf::Sprite sprite;
	float sprite_scale;
	sf::RectangleShape collision_area;	//真实碰撞体积

	//variable
	double health;

	double move_speed;
	sf::Clock animation_timer;
	float jump_speed;
	bool is_jumping;

	//animation
	float animation_frame_width;
	float animation_frame_heigh;
	sf::IntRect curent_frame;
	player_animation_state animation_state;
	player_jumping_state jumping_state;
	bool animation_switch;

	//physics
	sf::Vector2f velocity;
	float velocity_max;
	float velocity_min;
	float acceleration;
	float deceleration;
	float acceleration_gravity;
	float velocity_max_y;

	//game logic
	float edge_left;
	float edge_right;
	float edge_bottom;

	//resource
	sf::Texture player_texture_sheet;
	sf::Texture flame_texture_sheet;

	//init function
	void init_variable();
	void init_texture();
	void init_sprite(float x, float y);
	void init_animation();

	//environment
	bool is_to_the_falling_ground();

public:
	Player(float x = 600, float y = 400);
	~Player();

	//accessor
	const bool& getAnimationSwitch();
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;

	//inline function（提高运行速度）
	inline const bool& getJumpState() const { return is_jumping; }
	inline void setJumpState(const bool &jump_sate) { is_jumping = jump_sate; }
	inline const sf::Vector2f get_collision_area_position() const { return collision_area.getPosition(); }
	inline const sf::FloatRect get_collision_area_globalbounds() const { return collision_area.getGlobalBounds(); }

	//modifier
	void reset_velocity_y();
	void set_position(const float x, const float y);

	//function
	void update(sf::RenderTarget& target);
	void update_window_bounds_collision(sf::RenderTarget& target);
	void update_move();
	void update_animation();
	void update_physics();
	void function_move(const float direction_x, const float direction_y);
	void reset_animation_timer();

	void render(sf::RenderTarget& target);
};

