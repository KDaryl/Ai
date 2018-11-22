#pragma once
#include "SFML/Graphics.hpp"

class Map;
class Tile;

//Used to handle key presses, set default values to false
struct Input
{
	bool LEFT_MOUSE = false;
	bool RIGHT_MOUSE = false;
	bool ONE_BUTTON = false;
	bool TWO_BUTTON = false;
	bool THREE_BUTTON = false;
	bool SPACE = false;
	bool TAB = false;
};

class InputHandler
{
public:
	InputHandler(sf::RenderWindow& win);
	void update(Map& map);
	void draw();
	void checkInput();
	void setStartTile(Tile& start) { m_startTile = &start; };
	void setBooleans(bool placeS, bool placeG, bool placeO, sf::Color colour);
	double distance(sf::Vector2f a, sf::Vector2f b) { return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)); };
private:
	Input m_current, m_previous;
	sf::CircleShape m_mouseCircle;
	sf::IntRect m_collisionBox;
	sf::RenderWindow* m_renderWin;
	bool m_placeStart;
	bool m_placeGoal;
	bool m_placeObstacle;
	Tile* m_startTile;
};
