/**
@file colonist.cpp
*/

// Imports
#include "colonist.h"
#include "utils.h"

// Constructor
Colonist::Colonist(Environment * pEnv, const sf::Vector2f kPosition, const float kfRadius, const float kfHeading, const float kfSpeed)
{
	// Casts the incoming pointer to a shared_ptr and assigns it to the member
	m_pEnvironment = (std::shared_ptr<Environment>)pEnv;

	// Sets member values to corresponding input
	m_position = kPosition;
	m_fHeading = kfHeading;
	m_fRadius = kfRadius;
	m_fSpeed = kfSpeed;

	m_state = IDLE; // Sets Colonist state to a default state: IDLE
	m_path = {}; // Sets the current path queue to empty
}

// Void: Called to update the Colonist
void Colonist::update(const float kfElapsedTime)
{
	switch (m_state)
	{
		case IDLE: idle(); break; // State: Idle - run method

		case EXPLORE: explore(); break; // State: Explore - run method

		case FORAGE: explore(); break; // State: Forage - run method

		case TENDTONEEDS: explore(); break; // State: TendToNeeds - run method

		case BREED: explore(); break; // State: Breed - run method

		default: m_state = IDLE; break; // No valid state found: set IDLE;
	}

	// If path queue is not empty
	if (!m_path.empty())
	{
		// Paths the Colonist to the pos at the front of the queue
		if (moveTo(m_path.front(), m_fSpeed*kfElapsedTime))
		{
			// Removes the location at the front of the queue
			m_path.pop();
		}
	}

	// Binds heading to 360 degrees
	if (m_fHeading >= 360) m_fHeading -= 360;
	else if (m_fHeading < 0) m_fHeading += 360;
}

// Void: Called to draw the Colonist
void Colonist::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Declares new CircleShape to draw
	sf::CircleShape circle;

	// Sets the origin to the center of the circle
	circle.setOrigin(sf::Vector2f(m_fRadius, m_fRadius));

	// Sets the circle pos to position member
	circle.setPosition(sf::Vector2f(m_position));

	// Sets the circle radius to radius member
	circle.setRadius(m_fRadius);

	// Sets circle colour: Black RGB for Colonist
	circle.setFillColor(sf::Color(0, 0, 0, 255));

	// Draws circle to target
	target.draw(circle);
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
	if (m_path.empty())
	{
		// Declares a cone that the randPos will sit within infront of the Colonist
		float fCone = 90;

		// Defines random float
		float fDeltaHeading = (rand() % (int)fCone) - (fCone*0.5f);

		// Defines position to path towards with the heading + random angle
		sf::Vector2f randPos = Utils::unitVecFromAngle(m_fHeading + fDeltaHeading);

		// With the randPos currently a Unit Vector it's now multiplied to be ahead of the Colonist instead of 1.0f distance away
		randPos *= m_fSpeed;

		// Creates a path to the position
		createPath(m_position + randPos);
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
		displacement = distance;
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

// Void: Determines a path to an input destination and queues it
void Colonist::createPath(const sf::Vector2f kDestination)
{
	if (Utils::pointInArea(kDestination, m_pEnvironment->getSize()))
	{
		sf::err() << "kDestination.x " << kDestination.x << " kDestination.y " << kDestination.y << std::endl;
		m_path.push(kDestination);
	}
	else
	{
		m_fHeading += 180;
	}
}