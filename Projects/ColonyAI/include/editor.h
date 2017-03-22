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

//!< Struct that holds selection data
struct Hand
{
	sf::Vector2f m_position; //!< In-world coordinates
	float m_fRadius = 10.0f; //!< Radial size
	float m_fHeading = 0.0f; //!< Heading in degrees

	enum selectedItem { SELECTED_BUSH, SELECTED_ROCK, SELECTED_TREE, SELECTED_WATER, SELECTED_COLONIST, SELECTED_FOOD }; //!< Enum for possible selected items in Editor
	selectedItem m_selected = SELECTED_BUSH; //!< The item that is selected
};

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

	Hand m_hand; //!< Hand that holds selection pos, radius, heading and type

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
	/// \brief Pushes item in Hand onto the level
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void placeSelected();

	/////////////////////////////////////////////////
	///
	/// \brief Cycles the current selected item 
	///
	/// \param kiDirection Cycle up or down
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void cycleSelected(const int kiDirection);

	/////////////////////////////////////////////////
	///
	/// \brief Sets the Hand position
	///
	/// \param kPosition A new position vector
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setHandPos(const sf::Vector2f kPosition) { m_hand.m_position = kPosition; }

	/////////////////////////////////////////////////
	///
	/// \brief Sets the Hand heading
	///
	/// \param A new heading angle
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setHandHeading(const float kfHeading) { m_hand.m_fHeading = Utils::bindNum(kfHeading, 0, 360); }

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Hand heading
	///
	/// \return Heading of the selection in degrees
	///
	///////////////////////////////////////////////// 
	float getHandHeading() { return m_hand.m_fHeading; }

	/////////////////////////////////////////////////
	///
	/// \brief Sets the Hand radial size
	///
	/// \param kfRadius A new radius
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setHandRadius(const float kfRadius) { m_hand.m_fRadius = kfRadius; }

	/////////////////////////////////////////////////
	///
	/// \brief Gets the radial size of the Hand
	///
	/// \return Radial size of the selection
	///
	///////////////////////////////////////////////// 
	float getHandRadius() { return m_hand.m_fRadius; }

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