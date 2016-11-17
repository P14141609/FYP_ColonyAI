#ifndef ENTITY_H
#define ENTITY_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/////////////////////////////////////////////////
///
/// \brief Abstract base class for entities
/// 
/////////////////////////////////////////////////
class Entity
{
private:
	
protected:
	
	sf::Vector2f m_position; //!< Holds the Entity position
	float m_fHeading; //!< Holds the Entity heading in degrees

public:

	/////////////////////////////////////////////////
	///
	/// \brief Purely virtual: Draws the Entity to the RenderTarget
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
	/// \brief Purely virtual: Updates the Entity with elapsed time
	///
	/// \param kfElapsedTime The time passed since last update in seconds
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	virtual void update(const float kfElapsedTime) = 0;
};

#endif