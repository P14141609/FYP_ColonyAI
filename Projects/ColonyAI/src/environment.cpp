/**
@file environment.cpp
*/

// Imports
#include "environment.h"
// TEMPORARY
#include "bush.h"
#include "rock.h"
#include "tree.h"

// Constructor
Environment::Environment(const std::string ksFilename)
{
	// TODO

	// TEMPORARY
	m_bgColour = sf::Color(0, 150, 0, 255);

	m_Objects.push_back(std::shared_ptr<Object>(new Bush(sf::Vector2f(300, 150))));
	m_Objects.push_back(std::shared_ptr<Object>(new Rock(sf::Vector2f(300, 300))));
	m_Objects.push_back(std::shared_ptr<Object>(new Tree(sf::Vector2f(300, 450))));
}

// Void: Called to update the environment
void Environment::update(const float kfElapsedTime)
{
	// For every Entity in the Environment
	for (std::shared_ptr<Entity> entity : m_Entities)
	{
		// Updates the Entity
		entity->update(kfElapsedTime);
	}
}

// Void: Called to draw the environment
void Environment::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Clears previous frame and draws the Environment background colour to the target
	target.clear(m_bgColour);

	// For every Entity in the Environment
	for (std::shared_ptr<Entity> entity : m_Entities)
	{
		// Draws Entity to RenderTarget
		entity->draw(target, states);
	}

	// For every Object in the Environment
	for (std::shared_ptr<Object> object : m_Objects)
	{
		// Draws Object to RenderTarget
		object->draw(target, states);
	}
}