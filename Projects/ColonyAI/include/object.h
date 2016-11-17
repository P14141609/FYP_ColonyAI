#ifndef OBJECT_H
#define OBJECT_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/////////////////////////////////////////////////
///
/// \brief Abstract base class for objects
/// 
/////////////////////////////////////////////////
class Object
{
private:

protected:

	sf::Vector2f m_position; //!< Holds the Object position

public:

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

};

#endif