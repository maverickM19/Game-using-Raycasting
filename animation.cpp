#include "Animation.h" 

Animation::Animation(sf::Texture* texture, sf::Vector2u totalImage, float speed) : switchTime(speed), imageCount(totalImage), totalTime(0.0f)
{
	currentImage.x = 0;
	rectangle.width = texture->getSize().x / (totalImage.x);
	rectangle.height = texture->getSize().y / totalImage.y;
}

Animation::Animation()
{
}

bool Animation::update(size_t row, const float deltaTime)
{
	currentImage.y = row;
	totalTime += deltaTime;

	if (totalTime >= switchTime)
	{
		totalTime -= switchTime;
		currentImage.x++;
	}

	if (currentImage.x == imageCount.x)
	{
		currentImage.x = 0;
		return true;
	}

	rectangle.left = currentImage.x * rectangle.width;
	rectangle.top = currentImage.y * rectangle.height;

	return false;
}
