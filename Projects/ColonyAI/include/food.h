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

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	///	\param pEnv Pointer to the Environment the Food is within
	/// \param kPosition A vector with x&y for position
	///
	///////////////////////////////////////////////// 
	Food(Environment * pEnv, const sf::Vector2f kPosition);
};

#endif