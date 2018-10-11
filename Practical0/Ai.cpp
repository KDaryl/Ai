#include "Ai.h"
#include <iostream>

Ai::Ai(std::string name, sf::Vector2f pos, Player* player) :
	type(name),
	m_position(pos),
	m_player(player),
	m_velocity(0,0)
{
	if (type == "Wander") //Set our clock and start it
	{
		m_wanderClock = new sf::Clock();
		m_wanderClock->restart();
		m_wanderTarget = sf::Vector2f(3,3);
	}
}

Ai::~Ai()
{
}

void Ai::update()
{
	//Call specific updates depending on type of ai
	if (type == "Seek")
		seekUpdate();
	if (type == "Wander")
		wanderUpdate();
	if (type == "Flee")
		fleeUpdate();

	clampPos(); //Wrap our ai around

	m_sprite.setPosition(m_position); //Set our sprites position
	m_sprite.setRotation(m_rotation); //Set our sprites rotation
}

void Ai::seekUpdate(sf::Vector2f target)
{
	sf::Vector2f newTarget = target == sf::Vector2f(0, 0) ? m_player->pos() : target;
	float dt = (1 / 60.0f);
	sf::Vector2f desired = normalise(newTarget - m_position);
	desired = normalise(desired) * 75.0f;

	sf::Vector2f steering = desired - m_velocity;
	truncate(steering, 10.0f);

	m_velocity += steering * dt;
	truncate(m_velocity, 75.0f);

	m_position += m_velocity * dt;
	m_rotation = getOrientation(m_velocity);
}

void Ai::wanderUpdate()
{
	if (m_wanderClock->getElapsedTime().asSeconds() > 2) //Every 2 seconds
	{
		m_wanderClock->restart();
		m_velocity = sf::Vector2f(iRand(-40, 40),iRand(-40, 40));
	}
	m_wanderTarget = m_velocity;

	seekUpdate(m_wanderTarget);

	//if (m_wanderClock->getElapsedTime().asSeconds() > 2) //Every 2 seconds move to the right or left by 3 degrees in either direction
	//{
	//	m_wanderClock->restart();
	//	m_rotation += iRand(-40, 40);
	//}

	//sf::Vector2f direction = sf::Vector2f(cos(m_rotation * 0.0174533), sin(m_rotation * 0.0174533));
	//if (length(direction) > 0)
	//	direction = normalise(direction);

	//m_velocity = direction * 1.5f;

	//m_position += m_velocity;
}

void Ai::fleeUpdate()
{
	float dt = (1 / 60.0f);
	sf::Vector2f desired = normalise(m_position - m_player->pos());
	desired = normalise(desired) * 75.0f;

	sf::Vector2f steering = desired - m_velocity;
	truncate(steering, 10.0f);

	m_velocity += steering * dt;
	truncate(m_velocity, 75.0f);

	m_position += m_velocity * dt;
	m_rotation = getOrientation(m_velocity);
}

float Ai::getOrientation(sf::Vector2f vel)
{
	if (length(vel) > 0)
		return atan2(vel.y, vel.x) * 57.2958;
	return m_rotation;
}

void Ai::render(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void Ai::setTexture(sf::Texture & texture)
{
	m_sprite.setTexture(texture);
	m_sprite.setOrigin(sf::Vector2f(m_sprite.getGlobalBounds().width / 2.0, m_sprite.getGlobalBounds().height / 2.0));
	m_sprite.setPosition(m_position);
	m_sprite.setRotation(m_rotation);
}

void Ai::getTargetRotation(Kinematic & k)
{
}

void Ai::getTargetVelocity(Kinematic & k)
{
}

void Ai::getTargetAccel(Kinematic & k)
{
}

void Ai::truncate(sf::Vector2f & v, float max)
{
	float i = 0;

	i = max / length(v);

	i = i < 1.0 ? 1.0 : i;

	v *= i;
}

void Ai::clampPos()
{
	//Wrap character around screen
	if (m_position.x - (m_sprite.getLocalBounds().width / 2) > 1920)
		m_position.x = -(m_sprite.getLocalBounds().width / 2);
	if (m_position.x + (m_sprite.getLocalBounds().width / 2) < 0)
		m_position.x = 1920;
	//Checking y bounds
	if (m_position.y - (m_sprite.getLocalBounds().height / 2) > 1080)
		m_position.y = -(m_sprite.getLocalBounds().height / 2);
	if (m_position.y + (m_sprite.getLocalBounds().height / 2) < 0)
		m_position.y = 1080 + (m_sprite.getLocalBounds().height / 2);
}

void Ai::incRotation()
{
}

void Ai::decRotation()
{
}

void Ai::incSpeed()
{
}

void Ai::decSpeed()
{
}
