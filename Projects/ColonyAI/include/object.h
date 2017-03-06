#ifndef OBJECT_H
#define OBJECT_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

enum ObjectType { BUSH, ROCK, TREE }; //!< Enum for Object types

/////////////////////////////////////////////////
///
/// \brief Abstract base class for objects
/// 
/////////////////////////////////////////////////
class Object
{
private:

protected:

	ObjectType m_type; //!< Object type

	sf::Vector2f m_position; //!< Object position
	float m_fRadius; //!< Radial size of the Object

public:

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Object type
	///
	/// \return The Object type
	///
	///////////////////////////////////////////////// 
	ObjectType getType() { return m_type; };

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Object position
	///
	/// \return The Object position vector
	///
	///////////////////////////////////////////////// 
	sf::Vector2f getPosition() { return m_position; };

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Object radius
	///
	/// \return The Object radial size
	///
	///////////////////////////////////////////////// 
	float getRadius() { return m_fRadius; };

	/////////////////////////////////////////////////
	///
	/// \brief Purely virtual: Draws the Object to the RenderTarget
	///
	/// \param target The RenderTarget to draw on
	/// \param states The corresponding RenderStates
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	/////////////////////////////////////////////////
	///
	/// \brief Converts an ObjectType to a string
	///
	/// \return The Object type in string form
	///
	///////////////////////////////////////////////// 
	static std::string typeToStr(const ObjectType kType)
	{
		if (kType == BUSH) return "bush";
		if (kType == ROCK) return "rock";
		if (kType == TREE) return "tree";
		return "ERROR";
	};
};

#endif