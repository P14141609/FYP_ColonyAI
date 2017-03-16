#pragma once
#ifndef BUSH_H
#define BUSH_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "object.h"
#include "environment.h"
#include "utils.h"
#include "food.h"

/////////////////////////////////////////////////
///
/// \brief Subclass for Bush Objects
/// 
/////////////////////////////////////////////////
class Bush : public Object
{
private:

	float m_fGrowth = 0.0f; //!< Time the current Food has been growing
	float m_fGrowthLimit = 0.0f; //!< Time at which Food will be produced

	/////////////////////////////////////////////////
	///
	/// \brief Draws the Bush to the RenderTarget
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
	///	\param pEnv Pointer to the Environment the Bush is within
	/// \param kPosition A vector with x&y for position
	/// \param kfRadius A float for the radius
	///
	///////////////////////////////////////////////// 
	Bush(std::shared_ptr<Environment> pEnv, const sf::Vector2f kPosition, const float kfRadius);

	/////////////////////////////////////////////////
	///
	/// \brief Updates the Bush with elapsed time
	///
	/// \param kfElapsedTime The time passed since last update in seconds
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void update(const float kfElapsedTime);
};

#endif