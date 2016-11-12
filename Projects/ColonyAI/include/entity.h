#ifndef ENTITY_H
#define ENTITY_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/////////////////////////////////////////////////
///
/// \brief Base class for entities with sharing utilities
/// 
/////////////////////////////////////////////////
class Entity: public sf::Drawable
{
private:
	
	/////////////////////////////////////////////////
	///
	/// \brief Draws the Entity to the RenderTarget
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
	Entity() {};

	/////////////////////////////////////////////////
	///
	/// \brief Updates the Entity with elapsed time
	///
	/// \param kfElapsedTime The time passed since last update in seconds
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void update(const float kfElapsedTime);
};

#endif