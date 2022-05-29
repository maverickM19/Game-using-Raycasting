#include <array>
#include <cmath>
#include <iostream>
#include "player.h"

float convert_degrees_to_radians(const float degree)
{
	return (degree * ONE_DEGREE_TO_RADIAN);
}

float convert_radians_to_degrees(const float radian)
{
	return (radian * RADIAN);
}

void Player::draw(sf::RenderWindow& window)
{
	shape.setPosition(position.x - START_SIZE, position.y - START_SIZE);
	window.draw(shape);
}

void Player::draw_rays(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map, sf::RenderWindow& window)
{
	sf::Vector2f position_ray, next_position_ray, position_map, vertical_ray, vertical_map_position;
	sf::Color color_horizontal_ray(250, 0, 0, 190), color_vertical_ray(250, 0, 0, 210), color_of_ray;
	float angle_ray, tangens, final_distance, line_height, angle_distance, color_alpha = 200;
	float degree = convert_radians_to_degrees(angle) - 35;
	float texture_column;
	size_t brightness = 100;

	if (increase_jump_height)
	{
		jump_height += 10;
	}
	else if (!increase_jump_height && jump_height) 
	{
		jump_height -= 10;
	}

	if (jump_height > JUMP_HEIGHT)
	{
		increase_jump_height = false;
	}

	if (degree < 0)
	{
		degree += 360;
	}
	else if (degree > 360) 
	{
		degree -= 360;
	}

	for (size_t i = 0; i < 70 * 4; i++)
	{
		angle_ray = convert_degrees_to_radians(degree);

		tangens = (-1) / tan(angle_ray);

		if (angle_ray > PI)		//looking UP
		{
			position_ray.y = std::floor(position.y / MAP_CELL_SIZE) * MAP_CELL_SIZE;
			position_ray.x = (position.y - position_ray.y) * tangens + position.x;
			next_position_ray.y = -MAP_CELL_SIZE;
			next_position_ray.x = -next_position_ray.y * tangens;
		}
		else if (angle_ray < PI)	//looking DOWN
		{
			position_ray.y = std::ceil(position.y / MAP_CELL_SIZE) * MAP_CELL_SIZE;
			position_ray.x = (position.y - position_ray.y) * tangens + position.x;
			next_position_ray.y = MAP_CELL_SIZE;
			next_position_ray.x = -next_position_ray.y * tangens;
		}

		for (size_t i = 0; i < DRAWING; i++)
		{
			position_map.x = std::ceil(position_ray.x / MAP_CELL_SIZE) - 1;
			position_map.y = (angle_ray > PI) ? (position_ray.y / MAP_CELL_SIZE - 1) : (position_ray.y / MAP_CELL_SIZE);

			if (((position_map.x >= 0 && position_map.x < MAP_WIDTH) && (position_map.y >= 0 && position_map.y < MAP_HEIGHT))
				&& (map[position_map.y][position_map.x] == Cell::Wall))
			{
				break;
			}

			position_ray += next_position_ray;
		}

		vertical_map_position = position_map;
		vertical_ray = position_ray;	//save horisontal line

		tangens = -tan(angle_ray);

		if (angle_ray > PI / 2 && angle_ray < (3 * PI / 2)) // looking LEFT
		{
			position_ray.x = std::floor(position.x / MAP_CELL_SIZE) * MAP_CELL_SIZE;
			position_ray.y = (position.x - position_ray.x) * tangens + position.y;
			next_position_ray.x = -MAP_CELL_SIZE;
			next_position_ray.y = -next_position_ray.x * tangens;
		}
		else if (angle_ray < PI / 2 || angle_ray >(3 * PI / 2))	//looking RIGHT
		{
			position_ray.x = std::ceil(position.x / MAP_CELL_SIZE) * MAP_CELL_SIZE;
			position_ray.y = (position.x - position_ray.x) * tangens + position.y;
			next_position_ray.x = MAP_CELL_SIZE;
			next_position_ray.y = -next_position_ray.x * tangens;
		}

		for (size_t i = 0; i < DRAWING; i++)
		{
			position_map.y = std::ceil(position_ray.y / MAP_CELL_SIZE) - 1;
			position_map.x = (angle_ray < PI / 2 || angle_ray >(3 * PI / 2)) ? (position_ray.x / MAP_CELL_SIZE) : (position_ray.x / MAP_CELL_SIZE - 1);

			if (((position_map.x >= 0 && position_map.x < MAP_WIDTH) && (position_map.y >= 0 && position_map.y < MAP_HEIGHT))
				&& (map[position_map.y][position_map.x] == Cell::Wall))
			{
				break;
			}

			position_ray += next_position_ray;
		}
		
		if (distance_to(vertical_ray) < distance_to(position_ray))	//choose the shortest line and save it's distance
		{
			position_ray = vertical_ray;
			final_distance = distance_to(vertical_ray);
			brightness = 200;
			texture_column = (vertical_map_position.x + 1) * MAP_CELL_SIZE - vertical_ray.x;
		}
		else
		{
			texture_column = (position_map.y + 1) * MAP_CELL_SIZE - position_ray.y;
			final_distance = distance_to(position_ray);
			brightness = 150;
		}

		texture_column *= 4;
		if (texture_column > CELL_SIZE)
		{
			texture_column = CELL_SIZE;
		}

		//drawing 3D walls

		angle_distance = angle - angle_ray;
		if (angle_distance < 0)
		{
			angle_distance += 2 * PI;
		}
		else if (angle_distance > 2 * PI) 
		{
			angle_distance -= 2 * PI;
		}

		final_distance *= cos(angle_distance);

		line_height = (MAP_CELL_SIZE * SCREEN_HEIGHT) / final_distance;

		//draw sky
		cloud_shape.setPosition(4*i + MAP_CELL_SIZE * MAP_WIDTH, 0);
		cloud_shape.setColor(sf::Color(250, 250, 250));
		cloud_shape.setTextureRect(sf::IntRect(i, 0, 1, SCREEN_HEIGHT / 2 - line_height / 2));
		cloud_shape.setScale(4, 1);
		window.draw(cloud_shape);

		//draw walls
		wall_shape.setPosition(i * 4 + MAP_CELL_SIZE * MAP_WIDTH, SCREEN_HEIGHT / 2 - (line_height / 2 - jump_height));
		wall_shape.setColor(sf::Color(brightness, brightness, brightness));
		wall_shape.setTextureRect(sf::IntRect(round(texture_column) + 0, 0, 1, CELL_SIZE));
		wall_shape.setScale(4, (line_height) / CELL_SIZE);
		window.draw(wall_shape);

		if (i == 70 * 4 / 2)
		{
			sf::Vertex direction[] = { sf::Vertex(sf::Vector2f(position.x, position.y), sf::Color::White),
			sf::Vertex(sf::Vector2f(position_ray.x, position_ray.y), sf::Color::White) };

			window.draw(direction, 2, sf::Lines);
		}

		degree += 0.25;

		if (degree < 0)
		{
			degree += 360;
		}
		else if (degree > 360)
		{
			degree -= 360;
		}

		/*sf::VertexArray line(sf::TriangleStrip, 4);
		float percent = 0.07;
		line[0].position = sf::Vector2f(i * 4 + MAP_CELL_SIZE * MAP_WIDTH,	 (SCREEN_HEIGHT / 2 - line_height / 2));
		line[0].color = sf::Color(255, 0, 0);

		line[1].position = sf::Vector2f(i * 4 + 4 + MAP_CELL_SIZE * MAP_WIDTH, (SCREEN_HEIGHT / 2 - line_height / 2) + 4);
		line[1].color = sf::Color(255, 0, 0);

		line[2].position = sf::Vector2f(i * 4 + MAP_CELL_SIZE * MAP_WIDTH, (SCREEN_HEIGHT / 2 + line_height / 2));
		line[2].color = sf::Color(255, 0, 0);

		line[3].position = sf::Vector2f(i * 4 + 4 + MAP_CELL_SIZE * MAP_WIDTH, (SCREEN_HEIGHT / 2 + line_height / 2) + 4);
		line[3].color = sf::Color(255, 0, 0);
		window.draw(line);*/
	}
}

