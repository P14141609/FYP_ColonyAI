#pragma once
#ifndef EDITOR_H
#define EDITOR_H

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
/// \brief Class for level creation
/// 
/////////////////////////////////////////////////
class Editor : public sf::Drawable
{
private:

	std::string m_sFilePath; //!< Editor save file
	sf::Vector2u m_size; //!< Editor size
	sf::Color m_colour = sf::Color(0, 150, 0, 255); //!< Editor background colour

	std::vector<std::shared_ptr<Entity>> m_pEntities; //!< Vector of Entity pointers to associated Entities in the Editor
	std::vector<std::shared_ptr<Object>> m_pObjects; //!< Vector of Object pointers to associated Objects in the Editor

	/////////////////////////////////////////////////
	///
	/// \brief Draws the Editor to the RenderTarget
	///
	/// \param target The RenderTarget to draw on
	/// \param states The corresponding RenderStates
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	/// \param ksFilePath The Editor's file path for saving
	/// \param kSize The Editor level size
	///
	///////////////////////////////////////////////// 
	Editor(const std::string ksFilePath, sf::Vector2u kSize);

	/////////////////////////////////////////////////
	///
	/// \brief Saves data to the Editor's file
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void save();

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Editor size
	///
	/// \return The size vector
	///
	///////////////////////////////////////////////// 
	sf::Vector2u getSize() { return m_size; }
};

#endif