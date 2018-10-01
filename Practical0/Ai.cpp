#include "Ai.h"
#include <iostream>

Ai::Ai(std::string name, sf::Vector2f pos, Player* player) :
	type(name),
	m_position(pos),
	m_player(player)
{
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
}

void Ai::seekUpdate()
{
	float direction = angle(m_player->pos(), m_position);
	m_velocity = sf::Vector2f(cos(direction * 0.0174533), sin(direction * 0.0174533));
	if (length(m_velocity) > 0)
		m_velocity = normalise(m_velocity);

	m_velocity *= 100.0f;
	m_position += m_velocity * (1 / 60.0f);
	m_sprite.setRotation(direction);
	/*
	m_velocity = m_player->pos() - m_position;
	m_velocity = normalise(m_velocity);
	m_velocity *= .5f;
	m_rotation = getOrientation(m_rotation, m_velocity);

	m_sprite.setRotation(m_rotation);
	m_position += m_velocity;
	*/
}

void Ai::wanderUpdate()
{
}

void Ai::fleeUpdate()
{
}

float Ai::getOrientation(float orientation, sf::Vector2f vel)
{
	if (length(vel) > 0)
		return atan2(m_position.y, m_position.x) * 57.2958;
	return orientation;
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
