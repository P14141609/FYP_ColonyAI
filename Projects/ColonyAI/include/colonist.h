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
class Food;

//!< Struct that holds Colonist needs data
struct Needs
{
public:
	//!< Default Constructor
	Needs() {}

	//!< Constructor
	Needs(const float kfFatalHunger, const float kfFatalThirst)
	{
		// Sets member values to corresponding input
		m_fFatalHunger = kfFatalHunger;
		m_fFatalThirst = kfFatalThirst;
		// Defines initial hunger and thirst as 25% of fatal levels
		setHunger(kfFatalHunger*0.25f);
		setThirst(kfFatalThirst*0.25f);
	}

	//!< Sets Hunger level to a new value
	void setHunger(const float kfHunger)
	{
		// Sets value
		m_fHunger = kfHunger;

		// Caps value between minimum and max Hunger
		if (m_fHunger < 0.0f) m_fHunger = 0.0f;
		else if (m_fHunger > m_fFatalHunger) m_fHunger = m_fFatalHunger;

		// Updates percentage value
		m_fHungerPerc = (kfHunger / m_fFatalHunger) * 100.0f;
	}
	//!< Gets Hunger level
	float getHunger() { return m_fHunger; }
	//!< Gets HungerPerc level
	float getHungerPerc() const { return m_fHungerPerc; }

	//!< Sets Thirst level to a new value
	void setThirst(const float kfThirst)
	{
		// Sets value
		m_fThirst = kfThirst;

		// Caps value between minimum and max Thirst
		if (m_fThirst < 0.0f) m_fThirst = 0.0f;
		else if (m_fThirst > m_fFatalThirst) m_fThirst = m_fFatalThirst;

		// Updates percentage value
		m_fThirstPerc = (kfThirst / m_fFatalThirst) * 100.0f;
	}
	//!< Gets Thirst level
	float getThirst() { return m_fThirst; }
	//!< Gets ThirstPerc level
	float getThirstPerc() const { return m_fThirstPerc; }

private:
	float m_fHunger; //!< Hunger need level of the Colonist
	float m_fThirst; //!< Thirst need level of the Colonist

	float m_fFatalHunger; //!< Hunger need level that kills the Colonist
	float m_fFatalThirst; //!< Thirst need level that kills the Colonist

	float m_fHungerPerc; //!< Hunger need level of the Colonist as a percentage
	float m_fThirstPerc; //!< Thirst need level of the Colonist as a percentage
};

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
	float m_fReach; //!< Radial size of the Colonist's reach

	Needs m_needs; //!< Hunger and Thirst data

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
	/// \brief Processes DECEASED state functionality
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void deceased();

	/////////////////////////////////////////////////
	///
	/// \brief Consumes Food to replenish hunger
	///
	/// \param pFood A Food Entity to consume
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void eat(std::shared_ptr<Food> pFood);

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

	/////////////////////////////////////////////////
	///
	/// \brief Returns whether an item is within vision
	///
	/// \param kPosition Position of an item to check
	/// \param kfRadius Radial size of the item
	///
	/// \return Whether the item is within vision
	///
	///////////////////////////////////////////////// 
	bool inVision(const sf::Vector2f kPosition, const float kfRadius);

	/////////////////////////////////////////////////
	///
	/// \brief Returns whether an item is within reach
	///
	/// \param kPosition Position of an item to check
	/// \param kfRadius Radial size of the item
	///
	/// \return Whether the item is within reach
	///
	///////////////////////////////////////////////// 
	bool inReach(const sf::Vector2f kPosition, const float kfRadius);

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
	/// \brief Gets whether the Colonist is alive
	///
	/// \return Whether Colonist is alive
	///
	///////////////////////////////////////////////// 
	bool isAlive() { if (m_state != DECEASED) return true; return false; }

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