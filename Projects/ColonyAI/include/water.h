#ifndef WATER_H
#define WATER_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "object.h"

/////////////////////////////////////////////////
///
/// \brief Subclass for Water Objects
/// 
/////////////////////////////////////////////////
class Water : public Object
{
private:

	/////////////////////////////////////////////////
	///
	/// \brief Draws the Water to the RenderTarget
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
	/// \param kPosition A vector with x&y for position
	/// \param kfRadius A float for the radius
	///
	///////////////////////////////////////////////// 
	Water(const sf::Vector2f kPosition, const float kfRadius);
};

#endif