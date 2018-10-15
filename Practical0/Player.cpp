#include "Player.h"
#include <iostream>

Player::Player(sf::Vector2f pos, sf::Vector2f vel) :
	m_position(pos),
	m_velocity(vel),
	m_rotation(0),
	m_speed(1)
{
}

Player::~Player()
{
}

void Player::update()
{
	//Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		incRotation();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		decRotation();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		incSpeed();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		decSpeed();

	//Move player bassed off of rotation
	sf::Vector2f direction = sf::Vector2f(cos(m_rotation * 0.0174533), sin(m_rotation * 0.0174533));
	if (length(direction) > 0)
		direction = normalise(direction);

	m_velocity = direction * m_speed;

	m_position += m_velocity * (1 /60.f);
	clampPos();

	m_sprite.setPosition(m_position);
}

void Player::render(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void Player::setTexture(sf::Texture & texture)
{
	m_sprite.setTexture(texture);
	m_sprite.setOrigin(sf::Vector2f(m_sprite.getGlobalBounds().width / 2.0, m_sprite.getGlobalBounds().height / 2.0));
	m_sprite.setPosition(m_position);
	m_sprite.setRotation(m_rotation);
}

void Player::clampPos()
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

void Player::incRotation()
{
	m_rotation++;
	m_sprite.setRotation(m_rotation);
}

void Player::decRotation()
{
	m_rotation--;
	m_sprite.setRotation(m_rotation);
}

void Player::incSpeed()
{
	if(m_speed < 350)
		m_speed += 2;
}

void Player::decSpeed()
{
	if(m_speed > 0)
		m_speed -= 2;
}
