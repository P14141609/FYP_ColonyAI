/**
@file colonist.cpp
*/

// Imports
#include "colonist.h"

// Constructor
Colonist::Colonist(Environment * pEnv, const sf::Vector2f kPosition, const float kfHeading)
{
	// Defines the EntityType
	m_type = COLONIST;

	// Casts the incoming pointer to a shared_ptr and assigns it to the member
	m_pEnvironment = std::shared_ptr<Environment>(pEnv);

	// Sets member values to corresponding input
	m_position = kPosition;
	m_fHeading = kfHeading;
	m_fRadius = 7.5f;
	m_fVision = 100.0f;
	m_fSpeed = 50.0f;

	m_state = IDLE; // Sets Colonist state to a default state: IDLE

	m_pPathfinding = std::shared_ptr<Pathfinding>(new Pathfinding(this, m_pEnvironment));
}

// Void: Called to update the Colonist
void Colonist::update(const float kfElapsedTime)
{
	// Calls method to update Colonist Memory
	updateMemory((long)time(NULL));

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
		if (moveTo(m_pPathfinding->getPath().front(), kfElapsedTime))
		{
			// Removes the location at the front of the queue
			m_pPathfinding->popPath();
		}
	}

	// Binds heading to 360 degrees
	m_fHeading = Utils::bindNum(m_fHeading, 0, 360);
}

