#pragma once
#include <SFML/Graphics.hpp>
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
	static float angle(sf::Vector2f& a, sf::Vector2f& b) { return (a.x * b.x + a.y * b.y) / (length(a) * length(b)); }
	static sf::Vector2f normalise(sf::Vector2f v) { return v / length(v); }
};