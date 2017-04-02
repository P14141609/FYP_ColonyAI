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
		file << "Entity: "
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

	sf::err() << "[EDITOR] Environment saved to file: " << m_sFilePath << std::endl;

	// Closes open file
	file.close();
}

// Void: Pushes item in Hand onto the level
void Editor::placeSelected()
{
	// Places selected item into the level
	if (m_hand.m_selected == Hand::SELECTED_BUSH)
	{
		std::shared_ptr<Bush> item = std::shared_ptr<Bush>(new Bush(nullptr, m_hand.m_position, m_hand.m_fRadius));
		m_pObjects.push_back(std::dynamic_pointer_cast<Object>(item));
		m_placeHistory.push_back(Hand::SELECTED_BUSH);

		sf::err() << "[EDITOR] Bush placed: x(" << m_hand.m_position.x << ") y(" << m_hand.m_position.y << ") r(" << m_hand.m_fRadius << ")" << std::endl;
	}
	else if (m_hand.m_selected == Hand::SELECTED_ROCK)
	{
		std::shared_ptr<Rock> item = std::shared_ptr<Rock>(new Rock(nullptr, m_hand.m_position, m_hand.m_fRadius));
		m_pObjects.push_back(std::dynamic_pointer_cast<Object>(item));
		m_placeHistory.push_back(Hand::SELECTED_ROCK);

		sf::err() << "[EDITOR] Rock placed: x(" << m_hand.m_position.x << ") y(" << m_hand.m_position.y << ") r(" << m_hand.m_fRadius << ")" << std::endl;
	}
	else if (m_hand.m_selected == Hand::SELECTED_TREE)
	{
		std::shared_ptr<Tree> item = std::shared_ptr<Tree>(new Tree(nullptr, m_hand.m_position, m_hand.m_fRadius));
		m_pObjects.push_back(std::dynamic_pointer_cast<Object>(item));
		m_placeHistory.push_back(Hand::SELECTED_TREE);

		sf::err() << "[EDITOR] Tree placed: x(" << m_hand.m_position.x << ") y(" << m_hand.m_position.y << ") r(" << m_hand.m_fRadius << ")" << std::endl;
	}
	else if (m_hand.m_selected == Hand::SELECTED_WATER)
	{
		std::shared_ptr<Water> item = std::shared_ptr<Water>(new Water(nullptr, m_hand.m_position, m_hand.m_fRadius));
		m_pObjects.push_back(std::dynamic_pointer_cast<Object>(item));
		m_placeHistory.push_back(Hand::SELECTED_WATER);

		sf::err() << "[EDITOR] Water placed: x(" << m_hand.m_position.x << ") y(" << m_hand.m_position.y << ") r(" << m_hand.m_fRadius << ")" << std::endl;
	}
	else if (m_hand.m_selected == Hand::SELECTED_COLONIST)
	{
		std::shared_ptr<Colonist> item = std::shared_ptr<Colonist>(new Colonist(nullptr, m_hand.m_position, m_hand.m_fHeading));
		m_pEntities.push_back(std::dynamic_pointer_cast<Entity>(item));
		m_placeHistory.push_back(Hand::SELECTED_COLONIST);

		sf::err() << "[EDITOR] Colonist placed: x(" << m_hand.m_position.x << ") y(" << m_hand.m_position.y << ") h(" << m_hand.m_fHeading << ")" << std::endl;
	}
	else if (m_hand.m_selected == Hand::SELECTED_FOOD)
	{
		std::shared_ptr<Food> item = std::shared_ptr<Food>(new Food(nullptr, m_hand.m_position));
		m_pEntities.push_back(std::dynamic_pointer_cast<Entity>(item));
		m_placeHistory.push_back(Hand::SELECTED_FOOD);

		sf::err() << "[EDITOR] Food placed: x(" << m_hand.m_position.x << ") y(" << m_hand.m_position.y << ")" << std::endl;
	}
}

