/**
@file bush.cpp
*/

// Imports
#include "bush.h"

// Constructor
Bush::Bush(const sf::Vector2f kPosition)
{
	m_position = kPosition;
}

// Void: Called to draw the Bush
void Bush::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// TODO

	// TEMPORARY
	sf::CircleShape shape;
	shape.setFillColor(sf::Color(61, 237, 17, 255));
	shape.setRadius(27.5f);

	shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setPosition(m_position);

	target.draw(shape);
}