/**
@file water.cpp
*/

// Imports
#include "water.h"

// Constructor
Water::Water(std::shared_ptr<Environment> pEnv, const sf::Vector2f kPosition, const float kfRadius)
{
	// Defines the ObjectType
	m_type = WATER;

	// Sets member values to corresponding input
	m_pEnvironment = pEnv;
	m_position = kPosition;
	m_fRadius = kfRadius;
}

// Void: Called to draw the Rock
void Water::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Declares new CircleShape to draw
	sf::CircleShape circle;

	// Sets the origin to the center of the circle
	circle.setOrigin(sf::Vector2f(m_fRadius, m_fRadius));

	// Sets the circle pos to position member
	circle.setPosition(sf::Vector2f(m_position));

	// Sets the circle radius to radius member
	circle.setRadius(m_fRadius);

	// Sets circle colour: Grey RGB for rock
	circle.setFillColor(sf::Color(0, 0, 255, 255));

	// Draws circle to target
	target.draw(circle);
}