#pragma once
#include "Character.h"
#include "Player.h"

struct Kinematic
{
	sf::Vector2f position;
	float orientation;
	sf::Vector2f velocity;
	float rotation;
};

class Ai : public Character
{
public:
	Ai(std::string name, sf::Vector2f pos, Player* player);
	~Ai();
	void update();
	void seekUpdate(sf::Vector2f target = sf::Vector2f(0, 0));
	void wanderUpdate();
	void fleeUpdate();
	void pursueUpdate();
	void arriveUpdate();

	float getOrientation(sf::Vector2f vel);
	void render(sf::RenderWindow & window);
	void setTexture(sf::Texture& texture);
	void setArriveSpeed(float nearSpeed);

	void truncate(sf::Vector2f& v, float max);

	void clampPos();
	void incRotation();
	void decRotation();
	void incSpeed();
	void decSpeed();
private:
	sf::RectangleShape m_box;
	sf::Text m_text;
	sf::Font m_font;

	sf::Vector2f m_velocity, m_position, m_target;
	sf::Sprite m_sprite;
	float m_speed, m_rotation;
	std::string type; //The type of ai
	Player* m_player;

	//Wander variables
	sf::Clock* m_wanderClock;

	//Arrive variables
	float m_arriveSpeed;
};

