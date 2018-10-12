#pragma once
#include <SFML/Graphics.hpp>
#include <random>
class Character
{
public:
	virtual void update() = 0;
	virtual void render(sf::RenderWindow & window) = 0;
	virtual void setTexture(sf::Texture& texture) = 0;
	virtual void clampPos() = 0;
	virtual void incRotation() = 0;
	virtual void decRotation() = 0;
	virtual void incSpeed() = 0;
	virtual void decSpeed() = 0;

	static float length(sf::Vector2f& v) { return sqrtf(v.x * v.x + v.y * v.y); }
	static sf::Vector2f normalise(sf::Vector2f v) { return length(v) > 0 ? v / length(v) : v; }
	static float distance(sf::Vector2f v1, sf::Vector2f v2) { return sqrtf(powf((v2.x - v1.x), 2) + powf((v2.y - v1.y), 2)); } //Distance between two points
	static int iRand(int min, int max) //Generate random integer between min and max
	{
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> dis(min, max);
		return dis(gen);
	};
};