#ifndef UTILS_H
#define UTILS_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>

// Global boolean for debugging
bool g_bDebugging = false; //!< Used to display debug info or not

// Constant values for calculations
const float g_kfPi = 3.14159f; //!< Value of PI
const float g_kfDegToRad = g_kfPi / 180; //!< Used to convert degrees to radians

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
	/// \brief Calculates a unit vector
	///
	/// \param kVector A vector to normalise
	///
	/// \return Returns a unit vector of the given vector
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
	/// \brief Calculates a vector from a given angle
	///
	/// \param kfAngle An angle to derive a vector
	///
	/// \return The vector of the angle
	///
	///////////////////////////////////////////////// 
	static sf::Vector2f vecFromAngle(const float kfAngle) { return sf::Vector2f(cosf(kfAngle), sinf(kfAngle)); }

	/////////////////////////////////////////////////
	///
	/// \brief Calculates a unit vector from a given angle
	///
	/// \param kfAngle An angle to derive a unit vector
	///
	/// \return The unit vector of the angle
	///
	///////////////////////////////////////////////// 
	static sf::Vector2f unitVecFromAngle(const float kfAngle) { return normaliseVec(vecFromAngle(kfAngle)); }

	/////////////////////////////////////////////////
	///
	/// \brief Calculates an angle from a given vector
	///
	/// \param kVector A vector to derive an angle
	///
	/// \return The angle of the vector
	///
	///////////////////////////////////////////////// 
	static float angleFromVec(const sf::Vector2f kVector) { return (atan2(kVector.y, kVector.x) / g_kfPi) * 180; }

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
	/// \brief Determines the long with the lowest value
	///
	/// \param klA Long for comparison
	/// \param klB Long for comparison
	///
	/// \return Returns the lowest value
	///
	///////////////////////////////////////////////// 
	static long min(const long klA, const long klB) { if (klA <= klB) { return klA; } return klB; }

	/////////////////////////////////////////////////
	///
	/// \brief Determines the long with the highest value
	///
	/// \param klA Long for comparison
	/// \param klB Long for comparison
	///
	/// \return Returns the highest value
	///
	///////////////////////////////////////////////// 
	static long max(const long klA, const long klB) { if (klA >= klB) { return klA; } return klB; }

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

	/////////////////////////////////////////////////
	///
	/// \brief Returns the boolean value in a string form
	///
	/// \param kbValue Boolean to convert
	///
	/// \return Returns a string of "true" or "false"
	///
	///////////////////////////////////////////////// 
	static std::string boolToStr(const bool kbValue)
	{
		if (kbValue) return "true";
		return "false";
	}

	/////////////////////////////////////////////////
	///
	/// \brief Returns the string completely lowercase
	///
	/// \param ksString String to modify
	///
	/// \return Returns a string in lowercase
	///
	///////////////////////////////////////////////// 
	static std::string toLowercase(const std::string ksString)
	{
		// Declares 'locale' for tolower to function
		std::locale loc;
		// Declares string for result
		std::string sResult;

		// For all chars in string
		for (unsigned int i = 0; i < ksString.length(); i++)
		{
			// Adds the char to the result in lowercase
			sResult += std::tolower(ksString[i], loc);
		}
			
		// Returns resultant string
		return sResult;
	}

	/////////////////////////////////////////////////
	///
	/// \brief Returns the string completely uppercase
	///
	/// \param ksString String to modify
	///
	/// \return Returns a string in uppercase
	///
	///////////////////////////////////////////////// 
	static std::string toUppercase(const std::string ksString)
	{
		// Declares 'locale' for toupper to function
		std::locale loc;
		// Declares string for result
		std::string sResult;

		// For all chars in string
		for (unsigned int i = 0; i < ksString.length(); i++)
		{
			// Adds the char to the result in uppercase
			sResult += std::toupper(ksString[i], loc);
		}

		// Returns resultant string
		return sResult;
	}
};

#endif