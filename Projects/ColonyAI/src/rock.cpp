/**
@file rock.cpp
*/

// Imports
#include "rock.h"

// Constructor
Rock::Rock(const sf::Vector2f kPosition, const float kfRadius)
{
	m_position = kPosition;
	m_fRadius = kfRadius;
}

// Void: Called to draw the Rock
void Rock::draw(sf::RenderTarget& target, sf::RenderStates states) const
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
	circle.setFillColor(sf::Color(133, 133, 133, 255));

	// Draws circle to target
	target.draw(circle);
}