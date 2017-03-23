#pragma once
#ifndef COLONIST_H
#define COLONIST_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <math.h>
#include <queue>
#include <ctime>
#include "entity.h"
#include "memory.h"
#include "pathfinding.h"
#include "utils.h"

class Entity;
class Pathfinding;

/////////////////////////////////////////////////
///
/// \brief Subclass for Colonist Entities
/// 
/////////////////////////////////////////////////
class Colonist : public Entity
{
private:

	float m_fSpeed; //!< Speed the Colonist can move at per second
	float m_fHeading; //!< Heading in degrees

	float m_fVision; //!< Radial size of Colonist vision

	float m_fHunger; //!< Hunger need level of the Colonist
	float m_fThirst; //!< Thirst need level of the Colonist

	std::shared_ptr<Entity> m_pHeldEntity; //!< Pointer to an Entity the Colonist is holding

	enum aiState { IDLE, EXPLORE, FORAGE, TENDTONEEDS, BREED, DECEASED }; //!< Enum for AI behavioural types
	aiState m_state; //!< Current AI state of the Colonist

	std::shared_ptr<Pathfinding> m_pPathfinding; //!< Pathfinding Object for calculating paths
	
	std::vector<std::shared_ptr<Memory>> m_pMemories; //!< Vector of Colonist Memory data

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

	/////////////////////////////////////////////////
	///
	/// \brief Updates the Colonist's Memory
	///
	/// \param klTime The current time
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void updateMemory(const long klTime);

	/////////////////////////////////////////////////
	///
	/// \brief Updates the Colonist's AI state
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void updateState();

	/////////////////////////////////////////////////
	///
	/// \brief Processes IDLE state functionality
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void idle();

	/////////////////////////////////////////////////
	///
	/// \brief Processes EXPLORE state functionality
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void explore();

	/////////////////////////////////////////////////
	///
	/// \brief Processes FORAGE state functionality
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void forage();

	/////////////////////////////////////////////////
	///
	/// \brief Processes TENDTONEEDS state functionality
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void tendToNeeds();

	/////////////////////////////////////////////////
	///
	/// \brief Processes BREED state functionality
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void breed();

	/////////////////////////////////////////////////
	///
	/// \brief Moves the Colonist toward a destination at an input speed
	///
	/// \param kDestination A position to move toward
	/// \param kfElapsedTime kfElapsedTime The time passed since last update in seconds
	///
	/// \return Whether Colonist is at the destination
	///
	///////////////////////////////////////////////// 
	bool moveTo(const sf::Vector2f kDestination, const float kfElapsedTime);

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	///	\param pEnv Pointer to the Environment the Colonist is within
	/// \param kPosition A vector with in-world position
	/// \param kfHeading A float for initial heading
	///
	///////////////////////////////////////////////// 
	Colonist(std::shared_ptr<Environment> pEnv, const sf::Vector2f kPosition, const float kfHeading);

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Colonist heading angle
	///
	/// \return Heading in degrees
	///
	///////////////////////////////////////////////// 
	float getHeading() { return m_fHeading; }

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Colonist Memory vector
	///
	/// \return Vector of Memories
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Memory>> getMemories() { return m_pMemories; }
};

#endif