#ifndef UTILS_H
#define UTILS_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>

// Constant values for calculations
const float g_kfPi = 3.14159f;
const float g_kfDegToRad = g_kfPi / 180;

/////////////////////////////////////////////////
///
/// \brief Class with utilities for calculations
/// 
/////////////////////////////////////////////////
class Utils
{
public:

	/////////////////////////////////////////////////
	///
	/// \brief Calculates a unit vector from a given angle
	///
	/// \param kfAngle An angle to derive a unit vector
	///
	/// \return The unit vector of the angle
	///
	///////////////////////////////////////////////// 
	static sf::Vector2f unitVecFromAngle(const float kfAngle) { return sf::Vector2f(cosf(kfAngle * g_kfDegToRad), sinf(kfAngle * g_kfDegToRad)); }

	/////////////////////////////////////////////////
	///
	/// \brief Calculates an angle from a given unit vector
	///
	/// \param kVector A unit vector to derive an angle
	///
	/// \return The angle of the unit vector
	///
	///////////////////////////////////////////////// 
	static float angleFromUnitVec(const sf::Vector2f kUnitVector) { return (atan2(kUnitVector.y, kUnitVector.x) / g_kfPi) * 180; }
	
	/////////////////////////////////////////////////
	///
	/// \brief Calculates a normal vector
	///
	/// \param kVector A vector to normalise
	///
	/// \return Returns a normalised vector
	///
	///////////////////////////////////////////////// 
	static sf::Vector2f normaliseVec(const sf::Vector2f kVector)
	{
		// If magnitude is not 0: returns the normalised vector
		if (magnitude(kVector) != 0) return sf::Vector2f(kVector.x / magnitude(kVector), kVector.y / magnitude(kVector));
		// Else: return a 0,0 vector
		else return sf::Vector2f(0.0f, 0.0f);
	}

	/////////////////////////////////////////////////
	///
	/// \brief Calculates the magnitude of a vector
	///
	/// \param kVector A vector to derive a magnitude
	///
	/// \return Returns the magnitude of the vector
	///
	///////////////////////////////////////////////// 
	static float magnitude(const sf::Vector2f kVector) { return sqrt(pow(kVector.x, 2) + pow(kVector.y, 2)); }
};

#endif