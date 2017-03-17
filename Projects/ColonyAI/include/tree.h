#pragma once
#ifndef TREE_H
#define TREE_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "object.h"

/////////////////////////////////////////////////
///
/// \brief Subclass for Tree Objects
/// 
/////////////////////////////////////////////////
class Tree : public Object
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
	///	\param pEnv Pointer to the Environment the Tree is within
	/// \param kPosition A vector with in-world position
	/// \param kfRadius A float for the radius
	///
	///////////////////////////////////////////////// 
	Tree(std::shared_ptr<Environment> pEnv, const sf::Vector2f kPosition, const float kfRadius);
};

#endif