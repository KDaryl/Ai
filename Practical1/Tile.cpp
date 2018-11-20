#include "Tile.h"

Tile::Tile(int size, sf::Vector2f pos, std::string gridPos, sf::Font& font):
	m_cost(0),
	m_visited(false),
	m_text("0,0",font),
	m_gridPos(gridPos),
	m_isObstacle(false),
	m_isGoal(false)
{
	m_rect.setSize(sf::Vector2f(size, size));
	m_rect.setFillColor(sf::Color(27, 93, 214));
	m_rect.setOutlineThickness(5);
	m_rect.setOutlineColor(sf::Color::Black);
	m_rect.setOrigin(m_rect.getGlobalBounds().left + m_rect.getGlobalBounds().width / 2.0f, m_rect.getGlobalBounds().top + m_rect.getGlobalBounds().height / 2.0f); //Set origin to the centre of the rectangle
	m_rect.setPosition(pos.x + (size / 2.0f), pos.y + (size / 2.0f));

	m_pos = m_rect.getPosition(); //Set our position

	m_colBox.left = m_pos.x - (size / 2.0f);
	m_colBox.top = m_pos.y - (size / 2.0f);
	m_colBox.width = size;
	m_colBox.height = size;

	m_text.setCharacterSize(16);
	m_text.setString(std::to_string(m_cost));
	m_text.setFillColor(sf::Color::White);
	m_text.setOrigin(m_text.getGlobalBounds().left + m_text.getGlobalBounds().width / 2.0f, m_text.getGlobalBounds().top + m_text.getGlobalBounds().height / 2.0f);
	m_text.setPosition(m_pos);
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
	m_rect.setFillColor(sf::Color(255, 255, 102));
	m_isObstacle = true;
	setCost(999);
}

void Tile::setAsGoal()
{
	setColor(sf::Color(255, 51, 0));
	m_isGoal = true;
}

void Tile::setColor(sf::Color col)
{
	m_rect.setFillColor(col);
}

void Tile::resetColor()
{
	m_rect.setFillColor(sf::Color(27, 93, 214)); //Reset the colour to the normal blue
	m_text.setFillColor(sf::Color::White);
	m_isObstacle = false;
	m_isGoal = false;
	setCost(m_cost);
}

void Tile::setCost(int cost)
{
	if (m_isGoal)
	{
		setAsGoal();
	}

	m_cost = cost;
	m_text.setString(std::to_string(m_cost));
	auto col = m_rect.getFillColor();
	col.a = 255 - (255 * (cost / 100.0f));
	auto tCol = m_text.getFillColor();
	tCol.a = col.a;
	m_rect.setFillColor(col);
	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2.0f, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2.0f);
	m_text.setPosition(m_pos);
	m_text.setFillColor(tCol);
}

void Tile::setIntGridPos(int r, int c)
{
	m_gridIntPos.first = r;
	m_gridIntPos.second = c;
}

void Tile::resetTile()
{
	m_visited = false;
	setColor(sf::Color(27, 93, 214));
	m_isObstacle = false;
	setCost(0);
}