void Player::move(const Direction direction, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map)
{
	switch (direction)
	{
	case Direction::LEFT:
		if (check_map_collision(sf::Vector2f(position.x + cos(angle - PI / 2), position.y + sin(angle - PI / 2)), map))
		{
			position.x += cos(angle - PI / 2);
			position.y += sin(angle - PI / 2);
		}
		else if (check_map_collision(sf::Vector2f(position.x + cos(angle - PI / 2), position.y), map))
		{
			position.x += cos(angle - PI / 2);
		}
		else if (check_map_collision(sf::Vector2f(position.x, position.y + sin(angle - PI / 2)), map))
		{
			position.y += sin(angle - PI / 2);
		}

		break;
	case Direction::RIGHT:
		if (check_map_collision(sf::Vector2f(position.x + cos(angle + PI / 2), position.y + sin(angle + PI / 2)), map))
		{
			position.x += cos(angle + PI / 2);
			position.y += sin(angle + PI / 2);
		}
		else if (check_map_collision(sf::Vector2f(position.x + cos(angle + PI / 2), position.y), map))
		{
			position.x += cos(angle + PI / 2);
		}
		else if (check_map_collision(sf::Vector2f(position.x, position.y + sin(angle + PI / 2)), map))
		{
			position.y += sin(angle + PI / 2);
		}
      
		break;
	case Direction::UP:
		if (check_map_collision(sf::Vector2f(position + delta_position), map))
		{
			position.x += delta_position.x;
			position.y += delta_position.y;
		}
		else if (check_map_collision(sf::Vector2f(position.x + delta_position.x, position.y), map)) 
		{
			position.x += delta_position.x;
		}
		else if (check_map_collision(sf::Vector2f(position.x, position.y + delta_position.y), map))
		{	
			position.y += delta_position.y; 
		}

		break;
	case Direction::DOWN:
		if (check_map_collision(sf::Vector2f(position - delta_position), map))
		{
			position.x -= delta_position.x;
			position.y -= delta_position.y;
		}
		else if (check_map_collision(sf::Vector2f(position.x - delta_position.x, position.y), map))
		{
			position.x -= delta_position.x;
		}
		else if (check_map_collision(sf::Vector2f(position.x, position.y - delta_position.y), map))
		{
			position.y -= delta_position.y;
		}

		break;
	default:
		break;
	}
}

