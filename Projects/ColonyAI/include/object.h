#ifndef OBJECT_H
#define OBJECT_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/////////////////////////////////////////////////
///
/// \brief Base class for objects for sharing utilities
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

public:

	/////////////////////////////////////////////////
	///
	/// \brief Default constructor
	///
	///////////////////////////////////////////////// 
	Object() {};
};

#endif