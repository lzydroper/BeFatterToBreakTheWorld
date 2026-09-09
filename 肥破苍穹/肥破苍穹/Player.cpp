/*
	todo:
	角色行动：
		角色移动的物理效果（加速启动，减速停止）		已实现，但是效果不明显ORZ（原来是函数位置放错了）
		角色移动向左移动动画反转						已实现
		角色的跳跃									已实现
		角色的攻击
				设置攻击火焰sprite，由game判断flame的位置是否与enemy重合，重合造成伤害

	本来应该移动collision_area的，但是已经写完了，懒得改了
*/

#include "Player.h"

void Player::init_variable()
{
	move_speed = 7.5;
	sprite_scale = 2.5f;
	
	//game logic
	edge_left = -(300.f + 48.f);
	edge_right = 1500.f + 48.f;
	edge_bottom = 600.f;

	//physics
	velocity_max = move_speed;
	velocity_min = 1.f;
	acceleration = 1.5f;
	deceleration = 0.83f;
	acceleration_gravity = 3.f;
	velocity_max_y = 20.f;
	jump_speed = 100.f;
	is_jumping = false;

	//collision
	collision_area.setSize(sf::Vector2f(24,48));
	collision_area.setScale(sprite_scale, sprite_scale);
	collision_area.setFillColor(sf::Color::Transparent);
	collision_area.setOutlineColor(sf::Color::Red);
	collision_area.setOutlineThickness(1.f);

	//test
#if test
	player_pos_lt.setFillColor(sf::Color::Red);
	player_pos_rb.setFillColor(sf::Color::Red);
	player_pos_lt.setRadius(5.f);
	player_pos_rb.setRadius(5.f);
#endif // test
}

void Player::init_texture()
{
	if (!player_texture_sheet.loadFromFile("Image/player_sheet.png"))
		std::cout << "ERROR::PLAYER::Failed to load file player_sheet.png \n";

}

void Player::init_sprite(float x, float y)
{
	this->sprite.setPosition(x, y);
	this->sprite.setTexture(player_texture_sheet);
	this->sprite.setTextureRect(curent_frame);
	this->sprite.setScale(sf::Vector2f(sprite_scale, sprite_scale));
	
}

void Player::init_animation()
{
	animation_state = IDLE;
	animation_frame_width = 64;
	animation_frame_heigh = 80;
	curent_frame = sf::IntRect(0, 0, animation_frame_width, animation_frame_heigh);
	animation_timer.restart();
	animation_switch = true;
}

bool Player::is_to_the_falling_ground()
{
	//先不考虑跳台
	if (sprite.getGlobalBounds().top + sprite.getGlobalBounds().height < edge_bottom - 50.f)
	{
		return false;
	}
	return true;
}

Player::Player(float x, float y)
{
	//init function
	init_variable();
	init_texture();
	init_animation();
	init_sprite(x, y);
}

Player::~Player()
{
}

const bool& Player::getAnimationSwitch()
{
	if (animation_switch)
		animation_switch = false;
	return animation_switch;
}

const sf::Vector2f Player::getPosition() const
{
	return this->sprite.getPosition();
}

const sf::FloatRect Player::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

void Player::reset_velocity_y()
{
	velocity.y = 0.f;
}

void Player::set_position(const float x, const float y)
{
	sprite.setPosition(x, y);
}

void Player::update(sf::RenderTarget& target)
{
	/*
		@ return void

		 - move the player
		 - bounds the window
	*/

	update_move();

	update_window_bounds_collision(target);

	update_animation();

	update_physics();
}

void Player::update_window_bounds_collision(sf::RenderTarget& target)
{
	
	if (this->sprite.getGlobalBounds().left <= edge_left)
	{
		this->sprite.setPosition(edge_left, this->sprite.getPosition().y);
	}
	else if (this->sprite.getGlobalBounds().left + this->sprite.getGlobalBounds().width >= edge_right)
	{
		this->sprite.setPosition(edge_right - this->sprite.getGlobalBounds().width, this->sprite.getPosition().y);
	}
}

