/**
@file colonist.cpp
*/

// Imports
#include "colonist.h"
#include "utils.h"

// Constructor
Colonist::Colonist(const sf::Vector2f kPosition)
{
	m_position = kPosition; // Sets position with input
	m_fHeading = 0.0f; // Sets heading to 0.0f

	m_fSpeed = 75.0f;
	m_fRadius = 7.5f;

	m_state = IDLE; // Sets Colonist state to a default state: IDLE
	m_path = {}; // Sets the current path queue to empty
}

// Void: Called to update the Colonist
void Colonist::update(const float kfElapsedTime)
{
	switch (m_state)
	{
		case IDLE: // TODO
		{
			m_state = EXPLORE; // TEMPORARY
		} break;

		case EXPLORE: // TODO
		{
			// If path queue is not empty
			if (!m_path.empty())
			{
				// Paths the Colonist to the pos at the front of the queue
				if (pathTo(m_path.front(), m_fSpeed*kfElapsedTime))
				{
					m_path.pop();
				}
			}
			else
			{
				// Run explore method
				explore();
			}

		} break;

		default: m_state = IDLE; break; // No valid state found: set IDLE;
	}
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

// Void: 
void Colonist::explore()
{
	float fCone = 75;
	
	// Defines random float
	float fDeltaHeading = (rand() % (int)fCone) - (fCone*0.5f);
	
	// Defines position to path towards with random angle
	sf::Vector2f randPos = Utils::unitVecFromAngle(fDeltaHeading);
	
	randPos *= 10.0f;
	
	// Adds random position to the Colonist's path
	m_path.push(m_position + randPos);
}

// Bool: Moves the Colonist toward a destination at an input speed - Returns whether Colonist is at the destination
bool Colonist::pathTo(const sf::Vector2f kDestination, const float fSpeed)
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

