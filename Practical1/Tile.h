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
	void setAsGoal();
	void setColor(sf::Color col);
	void resetColor(bool useAlpha);
	void setCost(int cost);
	void setCost(int cost, float maxCostGenerated);
	void setVisited(bool visited) { m_visited = visited; }
	void setStart(bool start) { m_isStart = start; }
	void setIntGridPos(int r, int c);
	void setPrevious(Tile* tile) { m_aStarPrevious = tile; }
	void resetTile();
	void toggleCost();
	void setOutlineColor(sf::Color color, int alpha);
	void calculateH(Tile* goal);
	void setF(double f) { m_f = f; };
	int& getCost() { return m_cost; }
	double& heuristic() { return m_h; }
	double& f() { return m_f; }
	bool& getVisited() { return m_visited; }
	sf::Vector2f& getPos() { return m_pos; }
	std::string& getGridPos() { return m_gridPos; }
	std::pair<int, int>& getIntGridPos() { return m_gridIntPos; };
	bool& isObstacle() { return m_isObstacle; }
	bool& isGoal() { return m_isGoal; }
	bool& isStart() { return m_isStart; }
	sf::IntRect& getCollisionBox() { return m_colBox; }
	Tile& previous() { return *m_aStarPrevious; }
private:
	std::pair<int, int> m_gridIntPos;
	sf::Vector2f m_pos;
	std::string m_gridPos;
	sf::Text m_text;
	sf::RectangleShape m_rect;
	sf::IntRect m_colBox;
	int m_cost;
	bool m_visited;
	bool m_isObstacle;
	bool m_isGoal;
	bool m_isStart;
	bool m_toggleCost;
	double m_h;
	double m_f;
	float m_alpha;
	Tile* m_aStarPrevious;
};

