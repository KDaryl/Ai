#pragma once
#include <map>
#include <list>
#include "Tile.h"

class Map
{
public:
	Map();
	~Map();

	void draw(sf::RenderWindow& window);

	void addTile(std::string tileName, Tile* tile);
	void BFS(std::string from);

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
private:
	//Private helper function for BFS
	void tryAddToQueue(std::pair<int,int>& currentPos, std::pair<int, int>& originalPos, int& originalCost, std::list<Tile*>& queue);

	sf::Font m_font; //Used for drawing costs
	std::map<std::string, Tile*> m_tiles; //Tiles
	int m_tileSize; //Size of a tile width & height
};

