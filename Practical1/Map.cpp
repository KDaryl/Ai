#include "Map.h"

Map::Map() :
	m_startTile(nullptr),
	m_isGoalSet(false),
	m_tileSize(40),
	m_goalTile(nullptr),
	m_continueBFS(false),
	m_showingPath(false),
	m_maxCost(0.0f),
	m_foundPath(false)
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

	BFS(m_tiles["10,40"], m_tiles["10,45"]); //Start here
}

Map::~Map()
{
}

void Map::update()
{
	if (m_continueBFS)
		BFS(m_startTile, m_goalTile);

	if (m_foundPath)
	{
		m_visualiser.update();

		//Do this after visual has reached the goal
		if(m_visualiser.pos() == m_pathTaken.at(0)->getPos())
			m_showingPath = false; 

		if (m_visualiser.pathDone())
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

	if (m_foundPath)
	{
		m_visualiser.draw(window);
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

void Map::BFS(Tile* from, Tile* goal)
{
	typedef std::pair<int, int> pair; //Type def for std pair of int's

	m_bfsClock.restart(); //Start the BFS clock

	//If we are not continuing from a previous BFS then reset tiles and start the queue out
	if (m_continueBFS == false)
	{
		m_showingPath = false;
		m_foundPath = false;
		//Reset tiles
		for (auto& tile : m_tiles)
		{
			if (tile.second->isObstacle() == false)
			{
				tile.second->setVisited(false);
				tile.second->setColor(sf::Color(27, 93, 214));
				tile.second->setCost(0);
				tile.second->setPrevious(nullptr);
			}
		}

		goal->setAsGoal();
		from->setColor(sf::Color(25, 175, 25)); //Set our current pos 
		from->setVisited(true); //Set the starting node as visited

		m_goalTile = goal; //Set the goal tile
		m_goalTile->setAsGoal(); //Set the goal tiles boolean
		m_startTile = from; //Set the start tile
		m_startTile->setStart(true); //Set the start boolean

		m_prevBfsQueue.push_back(m_startTile);
	}

	while (!m_prevBfsQueue.empty())
	{
		//If the time gone for BFS has reached or exceeded our allowance, exit and come back later
		if (m_bfsClock.getElapsedTime().asSeconds() >= BFS_ALLOWANCE)
		{
			m_continueBFS = true;
			break;
		}

		auto gPos = m_prevBfsQueue.front()->getIntGridPos(); //Get the integer values of the grid position
		auto cPos = gPos;
		auto originalCost = m_prevBfsQueue.front()->getCost(); //Get the cost from the previous tile

		//Generate the positions for all adjacent tiles
		std::vector<std::pair<int, int>> adj = { pair(cPos.first - 1, cPos.second),pair(cPos.first + 1, cPos.second),pair(cPos.first, cPos.second - 1),
												 pair(cPos.first, cPos.second + 1), pair(cPos.first + 1, cPos.second + 1), pair(cPos.first + 1, cPos.second - 1),
												 pair(cPos.first - 1, cPos.second - 1),pair(cPos.first - 1, cPos.second + 1) };
		//Get all adjacent tiles and add the valid ones to the queue
		for (auto& val : adj)
		{
			if (val.first >= 0 && val.first < 50 && val.second < 50 && val.second >= 0)
			{
				//If the tile has not been visited and its not an obstacle, add it to the queue
				if (!m_tiles[stringify(val)]->getVisited() && !m_tiles[stringify(val)]->isObstacle())
				{
					if (originalCost + 1 > m_maxCost)
						m_maxCost = originalCost + 1;

					m_tiles[stringify(val)]->setVisited(true); //Set visited to true
					m_tiles[stringify(val)]->setCost(originalCost + 1); //Set cost
					m_prevBfsQueue.push_back(m_tiles[stringify(val)]); //Add to the queue
					m_tiles[stringify(val)]->setPrevious(m_prevBfsQueue.front()); //Set the previous string
				}
			}
		}

		m_prevBfsQueue.pop_front(); //Pop at the end
	}

	if (m_prevBfsQueue.empty()) //If BFS completed, set our unvisitable tiles
	{
		m_continueBFS = false;
		//Loop through the tiles and set the the obstacles cost
		for (auto& tile : m_tiles)
		{
			if (tile.second->getVisited() == false && tile.second->isObstacle() == false)
				tile.second->setCost(999);
			else
				tile.second->setCost(tile.second->getCost(), m_maxCost);
		}

		m_pathTaken.clear();

		//Create our path taken variable from the start to the goal
		for(auto& prev = goal; prev != nullptr && !m_foundPath;)
		{
			m_pathTaken.push_back(prev);
			if (prev == from)
			{
				m_foundPath = true;
			}
			prev = &prev->previous();
		}

		if (m_foundPath)
		{
			//Set the colour of the path
			for (auto& tile : m_pathTaken)
			{
				//if (tile != from && tile != goal)
					//tile->setColor(sf::Color::Red);
			}

			//Set to true so we can draw and show the path
			m_showingPath = true;

			m_visualiser.setVisualiser(m_startTile->getPos(), m_pathTaken);
		}
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

void Map::toggleCosts()
{
	for (auto& tile : m_tiles)
		tile.second->toggleCost();
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
