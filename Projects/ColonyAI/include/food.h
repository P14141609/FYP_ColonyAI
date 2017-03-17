#pragma once
#ifndef FOOD_H
#define FOOD_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "entity.h"

/////////////////////////////////////////////////
///
/// \brief Subclass for Food Entities
/// 
/////////////////////////////////////////////////
class Food : public Entity
{
private:

	/////////////////////////////////////////////////
	///
	/// \brief Draws the Food to the RenderTarget
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
	/// \brief Updates the Food with elapsed time
	///
	/// \param kfElapsedTime The time passed since last update in seconds
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void update(const float kfElapsedTime);

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	///	\param pEnv Shared Pointer to the Environment the Food is within
	/// \param kPosition A vector with in-world position
	///
	///////////////////////////////////////////////// 
	Food(std::shared_ptr<Environment> pEnv, const sf::Vector2f kPosition);
};

#endif