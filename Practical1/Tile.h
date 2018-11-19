#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
class Tile
{
public:
	Tile(int size, sf::Vector2f pos, std::string gridPos, sf::Font& font);
	~Tile();

	void draw(sf::RenderWindow& window);

	void setAsObstacle();
	void setColor(sf::Color col);
	void resetColor();
	void setCost(int cost);
	void setVisited(bool visited) { m_visited = visited; }
	void setIntGridPos(int r, int c);
	int& getCost() { return m_cost; }
	bool& getVisited() { return m_visited; }
	sf::Vector2f& getPos() { return m_pos; }
	std::string& getGridPos() { return m_gridPos; }
	std::pair<int, int>& getIntGridPos() { return m_gridIntPos; };
	bool& isObstacle() { return m_isObstacle; }
private:
	std::pair<int, int> m_gridIntPos;
	sf::Vector2f m_pos;
	std::string m_gridPos;
	sf::Text m_text;
	sf::RectangleShape m_rect;
	int m_cost;
	bool m_visited;
	bool m_isObstacle;
};

