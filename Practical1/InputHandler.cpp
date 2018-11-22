#include "InputHandler.h"
#include "Map.h"
#include <iostream>

InputHandler::InputHandler(sf::RenderWindow& win) :
	m_renderWin(&win),
	m_placeStart(true),
	m_placeGoal(false),
	m_placeObstacle(false),
	m_collisionBox(0, 0, 5, 5)
{
	m_mouseCircle.setRadius(2.5f);
	m_mouseCircle.setOutlineColor(sf::Color::Green);
	m_mouseCircle.setFillColor(sf::Color::Green);
	m_mouseCircle.setOutlineThickness(5);
	m_mouseCircle.setOrigin(2.5f, 2.5f);
}

void InputHandler::update(Map& map)
{
	checkInput(); //Check for input

	if (m_current.TAB && !m_previous.TAB)
		map.toggleCosts();

	//Only allow input if th emap is not showing a path
	if (!map.showingPath())
	{
		//Handle key presses
		if (m_current.ONE_BUTTON && !m_previous.ONE_BUTTON)
			setBooleans(true, false, false, sf::Color::Green);
		if (m_current.TWO_BUTTON && !m_previous.TWO_BUTTON)
			setBooleans(false, true, false, sf::Color::Red);
		if (m_current.THREE_BUTTON && !m_previous.THREE_BUTTON)
			setBooleans(false, false, true, sf::Color::Yellow);

		//If the map is not doing BFS then we can manipulate it
		if (map.continueBfs() == false)
		{
			//If space bar is pressed, run A star to the goal
			if (m_current.SPACE && !m_previous.SPACE)
			{

				map.BFS(map.getStart(), map.getGoal()); //Run BFS on the map on the map
			}

			if (m_current.LEFT_MOUSE && !m_previous.LEFT_MOUSE) //If we left mouse clicked
			{

				//If we are placing our start tile!, we can only place it on tiles that are not the goal or obstacles
				if (m_placeStart)
				{
					for (auto& tile : map.getTiles())
					{
						if (tile.second->getCollisionBox().intersects(m_collisionBox) && !tile.second->isGoal() && !tile.second->isObstacle())
						{
							if (m_startTile)
								m_startTile->setStart(false);
							tile.second->setStart(true);
							m_startTile = tile.second;
							map.BFS(m_startTile, map.getGoal()); //Run BFS on the map on the map
							break;
						}
					}

				}

				//If we are placing an obstacle, we can only place obstacles on empty tiles (not goal or start)
				if (m_placeObstacle)
				{
					for (auto& tile : map.getTiles())
					{
						if (tile.second->getCollisionBox().intersects(m_collisionBox) && !tile.second->isGoal() && !tile.second->isStart()) //Place if not the start or goal
						{
							tile.second->setAsObstacle(); //Set this tile as an obstacle
							//map.BFS(); //Run BFS using the last set start tile
							break;
						}
					}
				}

				//Place goal on an empty tile
				if (m_placeGoal)
				{
					for (auto& tile : map.getTiles())
					{
						if (tile.second->getCollisionBox().intersects(m_collisionBox) && !tile.second->isObstacle() && !tile.second->isStart())
						{
							map.setGoal(*tile.second); //Set this tile as the goal
							break;
						}
					}
				}
			}

			if (m_current.RIGHT_MOUSE && !m_previous.RIGHT_MOUSE)
			{

				//If we are removing an obstacle
				if (m_placeObstacle)
				{
					for (auto& tile : map.getTiles())
					{
						if (tile.second->getCollisionBox().intersects(m_collisionBox) && tile.second->isObstacle())
						{
							tile.second->resetTile(); //Reset this tile if it is an obstacle
							//map.BFS(); //Run BFS using the last set start tile
							break;
						}
					}
				}
			}
		}
	}
}

void InputHandler::draw()
{
	m_renderWin->draw(m_mouseCircle);
}

void InputHandler::checkInput()
{
	//Set mouse rectangle position
	m_mouseCircle.setPosition(sf::Mouse::getPosition(*m_renderWin).x, sf::Mouse::getPosition(*m_renderWin).y);
	m_collisionBox.left = m_mouseCircle.getPosition().x - 2.5f;
	m_collisionBox.top = m_mouseCircle.getPosition().y - 2.5f;

	//Set previous as current
	m_previous = m_current;

	//Checking mouse clicks
	m_current.LEFT_MOUSE = sf::Mouse::isButtonPressed(sf::Mouse::Left); //Checking left click		
	m_current.RIGHT_MOUSE = sf::Mouse::isButtonPressed(sf::Mouse::Right); //Checking right click

	//Check key presses
	m_current.ONE_BUTTON = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1); //Checking 1 press
	m_current.TWO_BUTTON = sf::Keyboard::isKeyPressed(sf::Keyboard::Num2); //Checking 2 press
	m_current.THREE_BUTTON = sf::Keyboard::isKeyPressed(sf::Keyboard::Num3); //Checking 3 press
	m_current.SPACE = sf::Keyboard::isKeyPressed(sf::Keyboard::Space); //Checking Space bar press
	m_current.TAB = sf::Keyboard::isKeyPressed(sf::Keyboard::Tab); //Checking for Left Tab press
}

void InputHandler::setBooleans(bool placeS, bool placeG, bool placeO, sf::Color colour)
{
	m_placeStart = placeS;
	m_placeGoal = placeG;
	m_placeObstacle = placeO;

	m_mouseCircle.setOutlineColor(colour);
	m_mouseCircle.setFillColor(colour);
}
