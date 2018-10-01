#pragma once
#include "Character.h"
#include "Player.h"

class Ai : public Character
{
public:
	Ai(std::string name, sf::Vector2f pos, Player* player);
	~Ai();
	void update();
	void seekUpdate();
	void wanderUpdate();
	void fleeUpdate();
	float getOrientation(float orientation, sf::Vector2f vel);
	void render(sf::RenderWindow & window);
	void setTexture(sf::Texture& texture);
	void clampPos();
	void incRotation();
	void decRotation();
	void incSpeed();
	void decSpeed();
private:
	sf::Vector2f m_velocity, m_position;
	sf::Sprite m_sprite;
	float m_speed, m_rotation;
	std::string type; //The type of ai
	Player* m_player;
};