void Player::update_move()
{
	//Keyboard input
	//Left & Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		//std::cout << "Moving Left\n";
		function_move(-move_speed, 0);
		sprite.setScale(sf::Vector2f(-sprite_scale, sprite_scale));
		sprite.setOrigin(sf::Vector2f(sprite.getGlobalBounds().width / sprite_scale, 0.f));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		function_move(move_speed, 0);
		sprite.setScale(sf::Vector2f(sprite_scale, sprite_scale));
		sprite.setOrigin(sf::Vector2f(0.f, 0.f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && is_jumping == false)
	{
		is_jumping = true;
		animation_state = JUMPING;
		jumping_state = START;
		//先播放起跳动画
		curent_frame.left = 0;
		animation_timer.restart();
	}

	if (velocity.x < 0.f && is_jumping == false)
		animation_state = MOVING_LEFT;
	else if (velocity.x > 0.f && is_jumping == false)
		animation_state = MOVING_RIGHT;
	else if (is_jumping == false)
		animation_state = IDLE;
}

void Player::update_animation()
{
	if (animation_state == IDLE)
	{
		if (animation_timer.getElapsedTime().asSeconds() >= 0.2f || getAnimationSwitch())
		{
			//std::cout << "IDLE\n";
			curent_frame.top = animation_frame_heigh * 0;
			curent_frame.left += animation_frame_width;
			if (curent_frame.left >= animation_frame_width * 4)
			{
				curent_frame.left = 0;
			}

			animation_timer.restart();
			this->sprite.setTextureRect(curent_frame);
		}
	}
	else if (animation_state == MOVING_RIGHT)
	{
		if (animation_timer.getElapsedTime().asSeconds() >= 0.1f || getAnimationSwitch())
		{
			curent_frame.top = animation_frame_heigh * 1;
			curent_frame.left += animation_frame_width;
			if (curent_frame.left >= animation_frame_width * 8)
			{
				curent_frame.left = 0;
			}

			animation_timer.restart();
			this->sprite.setTextureRect(curent_frame);
		}
	}
	else if (animation_state == MOVING_LEFT)
	{
		if (animation_timer.getElapsedTime().asSeconds() >= 0.1f || getAnimationSwitch())
		{
			curent_frame.top = animation_frame_heigh * 1;
			curent_frame.left += animation_frame_width;
			if (curent_frame.left >= animation_frame_width * 8)
			{
				curent_frame.left = 0;
			}

			animation_timer.restart();
			this->sprite.setTextureRect(curent_frame);
		}
	}
	else if (animation_state == JUMPING)
	{
		//先播放起跳动画
		if (jumping_state == START)
		{
			if (animation_timer.getElapsedTime().asSeconds() >= 0.05f || getAnimationSwitch())
			{
				curent_frame.top = animation_frame_heigh * 2;

				//从0开始
				animation_timer.restart();
				this->sprite.setTextureRect(curent_frame);

				curent_frame.left += animation_frame_width;
				//播放到第4帧，给向上速度，起跳
				if (curent_frame.left >= animation_frame_width * 4)
				{
					curent_frame.left = 0.f;
					animation_timer.restart();
					jumping_state = JUMP_AIR;
					velocity.y = -jump_speed;
				}
			}
			/*curent_frame.top = animation_frame_heigh * 2;
			if (curent_frame.left >= animation_frame_width * 4)
			{
				curent_frame.left = 0.f;
				jumping_state = JUMP_AIR;
			}
			if (animation_timer.getElapsedTime().asSeconds() >= 0.1f && curent_frame.left != 0.f)
			{
				curent_frame.left += animation_frame_width;
				std::cout << curent_frame.top << " " << curent_frame.left << " JUMP_START\n";
			}
			animation_timer.restart();
			this->sprite.setTextureRect(curent_frame);*/
		}
		//之后循环向上中动画
		else if (velocity.y < 0.f)
		{
			//y方向速度向上，说明正在向上跳
			if (animation_timer.getElapsedTime().asSeconds() >= 0.1f || getAnimationSwitch())
			{
				curent_frame.top = animation_frame_heigh * 3;
				if (curent_frame.left >= animation_frame_width * 4)
				{
					curent_frame.left = 0;
				}

				animation_timer.restart();
				this->sprite.setTextureRect(curent_frame);
				curent_frame.left += animation_frame_width;
			}
		}
		else
		{
			curent_frame.left = 0.f;
			animation_timer.restart();
			animation_state = FALLING;
			jumping_state = FALL_AIR;
		}
	}
	else if (animation_state == FALLING && jumping_state == FALL_AIR)
	{
		if (!is_to_the_falling_ground())
		{
			//若未落地，播放FALL_AIR动画
			if (animation_timer.getElapsedTime().asSeconds() >= 0.2f || getAnimationSwitch())
			{
				curent_frame.top = animation_frame_heigh * 4;
				if (curent_frame.left >= animation_frame_width * 4)
				{
					curent_frame.left = 0;
				}

				animation_timer.restart();
				this->sprite.setTextureRect(curent_frame);
				curent_frame.left += animation_frame_width;
			}
		}
		else
		{
			//落地，播放END动画
			curent_frame.left = 0.f;
			animation_timer.restart();
			jumping_state = END;
			/*jumping_state = END;
			curent_frame.top = animation_frame_heigh * 5;
			curent_frame.left = 0;
			animation_timer.restart();
			//失败，没考虑render
			while (curent_frame.left != animation_frame_width * 3)
			{
				if (animation_timer.getElapsedTime().asSeconds() >= 0.1f)
				{
					curent_frame.left += animation_frame_width;
					this->sprite.setTextureRect(curent_frame);
				}
			}
			is_jumping = false;
			animation_state = IDLE;
			animation_timer.restart();*/
		}
	}
	else if (animation_state == FALLING && jumping_state == END)
	{
		if (animation_timer.getElapsedTime().asSeconds() >= 0.01f || getAnimationSwitch())
		{
			curent_frame.top = animation_frame_heigh * 5;

			//从0开始
			animation_timer.restart();
			this->sprite.setTextureRect(curent_frame);

			curent_frame.left += animation_frame_width;
			//播放到第3帧，结束跳跃
			if (curent_frame.left >= animation_frame_width * 3)
			{
				curent_frame.left = 0.f;
				animation_timer.restart();
				animation_state = IDLE;
				is_jumping = false;
			}
		}

	}
	else
	{
		animation_timer.restart();
	}
}

void Player::update_physics()
{
	//gravity
	velocity.y += acceleration_gravity;
	if (velocity.y > velocity_max_y)
		velocity.y = velocity_max_y;

	//deceleration
	velocity *= deceleration;

	//limit deceleration
	//速度到头后停止移动，以达到减速效果
	if (std::fabs(velocity.x) < velocity_min)
		velocity.x = 0.f;
	if (std::fabs(velocity.y) < velocity_min)
		velocity.y = 0.f;

	//move the sprite
	sprite.move(velocity);
	//std::cout << sprite.getPosition().y + sprite.getGlobalBounds().height << std::endl;
}

void Player::function_move(const float direction_x, const float direction_y)
{
	//acceleration
	velocity.x += direction_x * acceleration;

	//limit acceleration
	if (std::fabs(velocity.x) > velocity_max)
		velocity.x = (velocity.x > 0) ? velocity_max : -velocity_max;

}

void Player::reset_animation_timer()
{
	animation_timer.restart();
	animation_switch = true;
}

void Player::render(sf::RenderTarget& target)
{
	
	//target.draw(shape);
	target.draw(sprite);

	//test
	collision_area.setPosition(this->sprite.getPosition().x + 21 * sprite_scale, this->sprite.getPosition().y + 16 * sprite_scale);
	target.draw(collision_area);

#if test
	player_pos_lt.setPosition(collision_area.getPosition());
	player_pos_rb.setPosition(collision_area.getPosition().x + collision_area.getGlobalBounds().width, collision_area.getPosition().y + collision_area.getGlobalBounds().height);
	target.draw(player_pos_lt);
	target.draw(player_pos_rb);

	std::cout << player_pos_lt.getPosition().x << "," << player_pos_rb.getPosition().y << std::endl;
#endif // test

}
