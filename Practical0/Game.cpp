#include "Game.h"

//updates per millisecond
static sf::Int32 MS_PER_UPDATE = 10;

Game::Game() :
	m_window(sf::VideoMode(1920, 1080), "AI Lab"),
	m_player(new Player(sf::Vector2f(1920 / 2, 1080 / 2), sf::Vector2f(1, 1))),
	m_seekAi(new Ai("Seek", sf::Vector2f(100, 100), m_player)),
	m_wanderAi(new Ai("Wander", sf::Vector2f(1280, 1000), m_player)),
	m_pursueAi(new Ai("Pursue", sf::Vector2f(900, 600), m_player)),
	m_slowArrive(new Ai("Arrive", sf::Vector2f(700, 800), m_player)),
	m_fastArrive(new Ai("Arrive", sf::Vector2f(0, 900), m_player))
{
	init();
	m_aTexture.loadFromFile("Alien.png");
	m_pTexture.loadFromFile("Player.png");

	m_player->setTexture(m_pTexture);
	m_seekAi->setTexture(m_aTexture);
	m_wanderAi->setTexture(m_aTexture);
	m_pursueAi->setTexture(m_aTexture);
	m_slowArrive->setTexture(m_aTexture);
	m_fastArrive->setTexture(m_aTexture);
}

void Game::init()
{
	m_window.setVerticalSyncEnabled(true); //Set FPS limit to screen refresh rate
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
	m_player->update();
	m_seekAi->update();
	m_wanderAi->update();
	m_pursueAi->update();
	m_slowArrive->update();
	m_fastArrive->update();
}

void Game::render()
{
	m_window.clear(sf::Color::White); //Clear all previously drawn items

	m_player->render(m_window);
	m_seekAi->render(m_window);
	m_wanderAi->render(m_window);
	m_pursueAi->render(m_window);
	m_slowArrive->render(m_window);
	m_fastArrive->render(m_window);

	m_window.display(); //Display all drawn items
}
