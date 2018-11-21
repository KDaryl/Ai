#include "Visualiser.h"

Visualiser::Visualiser(sf::Vector2f pos, std::vector<Tile*>& pathTaken)
{
	m_position = pos;
	m_circle.setRadius(10);
	m_circle.setFillColor(sf::Color::Magenta);
	m_circle.setOrigin(m_circle.getRadius(), m_circle.getRadius());
	m_circle.setPosition(m_position);
	m_index = pathTaken.size() - 1;
	m_pathTakenPtr = &pathTaken; //Set the path ptr
	m_dest = m_pathTakenPtr->at(m_index)->getPos(); //Destination is the begining of our 

	m_clock.restart(); //Start the clock

	m_reachedPath = false;
}

void Visualiser::update()
{
	//Figure out which destination we are to move to
	if (m_pathTakenPtr->size() != m_pathLine.size() && distance(m_pathTakenPtr->at(m_index)->getPos(), m_position) < 2)
	{
		//Add a line from the previous point to the current position
		m_pathLine.push_back(m_pathTakenPtr->at(m_index)->getPos());
		m_index--;
		if(m_index > -1)
			m_dest = m_pathTakenPtr->at(m_index)->getPos(); //Set the next destination
	}

	//Move to destination
	m_position = lerp(m_position, m_dest, 0.075f);

	if (!m_reachedPath && m_index == -1)
	{
		if (distance(m_position, m_dest) < 2)
		{
			m_reachedPath = true;
			std::cout << "Visualiser has reached the goal" << std::endl;
		}
	}

	m_circle.setPosition(m_position); //Set the position to the circle to the newly lerped position
	m_clock.restart(); //Reset the clock
}

void Visualiser::draw(sf::RenderWindow & win)
{
	m_lines.clear();

	//Create all of the lines for the path that we have followed so far
	for (int i = 0; i < m_pathLine.size(); i++)
	{
		//If we have not reache dthe bounds of th evector, make the lines
		if (i + 1 <= m_pathLine.size() - 1)
		{
			Line line(m_pathLine.at(i), m_pathLine.at(i+1));
			line.setColor(sf::Color::Magenta);
			m_lines.push_back(line);
		}
	}

	//Draw all of the lines
	for (auto& line : m_lines)
	{
		line.draw(win, sf::RenderStates::Default);
	}

	//Draw a line from the previous visited tile to our circle
	Line currentL(m_position, m_pathTakenPtr->at(m_index + 1)->getPos());
	currentL.setColor(sf::Color::Magenta);
	currentL.draw(win, sf::RenderStates::Default);

	win.draw(m_circle);
}

sf::Vector2f Visualiser::lerp(sf::Vector2f a, sf::Vector2f b, float speed)
{
	sf::Vector2f v;

	v.x = a.x + ((b.x - a.x) * speed);
	v.y = a.y + ((b.y - a.y) * speed);

	return v;
}

double Visualiser::distance(sf::Vector2f a, sf::Vector2f b)
{
	return sqrt(pow(a.x - b.x,2) + pow(a.y - b.y,2));
}
