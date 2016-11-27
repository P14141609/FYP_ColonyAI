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
	/// \brief 
	///
	/// \param 
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	static sf::Vector2f unitVecFromAngle(const float kfAngle) { return sf::Vector2f(cosf(kfAngle * g_kfDegToRad), sinf(kfAngle * g_kfDegToRad)); }

	/////////////////////////////////////////////////
	///
	/// \brief 
	///
	/// \param 
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	static float angleFromUnitVec(const sf::Vector2f kVector) { return (atan2(kVector.y, kVector.x) / g_kfPi) * 180; }
	
	/////////////////////////////////////////////////
	///
	/// \brief Calculates a normal vector
	///
	/// \param kVector A vector to normalise
	///
	/// \return Returns a normalised vector
	///
	///////////////////////////////////////////////// 
	//static sf::Vector2f normaliseVec(const sf::Vector2f kVector) 
	//{ 
	//	return sf::Vector2f(-kVector.y / magnitude(kVector), kVector.x / magnitude(kVector)); 
	//}
	static sf::Vector2f normaliseVec(const sf::Vector2f kVector)
	{
		float fMag(magnitude(kVector));

		if (fMag != 0)
			return sf::Vector2f(kVector.x / fMag, kVector.y / fMag);
		else
			return sf::Vector2f(0.0f, 0.0f);
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