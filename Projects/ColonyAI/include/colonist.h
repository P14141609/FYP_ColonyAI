#ifndef COLONIST_H
#define COLONIST_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "entity.h"

/////////////////////////////////////////////////
///
/// \brief Subclass for Colonist Entities
/// 
/////////////////////////////////////////////////
class Colonist : public Entity
{
private:

	enum aiState { X, Y, Z }; //!< Enum for AI behavioural types // TEMPORARY
	aiState m_state; //!< Stores the current AI state that the Colonist is in
	
	float m_fRadius = 7.5f; //!< Holds the radial size of the Colonist

	float m_hunger; //!< Holds the hunger need level of the Colonist
	float m_thirst; //!< Holds the thirst need level of the Colonist

	//std::vector<std::shared_ptr<Memory>> m_memories; // TEMPORARY - Need to implement Memory at a later date

	/////////////////////////////////////////////////
	///
	/// \brief Draws the Colonist to the RenderTarget
	///
	/// \param target The RenderTarget to draw on
	/// \param states The corresponding RenderStates
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/////////////////////////////////////////////////
	///
	/// \brief Updates the Colonist with elapsed time
	///
	/// \param kfElapsedTime The time passed since last update in seconds
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void update(const float kfElapsedTime);

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	/// \param position A vector with x&y for position
	///
	///////////////////////////////////////////////// 
	Colonist(const sf::Vector2f kPosition);
};

#endif