#ifndef COLONIST_H
#define COLONIST_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <math.h>
#include <queue>
#include "entity.h"
#include "pathfinding.h"
#include "utils.h"

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

	float m_fHunger; //!< Hunger need level of the Colonist
	float m_fThirst; //!< Thirst need level of the Colonist

	std::shared_ptr<Entity> m_pHeldEntity; //!< Pointer to an Entity the Colonist is holding

	enum aiState { IDLE, EXPLORE, FORAGE, TENDTONEEDS, BREED }; //!< Enum for AI behavioural types
	aiState m_state; //!< Current AI state of the Colonist

	std::shared_ptr<Pathfinding> m_pPathfinding; //!< Pathfinding Object for calculating paths
	
	//std::vector<std::shared_ptr<Memory>> m_pMemories; // TEMPORARY - Need to implement Memory at a later date

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
	/// \param fSpeed The speed it should move at
	///
	/// \return Whether Colonist is at the destination
	///
	///////////////////////////////////////////////// 
	bool moveTo(const sf::Vector2f kDestination, const float fSpeed);

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	///	\param pEnv Pointer to the Environment the Colonist is within
	/// \param kPosition A vector with x&y for position
	/// \param kfRadius A float for radial size
	/// \param kfHeading A float for initial heading
	/// \param kfSpeed A float for speed per second
	///
	///////////////////////////////////////////////// 
	Colonist(Environment * kpEnv, const sf::Vector2f kPosition, const float kfRadius, const float kfHeading, const float kfSpeed);
};

#endif