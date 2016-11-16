#ifndef OBJECT_H
#define OBJECT_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/////////////////////////////////////////////////
///
/// \brief Base class for objects
/// 
/////////////////////////////////////////////////
class Object : public sf::Drawable
{
private:

	/////////////////////////////////////////////////
	///
	/// \brief Draws the Object to the RenderTarget
	///
	/// \param target The RenderTarget to draw on
	/// \param states The corresponding RenderStates
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:

	sf::Vector2i m_position; //!< Holds the Object position

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	/// \param position A vector with x&y for position
	///
	///////////////////////////////////////////////// 
	Object(const sf::Vector2i kPosition);
};

#endif