bool Player::check_map_collision(const sf::Vector2f &curr_position, const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT>& map) const
{
	sf::Vector2f map_position;

	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
		{
			map_position.x = (floor((curr_position.x - START_SIZE) / MAP_CELL_SIZE));
			map_position.y = (floor((curr_position.y - START_SIZE) / MAP_CELL_SIZE));

			break;
		}
		case 1:
		{
			map_position.x = (floor((curr_position.x - START_SIZE) / MAP_CELL_SIZE));
			map_position.y = (floor((curr_position.y + START_SIZE) / MAP_CELL_SIZE));

			break;
		}
		case 2:
		{
			map_position.x = (floor((curr_position.x + START_SIZE) / MAP_CELL_SIZE));
			map_position.y = (floor((curr_position.y - START_SIZE) / MAP_CELL_SIZE));

			break;
		}
		case 3:
		{
			map_position.x = (floor((curr_position.x + START_SIZE) / MAP_CELL_SIZE));
			map_position.y = (floor((curr_position.y + START_SIZE) / MAP_CELL_SIZE));
			break;
		}
		}

		if ((map[map_position.y][map_position.x] == Cell::Wall))
		{
			return false;
		}
	}

	return true;
}

void Player::jump() 
{
	if (!jump_height)
	{
		increase_jump_height = true;
	}
}

void Player::draw_map(const std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> &map, sf::RenderWindow& window)
{
	sf::Color dark_gray(105, 105, 105, 230);
	//TODO: put player in the center of the random cell

	for (size_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (size_t j = 0; j < MAP_WIDTH; j++)
		{
			sf::RectangleShape cell_shape;
			cell_shape.setPosition(sf::Vector2f(j * MAP_CELL_SIZE, i * MAP_CELL_SIZE));
			cell_shape.setSize(sf::Vector2f(MAP_CELL_SIZE - 1, MAP_CELL_SIZE - 1));

			switch (map[i][j])
			{
			case Cell::Empty:
				cell_shape.setFillColor(sf::Color::Black);
				window.draw(cell_shape);
				break;
			case Cell::Wall:
				cell_shape.setFillColor(dark_gray);
				window.draw(cell_shape);
				break;
			default:
				break;
			}
		}
	}
}

float Player::distance_to(const sf::Vector2f &point) const
{
	return(sqrt((position.x - point.x) * (position.x - point.x) + (position.y - point.y) * (position.y - point.y)));
}

void Player::set_direction(const sf::Vector2i& mouse)			//set angle direction on minimap : OFF
{
	angle = -atan2(mouse.x - position.x, mouse.y - position.y) + PI/2;

	delta_position.x = cos(angle) * 5;
	delta_position.y = sin(angle) * 5;
}

void Player::set_direction_3d(const sf::Vector2i& mouse)
{
	sf::Vector2f difference((SCREEN_WIDTH - MAP_CELL_SIZE * MAP_WIDTH) / 2 + (MAP_CELL_SIZE * MAP_WIDTH) - mouse.x,
		SCREEN_HEIGHT / 2 - mouse.y);

	difference.x /= (SCREEN_WIDTH - MAP_CELL_SIZE * MAP_WIDTH);
	difference.y /= SCREEN_HEIGHT;

	float rotation_angle = difference.x * 70;
	angle -= convert_degrees_to_radians(rotation_angle);

	if (angle < 0)
	{
		angle += 2 * PI;
	}
	else if (angle > 2 * PI)
	{
		angle -= 2 * PI;
	}

	delta_position.x = cos(angle) * 2;
	delta_position.y = sin(angle) * 2;
}

void Player::gun_update(const float delta_time, sf::RenderWindow& window) 
{
	if (play_animation)
	{
		if (animation.update(0, delta_time)) 
		{
			play_animation = false;
		}

		gun_sprite.setTextureRect(animation.rectangle);
		//values at X are custom to place gun's scope right at the middle
		gun_sprite.setPosition(((SCREEN_WIDTH)-MAP_CELL_SIZE * MAP_WIDTH) / 2 + 135, SCREEN_HEIGHT - 344);
		gun_sprite.setScale(4, 4);
		window.draw(gun_sprite);

		return;
	}

	gun_sprite.setTextureRect(sf::IntRect(0, 0, 70, 111));
	//values at X are custom to place gun's scope right at the middle
	gun_sprite.setPosition(((SCREEN_WIDTH)-MAP_CELL_SIZE * MAP_WIDTH) / 2 + 135, SCREEN_HEIGHT - 344);

	gun_sprite.setScale(4, 4);
	window.draw(gun_sprite);
}

void Player::shoot() 
{
	play_animation = true;
}
