#include "Map.h"

Map::Map() :
	m_tileSize(40)
{
	if (!m_font.loadFromFile("ARIALBOLD.TTF"))
	{
		throw("File not found");
	}
}

Map::~Map()
{
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

void Map::BFS(std::string pos)
{
	m_tiles[pos]->setColor(sf::Color(25, 175, 25)); //Set our current pos 
	m_tiles[pos]->setVisited(true); //Set the starting node as visited
	
	std::list<Tile*> queue; //Our queue for BFS
	queue.push_back(m_tiles[pos]);

	while (!queue.empty())
	{
		auto gPos = queue.front()->getIntGridPos(); //Get the integer values of the grid position
		auto cPos = gPos;
		auto originalCost = queue.front()->getCost(); //Get the cost from the previous tile

		//Get all adjacent tiles and add them to the queue

		//Check left of this Tile
		cPos.first--;
		if (cPos.first >= 0)
		{
			tryAddToQueue(cPos, gPos, originalCost, queue);
		}

		//Checking the right of this Tile
		cPos.first++;
		if (cPos.first < 50)
		{
			tryAddToQueue(cPos, gPos, originalCost, queue);
		}

		//Checking the top of this Tile
		cPos.second--;
		if (cPos.second >= 0)
		{
			tryAddToQueue(cPos, gPos, originalCost, queue);
		}

		//Checking the Bottom of this Tile
		cPos.second++;
		if (cPos.second < 50)
		{
			tryAddToQueue(cPos, gPos, originalCost, queue);
		}

		//Checking the Bottom right of this Tile
		cPos.second++;
		cPos.first++;
		if (cPos.second < 50 && cPos.first < 50)
		{
			tryAddToQueue(cPos, gPos, originalCost, queue);
		}

		//Checking the Top right of this Tile
		cPos.second--;
		cPos.first++;
		if (cPos.second >= 0 && cPos.first < 50)
		{
			tryAddToQueue(cPos, gPos, originalCost, queue);
		}

		//Checking the Top Left of this Tile
		cPos.second--;
		cPos.first--;
		if (cPos.second >= 0 && cPos.first >= 0)
		{
			tryAddToQueue(cPos, gPos, originalCost, queue);
		}

		//Checking the Bottom Left of this Tile
		cPos.second++;
		cPos.first--;
		if (cPos.second < 50 && cPos.first >= 0)
		{
			tryAddToQueue(cPos, gPos, originalCost, queue);
		}


		queue.pop_front(); //Pop at the end
	}


													// Mark all the vertices as not visited 
	//bool *visited = new bool[V];
	//for (int i = 0; i < V; i++)
	//	visited[i] = false;

	//// Create a queue for BFS 
	//list<int> queue;

	//// Mark the current node as visited and enqueue it 
	//visited[s] = true;
	//queue.push_back(s);

	//// 'i' will be used to get all adjacent 
	//// vertices of a vertex 
	//list<int>::iterator i;

	//while (!queue.empty())
	//{
	//	// Dequeue a vertex from queue and print it 
	//	s = queue.front();
	//	cout << s << " ";
	//	queue.pop_front();

	//	// Get all adjacent vertices of the dequeued 
	//	// vertex s. If a adjacent has not been visited,  
	//	// then mark it visited and enqueue it 
	//	for (i = adj[s].begin(); i != adj[s].end(); ++i)
	//	{
	//		if (!visited[*i])
	//		{
	//			visited[*i] = true;
	//			queue.push_back(*i);
	//		}
	//	}
	//}


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

	currentPos = originalPos;
}
