#pragma once
#include <SFML/Graphics.hpp>
#include "general.h"
#include "Animation.h" 

const float START_SIZE = 2.0f;
const unsigned short DRAWING = 14;

enum class Cell
{
	Empty,
	Wall
};

enum class Direction 
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Animation;

class Player 
{
public:
	Player(sf::Vector2f position)
	{
		angle = 0;
		 
		delta_position.x = cos(angle) * 3;
		delta_position.y = sin(angle) * 3;

		increase_jump_height = false;
		jump_height = 0;

		this->position = position;
		shape.setPosition(position.x, position.y);
		shape.setRadius(START_SIZE);
		shape.setFillColor(sf::Color::Red);

		centre.x = (SCREEN_WIDTH - CELL_SIZE * MAP_WIDTH) / 2 + CELL_SIZE * MAP_WIDTH;
		centre.y = SCREEN_HEIGHT / 2;

		wall_texture.loadFromFile(PATH_WALL);
		wall_shape.setTexture(wall_texture);

		cloud_texture.loadFromFile(PATH_CLOUDS);
		cloud_shape.setTexture(cloud_texture);

		gun_texture.loadFromFile(PATH_GUN);
		gun_sprite.setTexture(gun_texture);

		animation = Animation(&gun_texture, sf::Vector2u(6, 1), ANIMATION_SPEED);
		play_animation = false;
	}

	float distance_to(const sf::Vector2f& point) const;

	void draw(sf::RenderWindow& window);
	void move(const Direction direction, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map);
	void draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window);
	void draw_rays(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window);
	void set_direction(const sf::Vector2i &mouse);
	void set_direction_3d(const sf::Vector2i& mouse);
	void jump();
	void gun_update(const float delta_time, sf::RenderWindow& window);
	void shoot();
private:
	bool check_map_collision(const sf::Vector2f& curr_position, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map) const;
private:
	sf::Vector2f position;
	sf::Vector2f delta_position;
	sf::Vector2f centre;
	sf::CircleShape shape;

	sf::Sprite cloud_shape;
	sf::Texture cloud_texture;

	sf::Sprite wall_shape;
	sf::Texture wall_texture;

	sf::Sprite gun_sprite;
	sf::Texture gun_texture;

	Animation animation;

	float angle;
	float anfle_vertical;
	float jump_height;
	bool increase_jump_height;
	bool play_animation;
};
