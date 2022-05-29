#pragma once
#include <SFML/Graphics.hpp>

class Animation
{
public:
	sf::IntRect rectangle;

	Animation(sf::Texture* texture, sf::Vector2u totalImage, float speed);
	Animation();

	bool update(size_t row, const float deltaTime);
private:
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;

	float totalTime;
	float switchTime;
};
