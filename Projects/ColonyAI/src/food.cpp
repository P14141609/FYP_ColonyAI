/**
@file food.cpp
*/

// Imports
#include "food.h"

// Constructor
Food::Food(Environment * pEnv, const sf::Vector2f kPosition)
{
	// Defines the EntityType
	m_type = FOOD;

	// Casts the incoming pointer to a shared_ptr and assigns it to the member
	m_pEnvironment = std::shared_ptr<Environment>(pEnv);

	// Sets member values to corresponding input
	m_position = kPosition;
	m_fRadius = 3.0f;
}

// Void: Called to draw the Food
void Colonist::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Declares new CircleShape to draw
	sf::CircleShape circle;

	// Sets circle colour: Red RGB for Food
	circle.setFillColor(sf::Color(166, 35, 35, 255));

	// Sets the circle radius to radius member
	circle.setRadius(m_fRadius);
	// Sets the origin to the center of the circle
	circle.setOrigin(sf::Vector2f(m_fRadius, m_fRadius));
	// Sets the circle pos to position member
	circle.setPosition(sf::Vector2f(m_position));

	// Draws circle to target
	target.draw(circle);
}