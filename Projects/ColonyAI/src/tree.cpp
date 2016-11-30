/**
@file tree.cpp
*/

// Imports
#include "tree.h"

// Constructor
Tree::Tree(const sf::Vector2f kPosition, const float kfRadius)
{
	// Sets member values to corresponding input
	m_position = kPosition;
	m_fRadius = kfRadius;
}

// Void: Called to draw the Tree
void Tree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Declares new CircleShape to draw
	sf::CircleShape circle;

	// Sets the origin to the center of the circle
	circle.setOrigin(sf::Vector2f(m_fRadius, m_fRadius));

	// Sets the circle pos to position member
	circle.setPosition(sf::Vector2f(m_position));

	// Sets the circle radius to radius member
	circle.setRadius(m_fRadius);

	// Sets circle colour: Brown RGB for trunk
	circle.setFillColor(sf::Color(99, 64, 14, 255));

	// Draws circle to target
	target.draw(circle);

	// Sets circle colour: Green RGB for leaves
	circle.setFillColor(sf::Color(26, 77, 6, 140));

	// Defines a new radius for the leaves with an arbitrary muliplier
	float leafRadius = m_fRadius*4.5f;

	// Sets the circle radius to new radius
	circle.setRadius(leafRadius);

	// Sets the origin to the center of the newly expanded circle
	circle.setOrigin(sf::Vector2f(leafRadius, leafRadius));

	// Draws circle to target
	target.draw(circle);
}