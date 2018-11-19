#include "Tile.h"

Tile::Tile(int size, sf::Vector2f pos, std::string gridPos, sf::Font& font):
	m_cost(0),
	m_visited(false),
	m_text("0,0",font),
	m_gridPos(gridPos),
	m_isObstacle(false)
{
	m_rect.setSize(sf::Vector2f(size, size));
	m_rect.setFillColor(sf::Color(25, 25, 175));
	m_rect.setOutlineThickness(5);
	m_rect.setOutlineColor(sf::Color::Black);
	m_rect.setOrigin(m_rect.getGlobalBounds().left + m_rect.getGlobalBounds().width / 2.0, m_rect.getGlobalBounds().top + m_rect.getGlobalBounds().height / 2.0); //Set origin to the centre of the rectangle
	m_rect.setPosition(pos.x + (size / 2.0), pos.y + (size / 2.0));

	m_text.setCharacterSize(20);
	m_text.setString(std::to_string(m_cost));
	m_text.setFillColor(sf::Color::White);
	m_text.setOrigin(m_text.getGlobalBounds().left + m_text.getGlobalBounds().width / 2.0, m_text.getGlobalBounds().top + m_text.getGlobalBounds().height / 2.0);
	m_text.setPosition(m_rect.getPosition());

	m_pos = m_rect.getPosition(); //Ste our position
}

Tile::~Tile()
{
}

void Tile::draw(sf::RenderWindow & window)
{
	window.draw(m_rect);

	window.draw(m_text);
}

void Tile::setAsObstacle()
{
	m_rect.setFillColor(sf::Color::Red);
	m_isObstacle = true;
}

void Tile::setColor(sf::Color col)
{
	m_rect.setFillColor(col);
}

void Tile::resetColor()
{
	m_rect.setFillColor(sf::Color(25, 25, 175)); //Reset the colour to the normal blue
	m_isObstacle = false;
}

void Tile::setCost(int cost)
{
	m_cost = cost;
	m_text.setString(std::to_string(m_cost));
}

void Tile::setIntGridPos(int r, int c)
{
	m_gridIntPos.first = r;
	m_gridIntPos.second = c;
}
