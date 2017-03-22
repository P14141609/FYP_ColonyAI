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
	sf::Vector2f m_position; //!< 
	float m_fRadius = 1.0f; //!< 
	float m_fHeading = 0.0f; //!< 

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
	/// \brief 
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void placeSelected();

	/////////////////////////////////////////////////
	///
	/// \brief 
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void cycleSelected()
	{

	}

	/////////////////////////////////////////////////
	///
	/// \brief 
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setHandPos(const sf::Vector2f kPosition) { m_hand.m_position = kPosition; }

	/////////////////////////////////////////////////
	///
	/// \brief 
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setHandHeading(const float kfRadius) { m_hand.m_fHeading = Utils::bindNum(kfRadius, 0, 360); }

	/////////////////////////////////////////////////
	///
	/// \brief 
	///
	/// \return Radius of the selection
	///
	///////////////////////////////////////////////// 
	float getHandHeading() { return m_hand.m_fHeading; }

	/////////////////////////////////////////////////
	///
	/// \brief 
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setHandRadius(const float kfRadius) { m_hand.m_fRadius = kfRadius; }

	/////////////////////////////////////////////////
	///
	/// \brief 
	///
	/// \return Radius of the selection
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