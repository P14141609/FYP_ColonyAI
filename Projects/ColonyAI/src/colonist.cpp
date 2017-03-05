/**
@file colonist.cpp
*/

// Imports
#include "colonist.h"

// Constructor
Colonist::Colonist(Environment * pEnv, const sf::Vector2f kPosition, const float kfHeading, const float kfRadius, const float kfSpeed)
{
	// Casts the incoming pointer to a shared_ptr and assigns it to the member
	m_pEnvironment = std::shared_ptr<Environment>(pEnv);

	// Sets member values to corresponding input
	m_position = kPosition;
	m_fHeading = kfHeading;
	m_fRadius = kfRadius;
	m_fVision = 100.0f;
	m_fSpeed = kfSpeed;

	m_state = IDLE; // Sets Colonist state to a default state: IDLE

	m_pPathfinding = std::shared_ptr<Pathfinding>(new Pathfinding(this, m_pEnvironment));
}

// Void: Called to update the Colonist
void Colonist::update(const float kfElapsedTime)
{
	// Calls method to update Colonist Memory
	updateMemory();

	switch (m_state)
	{
		case IDLE: idle(); break; // State: Idle - run method

		case EXPLORE: explore(); break; // State: Explore - run method

		case FORAGE: forage(); break; // State: Forage - run method

		case TENDTONEEDS: tendToNeeds(); break; // State: TendToNeeds - run method

		case BREED: breed(); break; // State: Breed - run method

		default: m_state = IDLE; break; // No valid state found: set IDLE;
	}

	// If path queue is not empty
	if (!m_pPathfinding->getPath().empty())
	{
		// Paths the Colonist to the pos at the front of the queue
		if (moveTo(m_pPathfinding->getPath().front(), m_fSpeed*kfElapsedTime))
		{
			// Removes the location at the front of the queue
			m_pPathfinding->popPath();
		}
	}

	// Binds heading to 360 degrees
	m_fHeading = Utils::bindNum(m_fHeading, 0, 360);
}

// Updates the Colonist's Memory
void Colonist::updateMemory()
{
	// TODO
	// Update Memory
	// Share Memories with nearby Colonists

	// For all objects in the Environment
	for (std::shared_ptr<Object> pObject : m_pEnvironment->getObjects())
	{
		// If Object is within vision of the Colonist
		if (Utils::magnitude(pObject->getPosition() - m_position) < m_fVision)
		{
			// Stores whether the position is already in Memory
			bool bPosInMemory = false;

			// For all existing Memories
			for (std::shared_ptr<Memory> pMemory : m_pMemories)
			{
				// If Memory has the same position as Object
				if (pMemory->getPosition() == pObject->getPosition())
				{
					// Sets bPosInMem true
					bPosInMemory = true;
				}
			}

			// If position is in Memory
			if (bPosInMemory)
			{
				// TEMPORARY
			}
			// Else position isn't in Memory
			else
			{
				// Adds the position to Memory
				m_pMemories.push_back(std::shared_ptr<Memory>
				(
					// TEMPORARY TIME
					new Memory((long)1234, pObject->getPosition(), pObject->getRadius(), OBSTRUCTION))
				);
			}
		}
	}
}

// Void: Processes IDLE state functionality
void Colonist::idle()
{
	m_state = EXPLORE; // TEMPORARY
}

// Void: Processes EXPLORE state functionality
void Colonist::explore()
{
	// If path queue is empty
	if (m_pPathfinding->getPath().empty())
	{
		// Calculates the accessibility of the Nodes
		m_pPathfinding->calcAccess();

		// Declares a cone that the randPos will sit within infront of the Colonist
		float fCone = 90.0f;

		// Defines random float for a delta heading
		float fDeltaHeading = (rand() % (int)fCone) - (fCone*0.5f);
		// Applies the delta heading
		m_fHeading += fDeltaHeading;

		// Defines the delta position with a unit vector from the heading
		sf::Vector2f deltaPos = Utils::unitVecFromAngle(m_fHeading);
		// Converts the delta position from a unit vector to a sizeable displacement with the Colonist speed
		deltaPos *= m_fSpeed;

		// Creates the randomly determined destination position 
		sf::Vector2f targetPos = m_position + deltaPos;

		// If destination is within the Environment
		if (Utils::pointInArea(targetPos, sf::Vector2f(0, 0), m_pEnvironment->getSize()))
		{
			// Creates path to the destination
			m_pPathfinding->createPathTo(m_pPathfinding->nodeFromPos(targetPos));
		}
		// Else 
		else m_fHeading += 180; // Reverses Colonist heading
	}
}

// Void: Processes FORAGE state functionality
void Colonist::forage()
{
	m_state = IDLE; // TEMPORARY
}

// Void: Processes TENDTONEEDS state functionality
void Colonist::tendToNeeds()
{
	m_state = IDLE; // TEMPORARY
}

// Void: Processes BREED state functionality
void Colonist::breed()
{
	m_state = IDLE; // TEMPORARY
}

// Bool: Moves the Colonist toward a destination at an input speed - Returns whether Colonist is at the destination
bool Colonist::moveTo(const sf::Vector2f kDestination, const float fSpeed)
{
	// Defines the distance between current and desired position
	sf::Vector2f distance = kDestination - m_position;
	
	// Defines displacement with the distance normal and speed
	sf::Vector2f displacement = Utils::normaliseVec(distance) * fSpeed;

	// If the displacement is larger than the distance between positions
	if (Utils::magnitude(displacement) >= Utils::magnitude(distance))
	{
		// Displacement is set to distance
		m_position = kDestination;
		// Return True (goal achieved)
		return true;
	}
	// Else the distance is larger than the displacement
	else 
	{
		// Applies displacement
		m_position += displacement;
	}
	
	// Return False (goal not achieved)
	return false;
}

// Void: Called to draw the Colonist
void Colonist::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Declares new CircleShape to draw
	sf::CircleShape circle;

	// Sets circle colour: Black RGB for Colonist
	circle.setFillColor(sf::Color(0, 0, 0, 255));

	// Sets the circle radius to radius member
	circle.setRadius(m_fRadius);
	// Sets the origin to the center of the circle
	circle.setOrigin(sf::Vector2f(m_fRadius, m_fRadius));
	// Sets the circle pos to position member
	circle.setPosition(sf::Vector2f(m_position));

	// Draws circle to target
	target.draw(circle);

	// If system debugging
	if (g_bDebugging)
	{
		// Sets circle colour: Transparent
		circle.setFillColor(sf::Color(0, 0, 0, 0));
		circle.setOutlineColor(sf::Color(0, 0, 0, 255));
		circle.setOutlineThickness(1.0f);

		// Sets the circle radius to radius member
		circle.setRadius(m_fVision);
		// Sets the origin to the center of the circle
		circle.setOrigin(sf::Vector2f(m_fVision, m_fVision));
		// Sets the circle pos to position member
		circle.setPosition(sf::Vector2f(m_position));

		// Draws circle to target
		target.draw(circle);

		// Declares line and colour
		sf::Vertex line[2];
		sf::Color colour = sf::Color(0, 0, 0, 255);

		// Sets the first point of the line at the Colonist position
		line[0] = sf::Vertex(sf::Vector2f(m_position), colour);
		// Sets the second point of the line infront of the Colonist based on heading
		line[1] = sf::Vertex(sf::Vector2f(m_position + (Utils::unitVecFromAngle(m_fHeading) * (m_fRadius*2.0f))), colour);

		// Draws the line to target
		target.draw(line, 2, sf::Lines);

		// Draws pathfinding info
		m_pPathfinding->draw(target);
	}
}