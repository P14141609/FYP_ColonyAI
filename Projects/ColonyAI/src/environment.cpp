/**
@file environment.cpp
*/

// Imports
#include "environment.h"

// Constructor
Environment::Environment(const std::string ksFilename)
{
	// TODO

	// TEMPORARY
	m_bgColour = sf::Color(0, 150, 0, 255);
}

// Void: Called to update the environment
void Environment::update(const float kfElapsedTime)
{
	// For every Entity in the Environment
	for (Entity entity : m_Entities)
	{
		// Updates the Entity
		entity.update(kfElapsedTime);
	}
}

// Void: Called to draw the environment
void Environment::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Clears previous frame and draws the Environment background colour to the target
	target.clear(m_bgColour);

	// For every Entity in the Environment
	for (Entity entity:m_Entities)
	{
		// Draws Entity to RenderTarget
		target.draw(entity);
	}

	// For every Object in the Environment
	for (Object object : m_Objects)
	{
		// Draws Object to RenderTarget
		target.draw(object);
	}
}