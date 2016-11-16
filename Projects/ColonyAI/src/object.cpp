/**
@file object.cpp
*/

// Imports
#include "object.h"

// Constructor
Object::Object(const sf::Vector2i kPosition)
{
	m_position = kPosition;
}

// Void: Called to draw the Object
void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const {}