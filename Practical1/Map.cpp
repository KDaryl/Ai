#include "Map.h"

Map::Map() :
	m_startTile(nullptr),
	m_isGoalSet(false),
	m_tileSize(40),
	m_goalTile(nullptr),
	m_continueBFS(false),
	m_showingPath(false)
{
	if (!m_font.loadFromFile("ARIALBOLD.TTF"))
	{
		throw("File not found");
	}

	//Setup 50x50 map with tiles
	for (int r = 0; r < 50; r++)
	{
		for (int c = 0; c < 50; c++)
		{
			auto gPos = std::to_string(c) + "," + std::to_string(r); //Create string to make a grid position in the format '1,1' 
			auto tSize =  m_tileSize;
			auto pos = sf::Vector2f(r * tSize, c * tSize);
			auto tile = new Tile(tSize, pos, gPos, m_font); //Create the tile
			addTile(gPos, tile); //Add tile to the map
			tile->setIntGridPos(c, r);
		}
	}

	BFS("24,24"); //Start here
}

Map::~Map()
{
}

void Map::update()
{
	if (m_continueBFS)
		BFS();

	if (m_visualiser)
	{
		m_visualiser->update();

		//Do this after visual has reached the goal
		if(m_visualiser->pos() == m_pathTaken.at(0)->getPos())
			m_showingPath = false; 

		if (m_visualiser->pathDone())
		{
			m_showingPath = false;
		}
	}
}

void Map::draw(sf::RenderWindow & window)
{
	for (auto& tile : m_tiles)
		tile.second->draw(window);

	for (auto& tile : m_whitePath)
	{
		tile->draw(window);
	}

	if (m_visualiser)
	{
		m_visualiser->draw(window);
	}
}

void Map::addTile(std::string tileName, Tile* tile)
{
	m_tiles[tileName] = tile;
}

void Map::resetMap()
{
	for (auto& tile : m_tiles)
	{
		tile.second->resetColor(false);
		tile.second->setCost(0);
		tile.second->setVisited(false);
	}
}

void Map::BFS(std::string pos)
{
	typedef std::pair<int, int> pair; //Type def for std pair of int's

	m_bfsClock.restart(); //Start the BFS clock

	//If we are not continuing from a previous BFS then reset tiles and start the queue out
	if (m_continueBFS == false)
	{
		m_showingPath = false;
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
		m_startTile->setStart(true); //Set the strat boolean

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

		//Generate the positions for all adjacent tiles
		std::vector<std::pair<int, int>> adj = { pair(cPos.first - 1, cPos.second),pair(cPos.first + 1, cPos.second),pair(cPos.first, cPos.second - 1),
												 pair(cPos.first, cPos.second + 1), pair(cPos.first + 1, cPos.second + 1), pair(cPos.first + 1, cPos.second - 1),
												 pair(cPos.first - 1, cPos.second - 1),pair(cPos.first - 1, cPos.second + 1)};
		//Get all adjacent tiles and add the valid ones to the queue
		for (auto& val : adj)
		{
			if (val.first >= 0 && val.first < 50 && val.second < 50 && val.second >= 0)
			{
				tryAddToQueue(val, gPos, originalCost, m_prevBfsQueue);
			}
		}

		m_prevBfsQueue.pop_front(); //Pop at the end
	}

	if (m_prevBfsQueue.empty()) //If BFS completed, set our unvisitable tiles
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
			m_goalTile->resetColor(true);
		}
	}

	m_goalTile = &tile;
	m_goalTile->setAsGoal();
}

void Map::aStar(Tile* goal)
{
	typedef std::pair<int, int> pair; //Type def for std pair of int's

	if (!goal) //If to is a nullptr then return as we dont have a destination
		return;

	//Caluclate H and reset visited
	for (auto& tile : m_tiles)
	{
		tile.second->calculateH(goal);
		tile.second->setVisited(false);
		tile.second->setF(tile.second->heuristic() + tile.second->getCost());
		tile.second->setPrevious(nullptr); //Reset previous ptr's
	}

	m_startTile->setVisited(true);

	std::list<Tile*> queue; //Create queue
	queue.push_back(m_startTile);


	while (!queue.empty())
	{
		auto& current = queue.front(); //Get current tile
		
		if (current == goal) //If we reached our goal then break
			break;

		auto gPos = current->getIntGridPos(); //Get the integer values of the grid position

		//Generate the grid positions for all adjacent tiles
		std::vector<std::pair<int, int>> adj = { pair(gPos.first - 1, gPos.second),pair(gPos.first + 1, gPos.second),pair(gPos.first, gPos.second - 1),
			pair(gPos.first, gPos.second + 1), pair(gPos.first + 1, gPos.second + 1), pair(gPos.first + 1, gPos.second - 1),
			pair(gPos.first - 1, gPos.second - 1),pair(gPos.first - 1, gPos.second + 1) };

		//Get all adjacent tiles and add the valid ones to the queue
		for (auto& val : adj)
		{
			//If the tile is valid and in range
			if (val.first >= 0 && val.first < 50 && val.second < 50 && val.second >= 0)
			{
				auto child = m_tiles[stringify(val.first, val.second)]; //Get the adjacent tile

				auto newCost = current->getCost() + 1; //Get the cost to this tile

				//if the child has not been visted and its not an obstacle or the cost is less than the total cost so far
				if ((!child->getVisited() || newCost < child->getCost()) && !child->isObstacle())
				{
					child->setVisited(true);
					child->setCost(newCost);
					child->setPrevious(current); //Set previous
					queue.push_back(child);
				}
			}
		}
		queue.pop_front();
		queue.sort(FComparator());
	}

	auto prev = goal;

	for (auto& tile : m_pathTaken)
	{
		if (tile->isObstacle() == false)
		{
			tile->setOutlineColor(sf::Color::Black, 255);
			tile->resetColor(true);
		}
	}

	m_whitePath.clear(); //Reset the vector
	m_pathTaken.clear(); //Clear the path taken vector

	while (prev != nullptr)
	{
		m_pathTaken.push_back(prev); //Add to the path taken
		prev->setOutlineColor(sf::Color(47, 239, 236), 255);
		//prev->setColor(sf::Color::White);
		m_whitePath.push_back(prev);
		prev = &prev->previous();
	}
	//Set to true so we can draw and show the path
	m_showingPath = true;

	//Delete the previous visualiser if we had one
	if (m_visualiser)
		delete m_visualiser;
	m_visualiser = new Visualiser(m_startTile->getPos(), m_pathTaken);
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
