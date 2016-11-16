#ifndef TREE_H
#define TREE_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "object.h"

/////////////////////////////////////////////////
///
/// \brief Subclass for Tree Objects
/// 
/////////////////////////////////////////////////
class Tree : public Object, public sf::Drawable
{
private:

	/////////////////////////////////////////////////
	///
	/// \brief Draws the Tree to the RenderTarget
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
	/// \param position A vector with x&y for position
	///
	///////////////////////////////////////////////// 
	Tree(const sf::Vector2i kPosition);
};

#endif