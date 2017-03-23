#pragma once
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "object.h"
#include "bush.h"
#include "rock.h"
#include "tree.h"
#include "water.h"

#include "entity.h"
#include "colonist.h"

class Entity;
class Object;

/////////////////////////////////////////////////
///
/// \brief Class for level setup, interation and display
/// 
/////////////////////////////////////////////////
class Environment: public sf::Drawable
{
private:

	sf::Vector2u m_size; //!< Environment size
	sf::Color m_colour = sf::Color(0, 150, 0, 255); //!< Environment background colour

	std::vector<std::shared_ptr<Entity>> m_pEntities; //!< Vector of Entity pointers to associated Entities in the Environment
	std::vector<std::shared_ptr<Object>> m_pObjects; //!< Vector of Object pointers to associated Objects in the Environment

	/////////////////////////////////////////////////
	///
	/// \brief Draws the Environment to the RenderTarget
	///
	/// \param target The RenderTarget to draw on
	/// \param states The corresponding RenderStates
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/////////////////////////////////////////////////
	///
	/// \brief Reads a Environement file line
	///
	/// \param iss Line input string stream to read
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void readEnvLine(std::istringstream& iss);

	/////////////////////////////////////////////////
	///
	/// \brief Reads an Object file line
	///
	/// \param iss Line input string stream to read
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void readObjectLine(std::istringstream& iss);

	/////////////////////////////////////////////////
	///
	/// \brief Reads an Entity file line
	///
	/// \param iss Line input string stream to read
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void readEntityLine(std::istringstream& iss);

protected:
	
public:

	/////////////////////////////////////////////////
	///
	/// \brief Default constructor
	///
	///////////////////////////////////////////////// 
	Environment() {}

	/////////////////////////////////////////////////
	///
	/// \brief Loads Env data from a file
	///
	/// \param ksFilePath The Env file path to load
	///
	/// \return Whether the Environment was loaded successfully
	///
	///////////////////////////////////////////////// 
	bool loadFromFile(const std::string ksFilePath);

	/////////////////////////////////////////////////
	///
	/// \brief Updates the Environment and its contents with elapsed time
	///
	/// \param kfElapsedTime The time passed since last update in seconds
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void update(const float kfElapsedTime);

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Environment size
	///
	/// \return The size vector
	///
	///////////////////////////////////////////////// 
	sf::Vector2u getSize() { return m_size; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Environment Object member
	///
	/// \return The vector of Objects
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Object>> getObjects() { return m_pObjects; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Environment Object member
	///
	/// \return Pointer to the Object vector
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Object>>* getObjectVec() { return &m_pObjects; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Environment Entity member
	///
	/// \return The vector of Entities
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Entity>> getEntities() { return m_pEntities; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Environment Entity member
	///
	/// \return Pointer to the Entity vector
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Entity>>* getEntityVec() { return &m_pEntities; }
};

#endif