/**
@file bush.cpp
*/

// Imports
#include "bush.h"

// Constructor
Bush::Bush(const sf::Vector2f kPosition, const float kfRadius)
{
	// Sets member values to corresponding input
	m_position = kPosition;
	m_fRadius = kfRadius;
}

// Void: Called to draw the Bush
void Bush::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Declares new CircleShape to draw
	sf::CircleShape circle;

	// Sets the origin to the center of the circle
	circle.setOrigin(sf::Vector2f(m_fRadius, m_fRadius));

	// Sets the circle pos to position member
	circle.setPosition(sf::Vector2f(m_position));

	// Sets the circle radius to radius member
	circle.setRadius(m_fRadius);

	// Sets circle colour: Green RGB for bush
	circle.setFillColor(sf::Color(61, 237, 17, 255));

	// Draws circle to target
	target.draw(circle);
}