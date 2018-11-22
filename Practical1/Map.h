#pragma once
#include <map>
#include <list>
#include "Visualiser.h"


class Map
{
public:
	Map();
	~Map();
	struct FComparator
	{
		bool operator() (Tile* a, Tile* b) const
		{
			return a->f() < b->f(); //Compare the (f) values of the two tiles
		}
	};

	void update();
	void draw(sf::RenderWindow& window);

	void addTile(std::string tileName, Tile* tile);
	void resetMap();
	void BFS(Tile* from, Tile* goal);
	void setGoal(Tile& tile);
	//void aStar(Tile* to);
	void toggleCosts();

	int& TileSize() { return m_tileSize; }
	sf::Font& Font() { return m_font; }

	std::string stringify(int r, int c); //Method that converts 2 integers value sinto a string (a readable grid position)
	std::string stringify(std::pair<int,int> p); //Method that converts 2 integers value sinto a string (a readable grid position)
	std::map<std::string, Tile*>& getTiles() { return m_tiles; }
	Tile* getGoal() { return m_goalTile; }
	Tile* getStart() { return m_startTile; }
	bool& isGoalSet() { return m_isGoalSet; }
	bool& continueBfs() { return m_continueBFS; }
	bool showingPath() { return m_showingPath; }
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
	const float BFS_ALLOWANCE = 0.99f; //Allow only 0.1 Second for BFS
	bool m_continueBFS; //Bool to show wheter we need to continue from a previous BFS
	std::vector<Tile*> m_whitePath; //All of the tiles that were changed to white to show the path
	std::vector<Tile*> m_pathTaken; //The path to take after a star
	bool m_showingPath;
	Visualiser m_visualiser;
	float m_maxCost; //Used to determine the max cost generated during BFS for the heatmap to generate correctly
	bool m_foundPath; // Used to find a path to the goal
};

