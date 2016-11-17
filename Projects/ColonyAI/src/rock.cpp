/**
@file rock.cpp
*/

// Imports
#include "rock.h"

// Constructor
Rock::Rock(const sf::Vector2f kPosition)
{
	m_position = kPosition;
}

// Void: Called to draw the Rock
void Rock::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// TODO

	// TEMPORARY
	sf::CircleShape shape;
	shape.setFillColor(sf::Color(133, 133, 133, 255));
	shape.setRadius(20.0f);

	shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setPosition(m_position);

	target.draw(shape);
}