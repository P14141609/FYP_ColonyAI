/**
@file colonist.cpp
*/

// Imports
#include "colonist.h"

// Constructor
Colonist::Colonist(const sf::Vector2f kPosition)
{
	m_position = kPosition; // Sets position with input
	m_state = IDLE; // Sets Colonist state to a default state: IDLE
}

// Void: Called to update the Colonist
void Colonist::update(const float kfElapsedTime)
{
	switch (m_state)
	{
		case IDLE: // TODO
		{

		} break;

		case EXPLORE: // TODO
		{

		} break;
		
		default: m_state = IDLE; break; // No valid state found: set IDLE;
	}
}

// Void: Called to draw the Colonist
void Colonist::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Declares new CircleShape to draw
	sf::CircleShape circle;

	// Sets the origin to the center of the circle
	circle.setOrigin(sf::Vector2f(m_fRadius, m_fRadius));

	// Sets the circle pos to position member
	circle.setPosition(sf::Vector2f(m_position));

	// Sets the circle radius to radius member
	circle.setRadius(m_fRadius);

	// Sets circle colour: Black RGB for Colonist
	circle.setFillColor(sf::Color(0, 0, 0, 255));

	// Draws circle to target
	target.draw(circle);
}