#ifndef ENTITY_H
#define ENTITY_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "environment.h"

class Environment;

/////////////////////////////////////////////////
///
/// \brief Abstract base class for entities
/// 
/////////////////////////////////////////////////
class Entity
{
private:
	
protected:

	std::shared_ptr<Environment> m_pEnvironment; //!< Pointer to the Environment the Entity is within

	sf::Vector2f m_position; //!< Holds the Entity position
	float m_fRadius; //!< Holds the radial size of the Entity

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

	/////////////////////////////////////////////////
	///
	/// \brief Sets the Entity position
	///
	/// \param kPosition A new position vector
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setPosition(const sf::Vector2f kPosition) { m_position = kPosition; };

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Entity position
	///
	/// \return The Entity position vector
	///
	///////////////////////////////////////////////// 
	sf::Vector2f getPosition() { return m_position; };

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Entity radius
	///
	/// \return The Entity radial size
	///
	///////////////////////////////////////////////// 
	float getRadius() { return m_fRadius; };

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Environment the Entity is within
	///
	/// \return A pointer to the Environment the Entity is within
	///
	///////////////////////////////////////////////// 
	std::shared_ptr<Environment> getEnvironment() { return m_pEnvironment; };
};

#endif