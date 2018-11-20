#pragma once
#include <map>
#include <list>
#include "Tile.h"

class Map
{
public:
	Map();
	~Map();

	void update();
	void draw(sf::RenderWindow& window);

	void addTile(std::string tileName, Tile* tile);
	void resetMap();
	void BFS(std::string from);
	void BFS();
	void setGoal(Tile& tile);

	int& TileSize() { return m_tileSize; }
	sf::Font& Font() { return m_font; }

	struct TileComparator
	{
		bool operator() (Tile& a, Tile& b)
		{
			return std::tie(a.getPos().x, a.getPos().y) < std::tie(b.getPos().x, b.getPos().y); //Compare sthe position of two tiles
		}
	};

	std::string stringify(int r, int c); //Method that converts 2 integers value sinto a string (a readable grid position)
	std::string stringify(std::pair<int,int> p); //Method that converts 2 integers value sinto a string (a readable grid position)
	std::map<std::string, Tile*>& getTiles() { return m_tiles; }
	bool& isGoalSet() { return m_isGoalSet; }
	bool& continueBfs() { return m_continueBFS; }
private:
	//Private helper function for BFS
	void tryAddToQueue(std::pair<int,int>& currentPos, std::pair<int, int>& originalPos, int& originalCost, std::list<Tile*>& queue);

	sf::Font m_font; //Used for drawing costs
	std::map<std::string, Tile*> m_tiles; //Tiles
	int m_tileSize; //Size of a tile width & height

	Tile* m_startTile, *m_goalTile;
	bool m_isGoalSet;

	//BFS Variables, for performance, will stop and continue BFS to improve performance
	sf::Clock m_bfsClock; //Clock used to determine wheter BFS is taking too long or not
	std::list<Tile*> m_prevBfsQueue; //Our queue for BFS, we will use this to continue afte rlong BFS searches
	const float BFS_ALLOWANCE = 0.01f; //Allow only 0.1 Second for BFS
	bool m_continueBFS; //Bool to show wheter we need to continue from a previous BFS
};

