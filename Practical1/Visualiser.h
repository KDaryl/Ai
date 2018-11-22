#pragma once
#include "Tile.h"
#include "Line.h"

class Tile;

class Visualiser
{
public:
	Visualiser();
	~Visualiser();

	void setVisualiser(sf::Vector2f pos, std::vector<Tile*>& pathTaken);
	void update();
	void draw(sf::RenderWindow& win);

	sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float time);

	double distance(sf::Vector2f a, sf::Vector2f b);
	std::vector<sf::Vector2f>& pathLine() { return m_pathLine; }
	sf::Vector2f pos() { return m_circle.getPosition(); }
	bool& pathDone() { return m_reachedPath; }
private:
	sf::CircleShape m_circle;
	std::vector<sf::Vector2f> m_pathLine; //The path we will draw at runtime
	sf::Vector2f m_dest, m_position;
	std::vector<Tile*>* m_pathTakenPtr;
	sf::Clock m_clock;
	int m_index;
	int m_maxIndex;
	std::vector<Line> m_lines;
	bool m_reachedPath;
};

