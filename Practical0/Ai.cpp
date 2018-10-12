#include "Ai.h"
#include <iostream>

Ai::Ai(std::string name, sf::Vector2f pos, Player* player) :
	type(name),
	m_position(pos),
	m_player(player),
	m_velocity(0,0),
	m_target(0,0)
{
	if (type == "Wander") //Set our clock and start it
	{
		m_wanderClock = new sf::Clock();
		m_wanderClock->restart();
		m_target = sf::Vector2f(100,100);
	}
	m_box.setSize(sf::Vector2f(100, 50));
	m_box.setFillColor(sf::Color(0, 0, 0, 150));
	m_box.setOrigin(50, 25);

	m_font.loadFromFile("ARIALBOLD.TTF");
	m_text.setFont(m_font);
	m_text.setCharacterSize(25);

	if (name == "Pursue")
		m_text.setString("Pursue");
	if (name == "Wander")
		m_text.setString("Wander");
	if (name == "Seek")
		m_text.setString("Seek");
	if (name == "Arrive")
		m_text.setString("Arrive");


	m_text.setOrigin(m_text.getGlobalBounds().width / 2.0f, m_text.getGlobalBounds().height / 2.0f);
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
	if (type == "Pursue")
		pursueUpdate();
	if (type == "Arrive")
		arriveUpdate();

	clampPos(); //Wrap our ai around

	m_sprite.setPosition(m_position); //Set our sprites position
	m_sprite.setRotation(m_rotation); //Set our sprites rotation

	m_box.setPosition(m_position); //Set our text box position
	m_text.setPosition(m_position); //Set our text position
}

void Ai::seekUpdate(sf::Vector2f target)
{
	float maxSpeed = 75.0f; //Change this for arrival

	sf::Vector2f newTarget = target == sf::Vector2f(0, 0) ? m_player->pos() : target;

	m_target = newTarget;

	float dt = (1 / 60.0f);
	sf::Vector2f desired = normalise(newTarget - m_position);
	desired = normalise(desired) * maxSpeed;

	sf::Vector2f steering = desired - m_velocity;
	truncate(steering, 10.0f);

	m_velocity += steering * dt;
	truncate(m_velocity, maxSpeed);

	m_position += m_velocity * dt;
	m_rotation = getOrientation(m_velocity);
}

void Ai::wanderUpdate()
{
	if (m_wanderClock->getElapsedTime().asSeconds() > 4) //Every 2 seconds
	{
		m_wanderClock->restart();
		m_target = sf::Vector2f(iRand(0, 1920),iRand(0, 1080));
	}

	seekUpdate(m_target);
}

void Ai::fleeUpdate()
{
	float maxSpeed = 75.0f; //Change this for arrival
	float dt = (1 / 60.0f);
	sf::Vector2f desired = normalise(m_position - m_player->pos());
	desired = normalise(desired) * maxSpeed;

	sf::Vector2f steering = desired - m_velocity;
	truncate(steering, 10.0f);

	m_velocity += steering * dt;
	truncate(m_velocity, maxSpeed);

	m_position += m_velocity * dt;
	m_rotation = getOrientation(m_velocity);
}

void Ai::pursueUpdate()
{
	m_target = m_player->pos() + m_player->vel() * 1.5f; //Anticipate where the player will be

	seekUpdate(m_target); //Seek to it
}

void Ai::arriveUpdate()
{
	float maxSpeed = 75.0f; //Change this for arrival

	if (distance(m_player->pos(), m_position) < 250)
		maxSpeed *= 0.5f;
	if (distance(m_player->pos(), m_position) < 100)
		maxSpeed = 0.0f;

	if (maxSpeed > 0)
	{

		m_target = m_player->pos();

		float dt = (1 / 60.0f);
		sf::Vector2f desired = normalise(m_target - m_position);
		desired = normalise(desired) * maxSpeed;

		sf::Vector2f steering = desired - m_velocity;
		truncate(steering, 10.0f);

		m_velocity += steering * dt;
		truncate(m_velocity, maxSpeed);

		m_position += m_velocity * dt;
		m_rotation = getOrientation(m_velocity);
	}
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

	sf::CircleShape cs;
	cs.setFillColor(sf::Color::Red);
	cs.setRadius(10);
	cs.setPosition(m_target);
	window.draw(cs);
	window.draw(m_box);
	window.draw(m_text);
}

void Ai::setTexture(sf::Texture & texture)
{
	m_sprite.setTexture(texture);
	m_sprite.setOrigin(sf::Vector2f(m_sprite.getGlobalBounds().width / 2.0, m_sprite.getGlobalBounds().height / 2.0));
	m_sprite.setPosition(m_position);
	m_sprite.setRotation(m_rotation);
}

void Ai::setArriveSpeed(float nearSpeed)
{
	m_arriveSpeed = nearSpeed;
}

void Ai::truncate(sf::Vector2f & v, float max)
{
	if (length(v) > 0)
	{
		float i = 0;

		i = max / length(v);

		i = i < 1.0 ? 1.0 : i;

		v *= i;
	}
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
