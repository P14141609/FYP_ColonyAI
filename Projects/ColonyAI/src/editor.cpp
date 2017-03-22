/**
@file editor.cpp
*/

// Imports
#include "editor.h"

// Constructor
Editor::Editor(const std::string ksFilePath, sf::Vector2u kSize)
{
	// Sets member values to corresponding input
	m_sFilePath = ksFilePath;
	m_size = kSize;
}

// Void: Saves data to the Editor's file
void Editor::save()
{
	// Creates the file for saving
	std::ofstream file;
	file.open(m_sFilePath);

	// Passes Environment data into file
	file << "Environment: x" << m_size.x << " y" << m_size.y << std::endl;

	// For all Objects
	for (std::shared_ptr<Object> pObject : m_pObjects)
	{
		// Passes Object data into file
		file << "Object: " 
			<< Utils::formatWord(Object::typeToStr(pObject->getType())) // Converts 'BUSH' to 'Bush'
			<< " x" << pObject->getPosition().x 
			<< " y" << pObject->getPosition().y 
			<< " r" << pObject->getRadius() << std::endl;
	}

	// For all Entities
	for (std::shared_ptr<Entity> pEntity : m_pEntities)
	{
		// Passes Entity data into file
		file << "Entity:"
			<< Utils::formatWord(Entity::typeToStr(pEntity->getType())) // Converts 'COLONIST' to 'Colonist'
			<< " x" << pEntity->getPosition().x
			<< " y" << pEntity->getPosition().y;

		// If Entity is a Colonist
		if (pEntity->getType() == COLONIST)
		{
			// Passes Colonist heading to file
			file << " h" << std::dynamic_pointer_cast<Colonist>(pEntity)->getHeading() << std::endl;
		}
		else
		{
			// Ends file line
			file << std::endl;
		}
	}

	// Closes open file
	file.close();
}

// Void: 
void Editor::placeSelected()
{
	// Draws selected item
	if (m_hand.m_selected == m_hand.SELECTED_BUSH)
	{
		std::shared_ptr<Bush> item = std::shared_ptr<Bush>(new Bush(nullptr, m_hand.m_position, m_hand.m_fRadius));
		m_pObjects.push_back(std::dynamic_pointer_cast<Object>(item));
	}
	else if (m_hand.m_selected == m_hand.SELECTED_ROCK)
	{
		std::shared_ptr<Rock> item = std::shared_ptr<Rock>(new Rock(nullptr, m_hand.m_position, m_hand.m_fRadius));
		m_pObjects.push_back(std::dynamic_pointer_cast<Object>(item));
	}
	else if (m_hand.m_selected == m_hand.SELECTED_TREE)
	{
		std::shared_ptr<Tree> item = std::shared_ptr<Tree>(new Tree(nullptr, m_hand.m_position, m_hand.m_fRadius));
		m_pObjects.push_back(std::dynamic_pointer_cast<Object>(item));
	}
	else if (m_hand.m_selected == m_hand.SELECTED_WATER)
	{
		std::shared_ptr<Water> item = std::shared_ptr<Water>(new Water(nullptr, m_hand.m_position, m_hand.m_fRadius));
		m_pObjects.push_back(std::dynamic_pointer_cast<Object>(item));
	}
	else if (m_hand.m_selected == m_hand.SELECTED_COLONIST)
	{
		std::shared_ptr<Colonist> item = std::shared_ptr<Colonist>(new Colonist(nullptr, m_hand.m_position, m_hand.m_fHeading));
		m_pEntities.push_back(std::dynamic_pointer_cast<Entity>(item));
	}
	else if (m_hand.m_selected == m_hand.SELECTED_FOOD)
	{
		std::shared_ptr<Food> item = std::shared_ptr<Food>(new Food(nullptr, m_hand.m_position));
		m_pEntities.push_back(std::dynamic_pointer_cast<Entity>(item));
	}
}

// Void: Draws the Editor to the RenderTarget
void Editor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// Rectangle shape for drawing
	sf::RectangleShape rectShape;
	rectShape.setSize(sf::Vector2f((float)m_size.x, (float)m_size.y)); // Size of Environment
	rectShape.setFillColor(m_colour);
	// Draws shape to display
	target.draw(rectShape);

	// For every Object in the Environment
	for (std::shared_ptr<Object> pObject : m_pObjects)
	{
		// If Object is not a Tree
		if (pObject->getType() != TREE)
		{
			// Draws Object to RenderTarget
			pObject->draw(target, states);
		}
	}

	// For every Entity in the Environment
	for (std::shared_ptr<Entity> pEntity : m_pEntities)
	{
		// Draws Entity to RenderTarget
		pEntity->draw(target, states);
	}

	// For every Object in the Environment
	for (std::shared_ptr<Object> pObject : m_pObjects)
	{
		// If Object is a Tree
		if (pObject->getType() == TREE)
		{
			// Draws Object to RenderTarget
			pObject->draw(target, states);
		}
	}

	// Draws selected item
	if (m_hand.m_selected == m_hand.SELECTED_BUSH)
	{
		std::shared_ptr<Bush> item = std::shared_ptr<Bush>(new Bush(nullptr, m_hand.m_position, m_hand.m_fRadius));
		std::dynamic_pointer_cast<Object>(item)->draw(target, states);
	}
	else if (m_hand.m_selected == m_hand.SELECTED_ROCK)
	{
		std::shared_ptr<Rock> item = std::shared_ptr<Rock>(new Rock(nullptr, m_hand.m_position, m_hand.m_fRadius));
		std::dynamic_pointer_cast<Object>(item)->draw(target, states);
	}
	else if (m_hand.m_selected == m_hand.SELECTED_TREE)
	{
		std::shared_ptr<Tree> item = std::shared_ptr<Tree>(new Tree(nullptr, m_hand.m_position, m_hand.m_fRadius));
		std::dynamic_pointer_cast<Object>(item)->draw(target, states);
	}
	else if (m_hand.m_selected == m_hand.SELECTED_WATER)
	{
		std::shared_ptr<Water> item = std::shared_ptr<Water>(new Water(nullptr, m_hand.m_position, m_hand.m_fRadius));
		std::dynamic_pointer_cast<Object>(item)->draw(target, states);
	}
	else if (m_hand.m_selected == m_hand.SELECTED_COLONIST)
	{
		std::shared_ptr<Colonist> item = std::shared_ptr<Colonist>(new Colonist(nullptr, m_hand.m_position, m_hand.m_fHeading));
		std::dynamic_pointer_cast<Entity>(item)->draw(target, states);
	}
	else if (m_hand.m_selected == m_hand.SELECTED_FOOD)
	{
		std::shared_ptr<Food> item = std::shared_ptr<Food>(new Food(nullptr, m_hand.m_position));
		std::dynamic_pointer_cast<Entity>(item)->draw(target, states);
	}
}