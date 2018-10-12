#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Ai.h"

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
	sf::Texture m_pTexture, m_aTexture;
	Player * m_player;
	Character *m_wanderAi, *m_seekAi, *m_fleeAi, *m_pursueAi, *m_slowArrive, *m_fastArrive;
};