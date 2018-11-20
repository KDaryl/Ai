#include "Map.h"

Map::Map() :
	m_startTile(nullptr),
	m_isGoalSet(false),
	m_tileSize(40),
	m_goalTile(nullptr),
	m_continueBFS(false)
{
	if (!m_font.loadFromFile("ARIALBOLD.TTF"))
	{
		throw("File not found");
	}
}

Map::~Map()
{
}

void Map::update()
{
	if (m_continueBFS)
	{
		BFS();
	}
}

void Map::draw(sf::RenderWindow & window)
{
	for (auto& tile : m_tiles)
		tile.second->draw(window);
}

void Map::addTile(std::string tileName, Tile* tile)
{
	m_tiles[tileName] = tile;
}

void Map::resetMap()
{
	for (auto& tile : m_tiles)
	{
		tile.second->resetColor();
		tile.second->setCost(0);
		tile.second->setVisited(false);
	}
}

void Map::BFS(std::string pos)
{
	m_bfsClock.restart(); //Start the BFS clock

	//If we are not continuing from a previous BFS then reset tiles and start the queue out
	if (m_continueBFS == false)
	{
		//Reset tiles
		for (auto& tile : m_tiles)
		{
			if (tile.second->isObstacle() == false)
			{
				tile.second->setVisited(false);
				tile.second->setColor(sf::Color(27, 93, 214));
				tile.second->setCost(0);
			}
		}

		m_tiles[pos]->setColor(sf::Color(25, 175, 25)); //Set our current pos 
		m_tiles[pos]->setVisited(true); //Set the starting node as visited

		m_startTile = m_tiles[pos]; //Set the start tile

		m_prevBfsQueue.push_back(m_tiles[pos]);
	}

	while (!m_prevBfsQueue.empty())
	{
		//If the time gone for BFS has reached or exceeded our allowance, exit and come back later
		if (m_bfsClock.getElapsedTime().asSeconds() >= BFS_ALLOWANCE)
		{
			std::cout << "Hit allowance" << std::endl;
			m_continueBFS = true;
			break;
		}


		auto gPos = m_prevBfsQueue.front()->getIntGridPos(); //Get the integer values of the grid position
		auto cPos = gPos;
		auto originalCost = m_prevBfsQueue.front()->getCost(); //Get the cost from the previous tile

		//Get all adjacent tiles and add them to the queue

		if (gPos.first == 24 && gPos.second == 0)
		{
			int x = 0;
		}

		//Check left of this Tile
		cPos.first--;
		if (cPos.first >= 0)
		{
			tryAddToQueue(cPos, gPos, originalCost, m_prevBfsQueue);
		}
		cPos = gPos;

		//Checking the right of this Tile
		cPos.first++;
		if (cPos.first < 50)
		{
			tryAddToQueue(cPos, gPos, originalCost, m_prevBfsQueue);
		}
		cPos = gPos;

		//Checking the top of this Tile
		cPos.second--;
		if (cPos.second >= 0)
		{
			tryAddToQueue(cPos, gPos, originalCost, m_prevBfsQueue);
		}
		cPos = gPos;

		//Checking the Bottom of this Tile
		cPos.second++;
		if (cPos.second < 50)
		{
			tryAddToQueue(cPos, gPos, originalCost, m_prevBfsQueue);
		}
		cPos = gPos;

		//Checking the Bottom right of this Tile
		cPos.second++;
		cPos.first++;
		if (cPos.second < 50 && cPos.first < 50)
		{
			tryAddToQueue(cPos, gPos, originalCost, m_prevBfsQueue);
		}
		cPos = gPos;

		//Checking the Top right of this Tile
		cPos.second--;
		cPos.first++;
		if (cPos.second >= 0 && cPos.first < 50)
		{
			tryAddToQueue(cPos, gPos, originalCost, m_prevBfsQueue);
		}
		cPos = gPos;

		//Checking the Top Left of this Tile
		cPos.second--;
		cPos.first--;
		if (cPos.second >= 0 && cPos.first >= 0)
		{
			tryAddToQueue(cPos, gPos, originalCost, m_prevBfsQueue);
		}
		cPos = gPos;

		//Checking the Bottom Left of this Tile
		cPos.second++;
		cPos.first--;
		if (cPos.second < 50 && cPos.first >= 0)
		{
			tryAddToQueue(cPos, gPos, originalCost, m_prevBfsQueue);
		}

		m_prevBfsQueue.pop_front(); //Pop at the end
	}

	if (m_prevBfsQueue.empty()) //If BFS completed, set our unvisitable tiles and reset our variables
	{
		m_continueBFS = false;
		for (auto& tile : m_tiles)
		{
			if (tile.second->getVisited() == false && tile.second->isObstacle() == false)
			{
				tile.second->setCost(999);
			}
		}
	}

	std::cout << "time taken for BFS: " << m_bfsClock.getElapsedTime().asSeconds() << std::endl;
}

void Map::BFS()
{
	if (m_startTile) //If the tile is set, re run BFS
	{
		BFS(m_startTile->getGridPos());
	}
}

void Map::setGoal(Tile & tile)
{
	m_isGoalSet = true;

	if (m_goalTile != nullptr)
	{
		if (m_goalTile == &tile)
		{
			return;
		}
		else
		{
			m_goalTile->resetColor();
		}
	}

	m_goalTile = &tile;
	m_goalTile->setAsGoal();
}

std::string Map::stringify(int r, int c)
{
	return std::to_string(r) + "," + std::to_string(c); //Create string to make a grid position in the format '1,1' 
}

std::string Map::stringify(std::pair<int, int> p)
{
	return std::to_string(p.first) + "," + std::to_string(p.second); //Create string to make a grid position in the format '1,1' 
}

void Map::tryAddToQueue(std::pair<int, int>& currentPos, std::pair<int, int>& originalPos, int& originalCost, std::list<Tile*>& queue)
{
	//If the tile has not been visited and its not an obstacle, add it to the queue
	if (!m_tiles[stringify(currentPos)]->getVisited() && !m_tiles[stringify(currentPos)]->isObstacle())
	{
		m_tiles[stringify(currentPos)]->setVisited(true); //Set visited to true
		m_tiles[stringify(currentPos)]->setCost(originalCost + 1); //Set cost
		queue.push_back(m_tiles[stringify(currentPos)]); //Add to the queue
	}
}
