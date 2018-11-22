#include "Game.h"

//updates per millisecond
static sf::Int32 MS_PER_UPDATE = 10;

Game::Game() :
	m_window(sf::VideoMode(2000, 2000), "AI Lab - Vector Field"),
	m_inputHandler(m_window)
{
	init();
}

void Game::init()
{
	//m_window.setVerticalSyncEnabled(true); //Set FPS limit to screen refresh rate
	m_inputHandler.setStartTile(*m_map.getTiles()["24,24"]);
	std::cout << "Controls:\nTo change the tile mode->\n'1' to change to Start(Green) tile\n'2' to change to Goal(Red) tile\n'3' to change to Obstacle(Yellow) tile\n'TAB' to toggle costs\n'SPACE BAR' to begin the pathfinding!\n'LEFT MOUSE CLICK' to place a tile of type 'MODE'\n'RIGHT MOUSE CLICK' to remove a tile, this only works in Obstacle mode!" << std::endl;
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
	m_map.update();

	m_inputHandler.update(m_map);
}

void Game::render()
{
	m_window.clear(sf::Color::Black); //Clear all previously drawn items

	m_map.draw(m_window); //Draw the map

	m_inputHandler.draw();

	m_window.display(); //Display all drawn items
}
