#ifndef COLONIST_H
#define COLONIST_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "entity.h"
#include "environment.h"
#include <memory>
#include <math.h>
#include <queue>

class Environment;

/////////////////////////////////////////////////
///
/// \brief Subclass for Colonist Entities
/// 
/////////////////////////////////////////////////
class Colonist : public Entity
{
private:

	std::shared_ptr<Environment> m_pEnvironment;

	enum aiState { IDLE, EXPLORE }; //!< Enum for AI behavioural types
	aiState m_state; //!< Stores the current AI state that the Colonist is in
	std::queue<sf::Vector2f> m_path; //!< Stores a queue of positions for path finding

	float m_fSpeed; //!< Holds the speed the Colonist can move at per second
	float m_fRadius; //!< Holds the radial size of the Colonist

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
	/// \brief Moves the Colonist toward a destination at an input speed
	///
	/// \param kDestination A position to move toward
	/// \param fSpeed The speed it should move at
	///
	/// \return Whether Colonist is at the destination
	///
	///////////////////////////////////////////////// 
	bool moveTo(const sf::Vector2f kDestination, const float fSpeed);

	/////////////////////////////////////////////////
	///
	/// \brief Determines a path to an input destination and queues it
	///
	/// \param kDestination A position to path toward
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void createPath(const sf::Vector2f kDestination);

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
	Colonist(Environment * pEnv, const sf::Vector2f kPosition, const float kfRadius, const float kfHeading, const float kfSpeed);
	~Colonist() {};

	std::shared_ptr<Environment> getEnvironment() { return m_pEnvironment; };
};

#endif