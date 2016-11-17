/**
@file tree.cpp
*/

// Imports
#include "tree.h"

// Constructor
Tree::Tree(const sf::Vector2f kPosition)
{
	m_position = kPosition;
}

// Void: Called to draw the Tree
void Tree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// TODO

	// TEMPORARY
	sf::CircleShape shape;
	shape.setFillColor(sf::Color(99, 64, 14, 255));
	shape.setRadius(15.0f);

	shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setPosition(sf::Vector2f(m_position));

	target.draw(shape);

	sf::CircleShape shape2;
	shape2.setFillColor(sf::Color(26, 77, 6, 140));
	shape2.setRadius(65.0f);

	shape2.setOrigin(sf::Vector2f(shape2.getRadius(), shape2.getRadius()));
	shape2.setPosition(m_position);

	target.draw(shape2);
}