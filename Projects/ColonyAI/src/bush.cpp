/**
@file bush.cpp
*/

// Imports
#include "bush.h"

// Constructor
Bush::Bush(std::shared_ptr<Environment> pEnv, const sf::Vector2f kPosition, const float kfRadius)
{
	// Defines the ObjectType
	m_type = BUSH;

	// Sets member values to corresponding input
	m_pEnvironment = pEnv;
	m_position = kPosition;
	m_fRadius = kfRadius;
}

// Void: Called to draw the Bush
void Bush::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Declares new CircleShape to draw
	sf::CircleShape circle;

	// Sets the origin to the center of the circle
	circle.setOrigin(sf::Vector2f(m_fRadius, m_fRadius));

	// Sets the circle pos to position member
	circle.setPosition(sf::Vector2f(m_position));

	// Sets the circle radius to radius member
	circle.setRadius(m_fRadius);

	// Sets circle colour: Green RGB for bush
	circle.setFillColor(sf::Color(61, 237, 17, 255));

	// Draws circle to target
	target.draw(circle);
}

// Void: Called to update the Bush
void Bush::update(const float kfElapsedTime)
{
	// Iterates the growth time of the Bush
	m_fGrowth += kfElapsedTime;

	// If growth exceeds growthLimit?
	if (m_fGrowth >= m_fGrowthLimit)
	{
		// Generate new growthLimit (0 and 240) + 60 // 1-5 minutes
		m_fGrowthLimit = float((rand() % int(240 + 1)) + 60);
		// Resets growth
		m_fGrowth = 0.0f;

		// Generate angle (0 and 359)
		float fAngle = float(rand() % (int)(360));
		// Creates vector with length of the Bush radius
		sf::Vector2f foodPos((Utils::unitVecFromAngle(fAngle)*m_fRadius) + m_position);

		// Adds a Food Entity to the Environment
		m_pEnvironment->getEntityVec()->push_back
		(
			std::shared_ptr<Entity>(new Food(m_pEnvironment, foodPos))
		);

		sf::err() << "[BUSH] Food produced at x(" << foodPos.x << ") y(" << foodPos.y << ")" << std::endl;
	}
}