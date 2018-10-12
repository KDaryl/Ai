#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(sf::Vector2f pos, sf::Vector2f vel);
	~Player();
	void update();
	void render(sf::RenderWindow & window);
	void setTexture(sf::Texture& texture);
	void clampPos();
	void incRotation();
	void decRotation();
	void incSpeed();
	void decSpeed();

	//Getters
	sf::Vector2f& pos() { return m_position; }
	sf::Vector2f& vel() { return m_velocity; }
private:
	sf::Vector2f m_velocity, m_position;
	sf::Sprite m_sprite;
	float m_speed, m_rotation;
};

