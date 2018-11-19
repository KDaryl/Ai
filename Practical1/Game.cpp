#include "Game.h"

//updates per millisecond
static sf::Int32 MS_PER_UPDATE = 10;

Game::Game() :
	m_window(sf::VideoMode(2000, 2000), "AI Lab - Vector Field")
{
	init();
}

void Game::init()
{
	m_window.setVerticalSyncEnabled(true); //Set FPS limit to screen refresh rate

	//Setup 50x50 map with tiles
	for (int r = 0; r < 50; r++)
	{
		for (int c = 0; c < 50; c++)
		{
			auto gPos = std::to_string(c) + "," + std::to_string(r); //Create string to make a grid position in the format '1,1' 
			auto tSize = m_map.TileSize();
			auto pos = sf::Vector2f(r * tSize, c * tSize);
			auto tile = new Tile(tSize, pos, gPos, m_map.Font()); //Create the tile
			m_map.addTile(gPos, tile); //Add tile to the map
			tile->setIntGridPos(c, r);
		}
	}

	m_map.BFS("24,24"); //Start at the center
}

void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;
	while (m_window.isOpen())
	{
		processEvents();
		sf::Time dt = clock.restart();
		lag += dt.asMilliseconds();

		while (lag > MS_PER_UPDATE)
		{
			update();
			lag -= MS_PER_UPDATE;
		}
		update();
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	bool handledInput = false;
	while (m_window.pollEvent(event))
	{
		// check the type of the event...
		switch (event.type)
		{
		// window closed
		case sf::Event::Closed:
			m_window.close();
			break;

		// we don't process other types of events
		default:
			break;
		}
	}
}

void Game::update()
{

}

void Game::render()
{
	m_window.clear(sf::Color::Black); //Clear all previously drawn items

	m_map.draw(m_window); //Draw the map

	m_window.display(); //Display all drawn items
}