// Void: Removes the latest placed item
void Editor::undoPlace()
{
	if (!m_placeHistory.empty())
	{
		if ((m_placeHistory.back() == Hand::SELECTED_COLONIST) || (m_placeHistory.back() == Hand::SELECTED_FOOD))
		{
			m_pEntities.pop_back();
		}
		else
		{
			m_pObjects.pop_back();
		}

		sf::err() << "[EDITOR] Undone last place: " << Utils::formatWord(m_hand.selectToStr(m_placeHistory.back())) << std::endl;
		m_placeHistory.pop_back();
	}
	else
	{
		sf::err() << "[EDITOR] Nothing to undo" << std::endl;
	}
}

// Void: Cycles the current selected item
void Editor::cycleSelected(const int kiDirection)
{ 
	if (kiDirection == 1)
	{
		// Selects new item
		if (m_hand.m_selected == Hand::SELECTED_BUSH) m_hand.m_selected = Hand::SELECTED_FOOD;
		else if (m_hand.m_selected == Hand::SELECTED_ROCK) m_hand.m_selected = Hand::SELECTED_BUSH;
		else if (m_hand.m_selected == Hand::SELECTED_TREE) m_hand.m_selected = Hand::SELECTED_ROCK;
		else if (m_hand.m_selected == Hand::SELECTED_WATER) m_hand.m_selected = Hand::SELECTED_TREE;
		else if (m_hand.m_selected == Hand::SELECTED_COLONIST) m_hand.m_selected = Hand::SELECTED_WATER;
		else if (m_hand.m_selected == Hand::SELECTED_FOOD) m_hand.m_selected = Hand::SELECTED_COLONIST;
	}
	else if (kiDirection == -1)
	{
		// Selects new item
		if (m_hand.m_selected == Hand::SELECTED_BUSH) m_hand.m_selected = Hand::SELECTED_ROCK;
		else if (m_hand.m_selected == Hand::SELECTED_ROCK) m_hand.m_selected = Hand::SELECTED_TREE;
		else if (m_hand.m_selected == Hand::SELECTED_TREE) m_hand.m_selected = Hand::SELECTED_WATER;
		else if (m_hand.m_selected == Hand::SELECTED_WATER) m_hand.m_selected = Hand::SELECTED_COLONIST;
		else if (m_hand.m_selected == Hand::SELECTED_COLONIST) m_hand.m_selected = Hand::SELECTED_FOOD;
		else if (m_hand.m_selected == Hand::SELECTED_FOOD) m_hand.m_selected = Hand::SELECTED_BUSH;
	}

	sf::err() << "[EDITOR] Selection set to: " << Utils::formatWord(m_hand.selectToStr(m_hand.m_selected)) << std::endl;
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

		// If Entity is a Colonist
		if (pEntity->getType() == COLONIST)
		{
			// Casts Entity to Colonist object
			std::shared_ptr<Colonist> pColonist = std::dynamic_pointer_cast<Colonist>(pEntity);

			// Declares line and colour
			sf::Vertex line[2];
			sf::Color colour = sf::Color(0, 0, 0, 255);

			// Sets the first point of the line at the Colonist position
			line[0] = sf::Vertex(pColonist->getPosition(), colour);
			// Sets the second point of the line infront of the Colonist based on heading
			line[1] = sf::Vertex(pColonist->getPosition() + (Utils::unitVecFromAngle(pColonist->getHeading()) * (pColonist->getRadius()*2.0f)), colour);

			// Draws the line to target
			target.draw(line, 2, sf::Lines);
		}
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

		// Declares line and colour
		sf::Vertex line[2];
		sf::Color colour = sf::Color(0, 0, 0, 255);

		// Sets the first point of the line at the Colonist position
		line[0] = sf::Vertex(item->getPosition(), colour);
		// Sets the second point of the line infront of the Colonist based on heading
		line[1] = sf::Vertex(item->getPosition() + (Utils::unitVecFromAngle(item->getHeading()) * (item->getRadius()*2.0f)), colour);

		// Draws the line to target
		target.draw(line, 2, sf::Lines);
	}
	else if (m_hand.m_selected == m_hand.SELECTED_FOOD)
	{
		std::shared_ptr<Food> item = std::shared_ptr<Food>(new Food(nullptr, m_hand.m_position));
		std::dynamic_pointer_cast<Entity>(item)->draw(target, states);
	}
}