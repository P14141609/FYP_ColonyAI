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

	/////////////////////////////////////////////////
	///
	/// \brief Determines whether a point lies within a given area
	///
	/// \param kPoint Position vector for the point
	/// \param kOrigin The origin of the area
	/// \param kExtents The extents of the area
	///
	/// \return Returns whether the point is in the area
	///
	///////////////////////////////////////////////// 
	static bool pointInArea(const sf::Vector2f kPoint, const sf::Vector2f kOrigin, const sf::Vector2u kExtents)
	{
		// If point.x is less than 0: Out of bounds
		if (kPoint.x < kOrigin.x) return false;

		// If point.y is less than 0: Out of bounds
		if (kPoint.y < kOrigin.y) return false;

		// If point.x is greater than the area.x: Out of bounds
		if (kPoint.x > kOrigin.x + kExtents.x) return false;

		// If point.y is greater than the area.y: Out of bounds
		if (kPoint.y > kOrigin.y + kExtents.y) return false;

		// No False returns called: call return True
		return true;
	}

	/////////////////////////////////////////////////
	///
	/// \brief Determines the float with the lowest value
	///
	/// \param kfA Float for comparison
	/// \param kfB Float for comparison
	///
	/// \return Returns the lowest value
	///
	///////////////////////////////////////////////// 
	static float min(const float kfA, const float kfB) { if (kfA <= kfB) { return kfA; } return kfB; }

	/////////////////////////////////////////////////
	///
	/// \brief Determines the float with the highest value
	///
	/// \param kfA Float for comparison
	/// \param kfB Float for comparison
	///
	/// \return Returns the highest value
	///
	///////////////////////////////////////////////// 
	static float max(const float kfA, const float kfB) { if (kfA >= kfB) { return kfA; } return kfB; }

	/////////////////////////////////////////////////
	///
	/// \brief Inverts a positive number to negative and vice versa
	///
	/// \param kfNumber Float to invert
	///
	/// \return Returns the inverted value
	///
	///////////////////////////////////////////////// 
	static float invert(const float kfNumber) { return kfNumber * -1; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns a value bound to a given min and max
	///
	/// \param fNumber Value to bind
	/// \param kfMin Lower binding
	/// \param kfMax Upper binding
	///
	/// \return Returns the number now bound to the min and max
	///
	///////////////////////////////////////////////// 
	static float bindNum(float fNumber, const float kfMin, const float kfMax)
	{
		// While the number is below the lower bound
		while (fNumber < kfMin) fNumber += kfMax; // Add the upper bound to the number
		// While the number is above the upper bound
		while (fNumber > kfMax) fNumber -= kfMax; // Subtract the upper bound from the number

		// Returns the newly bound number
		return fNumber;
	}
};

#endif