// Updates the Colonist's Memory
void Colonist::updateMemory(const long klTime)
{
	// For all Objects in the Environment
	for (std::shared_ptr<Object> pObject : m_pEnvironment->getObjects())
	{
		// If Object is within vision of the Colonist
		if (Utils::magnitude(pObject->getPosition() - m_position) - pObject->getRadius() < m_fVision)
		{
			// Stores whether the position is already in Memory
			bool bPosInMemory = false;

			// For all existing Memories
			for (std::shared_ptr<Memory> pMemory : m_pMemories)
			{
				// If Memory has the same position as Object
				if (pMemory->getObject()->getPosition() == pObject->getPosition())
				{
					// Sets bPosInMem true
					bPosInMemory = true;
				}
			}

			// If position is in Memory
			if (bPosInMemory)
			{
				// TEMPORARY - Need to update whether it is still a valid memory
			}
			// Else position isn't in Memory
			else
			{
				// Declares a MemoryType with default: OBSTRUCTION
				MemoryType type = OBSTRUCTION;

				// If Object is a Bush
				if (pObject->getType() == BUSH) { type = FOOD_SOURCE; }

				// If Object is a Rock
				else if (pObject->getType() == ROCK) { type = STONE_SOURCE; }

				// If Object is a Tree
				else if (pObject->getType() == TREE) { type = LUMBER_SOURCE; }

				// If Object is Water
				else if (pObject->getType() == WATER) { type = WATER_SOURCE; }

				// Adds the position to Memory with corresponding type
				m_pMemories.push_back(std::shared_ptr<Memory>
				(
					new Memory(klTime, pObject, type))
				);

				// Calculates Node accessibility with new Memory Object
				m_pPathfinding->calcAccess(pObject->getPosition(), pObject->getRadius());
			}
		}
	}

	// For all Entities in the Environment
	for (std::shared_ptr<Entity> pEntity : m_pEnvironment->getEntities())
	{
		// If Entity is within vision of the Colonist
		if (Utils::magnitude(pEntity->getPosition() - m_position) - pEntity->getRadius() < m_fVision)
		{
			// If the Entity is a Colonist
			if (pEntity->getType() == COLONIST)
			{
				// Casts the Entity to a Colonist
				std::shared_ptr<Colonist> pColonist = std::dynamic_pointer_cast<Colonist>(pEntity);

				// For all of their Memories
				for (std::shared_ptr<Memory> pTheirMemory : pColonist->getMemories())
				{
					// Declares bool; whether their Memory is in ours
					bool bInTheirMem = false;

					// For all of our Memories
					for (std::shared_ptr<Memory> pOurMemory : m_pMemories)
					{
						// If our Memory is in theirs
						if (pOurMemory->getObject() == pTheirMemory->getObject())
						{
							// Update our time to the most recent of the two
							// This means the Memories are kept up to date
							pOurMemory->setTime(Utils::max(pOurMemory->getTime(), pTheirMemory->getTime()));

							// Sets true; their memory is in ours
							bInTheirMem = true;
						}
					}

					// If other Colonist's Memory isn't in our Memory
					if (!bInTheirMem)
					{
						// Add their Memory to our Memory
						// Note a new ptr is created instead of having two Colonists with the same Memory ptr
						m_pMemories.push_back(std::shared_ptr<Memory>(pTheirMemory.get()));

						// Calculates Node accessibility with new Memory Object
						m_pPathfinding->calcAccess(pTheirMemory->getObject()->getPosition(), pTheirMemory->getObject()->getRadius());
					}
				}
			}
			// Else
			else 
			{
				// TEMPORARY - Need to add other Entity handling later with food, stone and lumber
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
		// Declares a cone that the randPos will sit within infront of the Colonist
		float fCone = 60.0f;

		// Defines a random angle ((0 and fCone) - 30) so -30 to 30
		float fRandomAngle = (rand() % (int)(fCone+1)) - (fCone*0.5f);

		// Applies the delta heading
		m_fHeading += fRandomAngle;

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
		//else m_fHeading += 180; // Reverses Colonist heading
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
bool Colonist::moveTo(const sf::Vector2f kDestination, const float kfElapsedTime)
{
	// Defines the distance between current and desired position
	sf::Vector2f distance = kDestination - m_position;
	
	// Defines displacement with the distance normal and speed
	sf::Vector2f displacement = Utils::normaliseVec(distance) * (m_fSpeed*kfElapsedTime);

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
		///////////////////// HEADING /////////////////////
		// Declares line and colour
		sf::Vertex line[2];
		sf::Color colour = sf::Color(0, 0, 0, 255);

		// Sets the first point of the line at the Colonist position
		line[0] = sf::Vertex(m_position, colour);
		// Sets the second point of the line infront of the Colonist based on heading
		line[1] = sf::Vertex(m_position + (Utils::unitVecFromAngle(m_fHeading) * (m_fRadius*2.0f)), colour);

		// Draws the line to target
		target.draw(line, 2, sf::Lines);

		///////////////////// VISION /////////////////////
		// Sets the colour to blue
		colour = sf::Color(0, 0, 255, 255);

		// Sets circle colour: Transparent with black outline
		circle.setFillColor(sf::Color(0, 0, 0, 0));
		circle.setOutlineColor(colour);
		circle.setOutlineThickness(1.0f);

		// Sets the circle radius to radius member
		circle.setRadius(m_fVision);
		// Sets the origin to the center of the circle
		circle.setOrigin(sf::Vector2f(m_fVision, m_fVision));
		// Sets the circle pos to position member
		circle.setPosition(m_position);

		// Draws circle to target
		target.draw(circle);

		///////////////////// MEMORY /////////////////////
		// For all Memories
		for (std::shared_ptr<Memory> pMemory : m_pMemories)
		{
			// Sets circle colour: Transparent with blue outline
			circle.setFillColor(sf::Color(0, 0, 0, 0));
			circle.setOutlineColor(colour);
			circle.setOutlineThickness(1.0f);

			// Sets the circle radius to radius member
			circle.setRadius(pMemory->getObject()->getRadius());
			// Sets the origin to the center of the circle
			circle.setOrigin(sf::Vector2f(pMemory->getObject()->getRadius(), pMemory->getObject()->getRadius()));
			// Sets the circle pos to position member
			circle.setPosition(pMemory->getObject()->getPosition());

			// Draws circle to target
			target.draw(circle);

			// Sets the colour to translucent blue
			colour = sf::Color(0, 0, 255, 75);
			// Sets the first point of the line at the Colonist position
			line[0] = sf::Vertex(m_position, colour);
			// Sets the second point of the line at the Memory position
			line[1] = sf::Vertex(pMemory->getObject()->getPosition(), colour);

			// Draws the line to target
			target.draw(line, 2, sf::Lines);
		}

		///////////////////// PATHFINDING /////////////////////
		// Draws pathfinding info
		m_pPathfinding->draw(target);
	}
}