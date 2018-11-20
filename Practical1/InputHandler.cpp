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
	m_mouseRect.setSize(sf::Vector2f(5,5));
	m_mouseRect.setOutlineColor(sf::Color::Green);
	m_mouseRect.setFillColor(sf::Color::Transparent);
	m_mouseRect.setOutlineThickness(5);
	m_mouseRect.setOrigin(2.5f, 2.5f);
}

void InputHandler::update(Map& map)
{
	checkInput(); //Check for input

	//Handle key presses
	if (m_current.ONE_BUTTON && !m_previous.ONE_BUTTON)
		setBooleans(true, false, false);
	if (m_current.TWO_BUTTON && !m_previous.TWO_BUTTON)
		setBooleans(false, true, false);
	if (m_current.THREE_BUTTON && !m_previous.THREE_BUTTON)
		setBooleans(false, false, true);

	//If the map is not doing BFS then we can manipulate it
	if (map.continueBfs() == false)
	{
		if (m_current.LEFT_MOUSE && !m_previous.LEFT_MOUSE) //If we left mouse clicked
		{
			std::cout << "Left Mouse click" << std::endl;

			//If we are placing our start tile!
			if (m_placeStart)
			{
				for (auto& tile : map.getTiles())
				{
					if (tile.second->getCollisionBox().intersects(m_collisionBox))
					{
						map.BFS(tile.second->getGridPos());
						break;
					}
				}

			}

			//If we are placing an obstacle
			if (m_placeObstacle)
			{
				for (auto& tile : map.getTiles())
				{
					if (tile.second->getCollisionBox().intersects(m_collisionBox) && !tile.second->isGoal()) //Place if not the start or goal
					{
						tile.second->setAsObstacle(); //Set this tile as an obstacle
						map.BFS(); //Run BFS using the last set start tile
						break;
					}
				}
			}

			if (m_placeGoal)
			{
				for (auto& tile : map.getTiles())
				{
					if (tile.second->getCollisionBox().intersects(m_collisionBox) && !tile.second->isObstacle())
					{
						map.setGoal(*tile.second); //Set this tile as the goal
						break;
					}
				}
			}
		}

		if (m_current.RIGHT_MOUSE && !m_previous.RIGHT_MOUSE)
		{
			std::cout << "Right Mouse click" << std::endl;

			//If we are removing an obstacle
			if (m_placeObstacle)
			{
				for (auto& tile : map.getTiles())
				{
					if (tile.second->getCollisionBox().intersects(m_collisionBox) && tile.second->isObstacle())
					{
						tile.second->resetTile(); //Reset this tile if it is an obstacle
						map.BFS(); //Run BFS using the last set start tile
						break;
					}
				}
			}
		}
	}
}

void InputHandler::draw()
{
	m_renderWin->draw(m_mouseRect);
}

void InputHandler::checkInput()
{
	//Set mouse rectangle position
	m_mouseRect.setPosition(sf::Mouse::getPosition(*m_renderWin).x, sf::Mouse::getPosition(*m_renderWin).y);
	m_collisionBox.left = m_mouseRect.getPosition().x - 2.5f;
	m_collisionBox.top = m_mouseRect.getPosition().y - 2.5f;

	//Set previous as current
	m_previous = m_current;

	//Checking mouse clicks
	m_current.LEFT_MOUSE = sf::Mouse::isButtonPressed(sf::Mouse::Left); //Checking left click		
	m_current.RIGHT_MOUSE = sf::Mouse::isButtonPressed(sf::Mouse::Right); //Checking right click

	//Check key presses
	m_current.ONE_BUTTON = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1); //Checking 1 press
	m_current.TWO_BUTTON = sf::Keyboard::isKeyPressed(sf::Keyboard::Num2); //Checking 2 press
	m_current.THREE_BUTTON = sf::Keyboard::isKeyPressed(sf::Keyboard::Num3); //Checking 3 press
}

void InputHandler::setBooleans(bool placeS, bool placeG, bool placeO)
{
	m_placeStart = placeS;
	m_placeGoal = placeG;
	m_placeObstacle = placeO;
}
