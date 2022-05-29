#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <array>
#include "player.h"

std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> get_map()
{
	std::fstream is(PATH_MAP);

	std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> input_map = {};
	char value;

	for (size_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (size_t j = 0; j < MAP_WIDTH; j++)
		{
			is >> value;
			switch (value)
			{
			case '0':
				input_map[i][j] = Cell::Empty;
				break;
			case '1':
				input_map[i][j] = Cell::Wall;
				break;
			default:
				break;
			}
		}
	}

	return input_map;
}

int main() 
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Raycaster!");
	Player player(sf::Vector2f(100, 100));

	std::chrono::time_point<std::chrono::steady_clock> previous_time;
	previous_time = std::chrono::steady_clock::now();
	unsigned int lag = 0;

	std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> map{};

	map = get_map();

	sf::Clock delta_time_clock;
	float delta_time_gun;

	sf::RectangleShape ground, menu, scope;
	ground.setSize(sf::Vector2f((SCREEN_WIDTH) - MAP_CELL_SIZE * MAP_WIDTH, SCREEN_HEIGHT / 2));
	ground.setFillColor(sf::Color(0, 128, 0));
	ground.setPosition(sf::Vector2f(MAP_CELL_SIZE * MAP_WIDTH, SCREEN_HEIGHT / 2));

	menu.setSize(sf::Vector2f(MAP_CELL_SIZE * MAP_WIDTH, SCREEN_HEIGHT));
	menu.setFillColor(sf::Color(0, 0, 0));
	menu.setPosition(sf::Vector2f(0, 0));
	
	scope.setSize(sf::Vector2f(4, 4));
	scope.setFillColor(sf::Color(255, 0, 0));
	scope.setPosition(sf::Vector2f(((SCREEN_WIDTH) - MAP_CELL_SIZE * MAP_WIDTH) / 2 + MAP_CELL_SIZE * MAP_WIDTH, SCREEN_HEIGHT / 2));

	bool pause = false;
	window.setMouseCursorVisible(pause);
	while (window.isOpen())
	{
		sf::Event event;

		//window.setFramerateLimit(60);

		auto delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
		lag += delta_time;
		previous_time += std::chrono::microseconds(delta_time);

		while (FRAME_DURATION <= lag)
		{
			lag -= FRAME_DURATION;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (event.type == sf::Event::MouseMoved)
				{
					std::cout << "Method mouse: (" << sf::Mouse::getPosition(window).x << ", " << sf::Mouse::getPosition(window).y << ")\n\n";
					if (!pause)
					{
						player.set_direction_3d(sf::Mouse::getPosition(window));
						sf::Mouse::setPosition(sf::Vector2i(((SCREEN_WIDTH)-MAP_CELL_SIZE * MAP_WIDTH) / 2 + MAP_CELL_SIZE * MAP_WIDTH,
							SCREEN_HEIGHT / 2), window);
					}
				}

				if (event.type == sf::Event::KeyPressed)
				{
					if (!pause)
					{
						if (event.key.code == sf::Keyboard::Key::A)
						{
							player.move(Direction::LEFT, map);
						}
						if (event.key.code == sf::Keyboard::Key::D)
						{
							player.move(Direction::RIGHT, map);
						}
						if (event.key.code == sf::Keyboard::Key::W)
						{
							player.move(Direction::UP, map);
						}
						if (event.key.code == sf::Keyboard::Key::S)
						{
							player.move(Direction::DOWN, map);
						}
						if (event.key.code == sf::Keyboard::Key::Space)
						{
							player.jump();
						}
					}

					if (event.key.code == sf::Keyboard::Key::Escape)
					{
						pause = !pause;
						window.setMouseCursorVisible(pause);
					}
				}
				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.key.code == sf::Mouse::Left)
					{
						player.shoot();
					}
				}
			}

			if (FRAME_DURATION > lag)
			{
				window.clear(sf::Color(135, 206, 235));
				window.draw(menu);
				window.draw(ground);

				player.draw_map(map, window);
				player.draw(window);
				player.draw_rays(map, window);

				window.draw(scope);
				
				delta_time_gun = delta_time_clock.restart().asSeconds();
				player.gun_update(delta_time_gun, window);	//convert delta to seconds

				window.display();
			}
		}
	}

	return 0;
}
