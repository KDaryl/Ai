#pragma once
#include "Map.h"
#include "InputHandler.h"

//Game loop
class Game
{
public:
	Game();
	void run();
	void init();
	void processEvents();
private:
	void update();
	void render();

	sf::RenderWindow m_window; //Render window

	//Lab specific variables
	Map m_map;
	InputHandler m_inputHandler;
};