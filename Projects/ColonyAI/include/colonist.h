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
	}
	//!< Gets Hunger level
	float getHunger() { return m_fHunger; }
	//!< Gets HungerPerc level
	float getHungerPerc() const { return (m_fHunger / m_fFatalHunger) * 100.0f; }

	//!< Sets Thirst level to a new value
	void setThirst(const float kfThirst)
	{
		// Sets value
		m_fThirst = kfThirst;

		// Caps value between minimum and max Thirst
		if (m_fThirst < 0.0f) m_fThirst = 0.0f;
		else if (m_fThirst > m_fFatalThirst) m_fThirst = m_fFatalThirst;
	}
	//!< Gets Thirst level
	float getThirst() { return m_fThirst; }
	//!< Gets ThirstPerc level
	float getThirstPerc() const { return (m_fThirst / m_fFatalThirst) * 100.0f; }

private:
	float m_fHunger; //!< Hunger need level of the Colonist
	float m_fThirst; //!< Thirst need level of the Colonist

	float m_fFatalHunger; //!< Hunger need level that kills the Colonist
	float m_fFatalThirst; //!< Thirst need level that kills the Colonist
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

	float m_fBirthCooldown; //!< Reproduction cooldown in seconds

	sf::Vector2f m_homePos; //!< Home position
	std::vector<std::shared_ptr<Object>> m_pClearObjs; //!< Objects with clear routes to from home

	std::shared_ptr<Entity> m_pHeldEntity; //!< Pointer to an Entity the Colonist is holding

	enum aiState { DECEASED, TENDTONEEDS, REPRODUCE, LABOUR }; //!< Enum for AI behavioural types
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
	/// \brief Sets the Memories vector to a new value
	///
	/// \param kpMemories A new Memories value
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setMemories(const std::vector<std::shared_ptr<Memory>> kpMemories) 
	{
		// For all Memories
		for (std::shared_ptr<Memory> pMemory : kpMemories)
		{
			// Pushes new ptr onto member
			m_pMemories.push_back(std::shared_ptr<Memory>(pMemory.get()));
		}
	}

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
	/// \brief Processes DECEASED state functionality
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void deceased();

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
	/// \brief Processes REPRODUCE state functionality
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void reproduce();

	/////////////////////////////////////////////////
	///
	/// \brief Processes LABOUR state functionality
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void labour();

	/////////////////////////////////////////////////
	///
	/// \brief Paths the Colonist forward with a random heading
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void wander();

	/////////////////////////////////////////////////
	///
	/// \brief Sets the clearObjs vector to a new value
	///
	/// \param kpClearObjs A new clearObjs value
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setClearObjs(const std::vector<std::shared_ptr<Object>> kpClearObjs)
	{
		// For all clearObjs
		for (std::shared_ptr<Object> pClearObj : kpClearObjs)
		{
			// Pushes new ptr onto member
			m_pClearObjs.push_back(std::shared_ptr<Object>(pClearObj.get()));
		}
	}

	/////////////////////////////////////////////////
	///
	/// \brief Determines whether the route from home to the object is clear
	///
	/// \return Whether the route is clear
	///
	///////////////////////////////////////////////// 
	bool routeClear(const std::shared_ptr<Object> kpObject);

	/////////////////////////////////////////////////
	///
	/// \brief Makes the Colonist clear the path to an Object from home
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void clearRoute(const std::shared_ptr<Object> kpObject);

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