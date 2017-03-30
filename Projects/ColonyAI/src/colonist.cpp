/**
@file colonist.cpp
*/

// Imports
#include "colonist.h"

// Constructor
Colonist::Colonist(std::shared_ptr<Environment> pEnv, const sf::Vector2f kPosition, const float kfHeading)
{
	// Defines the EntityType
	m_type = COLONIST;

	// Sets member values to corresponding input
	m_pEnvironment = pEnv;
	m_position = kPosition;
	m_fHeading = kfHeading;
	m_fRadius = 7.5f;
	m_fVision = 100.0f;
	m_fReach = m_fRadius*4;
	m_fSpeed = 150.0f;
	m_fBirthCooldown = 15.0f;

	// Defines fatal levels of hunger and thirst
	m_needs = Needs(300.0f, 120.0f); // 3 minutes // 2 minutes

	m_state = LABOUR; // Sets Colonist state to a default state: LABOUR

	// Nullptr check
	if (m_pEnvironment != nullptr)
	{
		m_pPathfinding = std::shared_ptr<Pathfinding>(new Pathfinding(std::shared_ptr<Colonist>(this), m_pEnvironment));
	}
}

// Void: Called to update the Colonist
void Colonist::update(const float kfElapsedTime)
{
	// If Colonist's not deceased
	if (isAlive())
	{
		// Iterates Hunger
		m_needs.setHunger(m_needs.getHunger() + kfElapsedTime);
		m_needs.setThirst(m_needs.getThirst() + kfElapsedTime);

		// Counts the birth cooldown down
		if (m_fBirthCooldown > 0.0f) m_fBirthCooldown -= kfElapsedTime;
		else if (m_fBirthCooldown < 0.0f) m_fBirthCooldown = 0.0f;

		// Calculate the Colonist's 'home' position
		// TEMPORARY - Currently ignores Memory entirely
		unsigned int uiCount = 0;
		sf::Vector2f sum;
		// For all Objects
		for (std::shared_ptr<Object> pObject : m_pEnvironment->getObjects())
		{
			// If Object is FoodSource or WaterSource
			if (pObject->getType() == BUSH || pObject->getType() == WATER)
			{
				// Adds position to sum and iterates counter
				sum += pObject->getPosition();
				uiCount++;
			}
		}
		// Home position = 0,0 (No sources in Environment)
		if (uiCount == 0) m_homePos = sf::Vector2f(0.0f, 0.0f);
		// Home position = avg position of all sources
		else m_homePos = sum / (float)uiCount;

		// Calls method to update Colonist Memory
		updateMemory((long)time(NULL));
		
		// Calls method to update Colonist AI state
		updateState();
	}

	switch (m_state)
	{
		case DECEASED: deceased(); break; // State: Deceased - run method

		case TENDTONEEDS: tendToNeeds(); break; // State: TendToNeeds - run method

		case REPRODUCE: reproduce(); break; // State: Reproduce - run method

		case LABOUR: labour(); break; // State: Labour - run method

		default: m_state = LABOUR; break; // No valid state found: set LABOUR;
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

// Void: Updates the Colonist's Memory
void Colonist::updateMemory(const long klTime)
{
	// For all Objects in the Environment
	for (std::shared_ptr<Object> pObject : m_pEnvironment->getObjects())
	{
		// If Object is within vision of the Colonist
		if (inVision(pObject->getPosition(), pObject->getRadius()))
		{
			// Stores whether the position is already in Memory
			bool bPosInMemory = false;

			// For all existing Memories
			for (std::shared_ptr<Memory> pMemory : m_pMemories)
			{
				// If Memory Object exists and If Memory has the same position as Object
				if ((pMemory->getType() != NULL_OBJECT) && (pMemory->getObject()->getPosition() == pObject->getPosition()))
				{
					// Sets bPosInMem true
					bPosInMemory = true;
					// Updates Memory with new time
					pMemory->setTime(klTime);
				}
			}

			// If position isn't in memory
			if (!bPosInMemory)
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

				// Adds the position to memory with corresponding type
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
		if (inVision(pEntity->getPosition(), pEntity->getRadius()))
		{
			// If the Entity is a Colonist
			if (pEntity->getType() == COLONIST)
			{
				// Casts the Entity to a Colonist
				std::shared_ptr<Colonist> pColonist = std::dynamic_pointer_cast<Colonist>(pEntity);

				// If Colonist is alive
				if (pColonist->isAlive())
				{
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
			}
			// Else
			else 
			{
				// TEMPORARY - Need to add other Entity handling later with food, stone and lumber
			}
		}
	}

	// For all existing Memories
	for (std::shared_ptr<Memory> pMemory : m_pMemories)
	{
		// If Memory Object is a nullptr and type is not set to NULL_OBJECT
		if ((pMemory->getObject() == nullptr) && (pMemory->getType() != NULL_OBJECT))
		{
			// Updates Memory type to NULL_OBJECT
			pMemory->setType(NULL_OBJECT);
		}
	}
}

// Void: Updates the Colonist's AI state
void Colonist::updateState()
{
	// Tier 01 - Is the Colonist Dead
	// If thirst or hunger is 100% of fatal level
	if (m_needs.getHungerPerc() >= 100.0f || m_needs.getThirstPerc() >= 100.0f)
	{
		m_state = DECEASED;
	}

	// Tier 02 - Is the Colonist dying
	// If thirst or hunger is 75% of fatal level
	else if (m_needs.getHungerPerc() >= 75.0f || m_needs.getThirstPerc() >= 75.0f)
	{
		m_state = TENDTONEEDS;
	}

	// Tier 03 - Is the Colonist well satisfied
	// If thirst or hunger is less than 25% of fatal level and able to birth
	else if ((m_needs.getHungerPerc() <= 25.0f && m_needs.getThirstPerc() <= 25.0f) && (m_fBirthCooldown == 0.0f))
	{
		m_state = REPRODUCE;
	}

	// Tier 04 - Nothing important to do
	// Else - Labour
	else
	{
		m_state = LABOUR;
	}
}

// Void: Processes DECEASED state functionality
void Colonist::deceased()
{
	// If path is not empty
	if (!m_pPathfinding->getPath().empty())
	{
		m_pPathfinding->clearPath();
	}
}

// Void: Processes TENDTONEEDS state functionality
void Colonist::tendToNeeds()
{
	// If hunger is more dire than thirst
	if (m_needs.getHungerPerc() > m_needs.getThirstPerc())
	{
		// TODO - Gotta modify to consume Food instead of replenishing with Bushes

		// Defines vector to store Food in vision
		std::vector<std::shared_ptr<Bush>> pFoodInVision;

		// If has vision of Bush Object
		for (std::shared_ptr<Object> pObject : m_pEnvironment->getObjects(BUSH))
		{
			// If Object is in vision
			if (inVision(pObject->getPosition(), pObject->getRadius()))
			{
				pFoodInVision.push_back(std::dynamic_pointer_cast<Bush>(pObject));
			}
		}

		// If there's Food in vision
		if (!pFoodInVision.empty())
		{
			// Determines nearest source
			std::shared_ptr<Bush> pNearestBush = pFoodInVision.front();
			for (std::shared_ptr<Bush> pBush : pFoodInVision)
			{
				// If pWater is closer than pNearestWater
				if (Utils::magnitude(pBush->getPosition() - m_position) - pBush->getRadius() <= Utils::magnitude(pNearestBush->getPosition() - m_position) - pNearestBush->getRadius())
				{
					pNearestBush.swap(pBush);
				}
			}

			// Determines nearest Node to source
			std::vector<std::shared_ptr<Node>> pPerimeterNodes = m_pPathfinding->perimeterNodes(pNearestBush->getPosition());
			std::shared_ptr<Node> pNearestNode = pPerimeterNodes.front();
			for (std::shared_ptr<Node> pNode : pPerimeterNodes)
			{
				// If pNode is closer than pNearestNode
				if (Utils::magnitude(pNode->getPosition() - m_position) <= Utils::magnitude(pNearestNode->getPosition() - m_position))
				{
					pNearestNode.swap(pNode);
				}
			}

			//	If destination Node doesn't exist
			if (pNearestNode == nullptr) {}
			// Destination Node exists and path isn't leading to Food source
			else
			{
				// If path exists
				if (!m_pPathfinding->getPath().empty())
				{
					// If path is leading to the Water source
					if (m_pPathfinding->getPath().back() == pNearestNode->getPosition()) {}
					else
					{
						// Generate path to Food Source
						m_pPathfinding->createPathTo(pNearestNode);
					}
				}
				else
				{
					// Generate path to Food Source
					m_pPathfinding->createPathTo(pNearestNode);
				}
			}

			// If Food source is within reach
			if (inReach(pNearestBush->getPosition(), pNearestBush->getRadius()))
			{
				// Replenishes hunger 
				m_needs.setHunger(0.0f);
			}
		}

		// Else If has memory of a food source
		else if (Memory::typeInMem(FOOD_SOURCE, m_pMemories))
		{
			// Defines vector to store Food_Source memories
			std::vector<std::shared_ptr<Bush>> pFoodSources;

			// If has vision of food entity
			for (std::shared_ptr<Memory> pMemory : m_pMemories)
			{
				// If Memory is Food_Source
				if (pMemory->getType() == FOOD_SOURCE)
				{
					pFoodSources.push_back(std::dynamic_pointer_cast<Bush>(pMemory->getObject()));
				}
			}

			// Determines nearest source
			std::shared_ptr<Bush> pNearestBush = pFoodSources.front();
			for (std::shared_ptr<Bush> pBush : pFoodSources)
			{
				// If pBush is closer than pNearestBush
				if (Utils::magnitude(pBush->getPosition() - m_position) - pBush->getRadius() <= Utils::magnitude(pNearestBush->getPosition() - m_position) - pNearestBush->getRadius())
				{
					pNearestBush.swap(pBush);
				}
			}

			// Determines nearest Node to source
			std::vector<std::shared_ptr<Node>> pPerimeterNodes = m_pPathfinding->perimeterNodes(pNearestBush->getPosition());
			std::shared_ptr<Node> pNearestNode = pPerimeterNodes.front();
			for (std::shared_ptr<Node> pNode : pPerimeterNodes)
			{
				// If pNode is closer than pNearestNode
				if (Utils::magnitude(pNode->getPosition() - m_position) <= Utils::magnitude(pNearestNode->getPosition() - m_position))
				{
					pNearestNode.swap(pNode);
				}
			}

			//	If destination Node doesn't exist
			if (pNearestNode == nullptr) {}
			// Destination Node exists and path isn't leading to Food source
			else
			{
				// If path exists
				if (!m_pPathfinding->getPath().empty())
				{
					// If path is leading to the Food source
					if (m_pPathfinding->getPath().back() == pNearestNode->getPosition()) {}
					else
					{
						// Generate path to Food Source
						m_pPathfinding->createPathTo(pNearestNode);
					}
				}
				else
				{
					// Generate path to Food Source
					m_pPathfinding->createPathTo(pNearestNode);
				}
			}

			// If Food source is within reach
			if (inReach(pNearestBush->getPosition(), pNearestBush->getRadius()))
			{
				// Replenishes hunger 
				m_needs.setHunger(0.0f);
			}
		}
		// Else - No knowledge of food or source
		else
		{
			// Go to labour
			labour();
		}
	}
	// If thirst is more dire than hunger
	else
	{
		// Defines vector to store Water in vision
		std::vector<std::shared_ptr<Water>> pWaterInVision;

		// If has vision of Water object
		for (std::shared_ptr<Object> pObject : m_pEnvironment->getObjects(WATER))
		{
			// If Entity is in vision
			if (inVision(pObject->getPosition(), pObject->getRadius()))
			{
				pWaterInVision.push_back(std::dynamic_pointer_cast<Water>(pObject));
			}
		}

		// If there's Water in vision
		if (!pWaterInVision.empty())
		{
			// Determines nearest source
			std::shared_ptr<Water> pNearestWater = pWaterInVision.front();
			for (std::shared_ptr<Water> pWater : pWaterInVision)
			{
				// If pWater is closer than pNearestWater
				if (Utils::magnitude(pWater->getPosition() - m_position) - pWater->getRadius() <= Utils::magnitude(pNearestWater->getPosition() - m_position) - pNearestWater->getRadius())
				{
					pNearestWater.swap(pWater);
				}
			}

			// Determines nearest Node to source
			std::vector<std::shared_ptr<Node>> pPerimeterNodes = m_pPathfinding->perimeterNodes(pNearestWater->getPosition());
			std::shared_ptr<Node> pNearestNode = pPerimeterNodes.front();
			for (std::shared_ptr<Node> pNode : pPerimeterNodes)
			{
				// If pNode is closer than pNearestNode
				if (Utils::magnitude(pNode->getPosition() - m_position) <= Utils::magnitude(pNearestNode->getPosition() - m_position))
				{
					pNearestNode.swap(pNode);
				}
			}

			//	If destination Node doesn't exist
			if (pNearestNode == nullptr) {}
			// Destination Node exists and path isn't leading to Water source
			else
			{
				// If path exists
				if (!m_pPathfinding->getPath().empty())
				{
					// If path is leading to the Water source
					if (m_pPathfinding->getPath().back() == pNearestNode->getPosition()) {}
					else
					{
						// Generate path to Water Source
						m_pPathfinding->createPathTo(pNearestNode);
					}
				}
				else
				{
					// Generate path to Water Source
					m_pPathfinding->createPathTo(pNearestNode);
				}
			}

			// If Water source is within reach
			if (inReach(pNearestWater->getPosition(), pNearestWater->getRadius()))
			{
				// Replenishes thirst 
				m_needs.setThirst(0.0f);
			}
		}

		// If has Memory of a Water source
		else if (Memory::typeInMem(WATER_SOURCE, m_pMemories))
		{
			// Defines vector to store Water_Source memories
			std::vector<std::shared_ptr<Water>> pWaterSources;

			// If has vision of food entity
			for (std::shared_ptr<Memory> pMemory : m_pMemories)
			{
				// If Memory is Food_Source
				if (pMemory->getType() == WATER_SOURCE)
				{
					pWaterSources.push_back(std::dynamic_pointer_cast<Water>(pMemory->getObject()));
				}
			}

			// Determines nearest source
			std::shared_ptr<Water> pNearestWater = pWaterSources.front();
			for (std::shared_ptr<Water> pWater : pWaterSources)
			{
				// If pWater is closer than pNearestWater
				if (Utils::magnitude(pWater->getPosition() - m_position) - pWater->getRadius() <= Utils::magnitude(pNearestWater->getPosition() - m_position) - pNearestWater->getRadius())
				{
					pNearestWater.swap(pWater);
				}
			}

			// Determines nearest Node to source
			std::vector<std::shared_ptr<Node>> pPerimeterNodes = m_pPathfinding->perimeterNodes(pNearestWater->getPosition());
			std::shared_ptr<Node> pNearestNode = pPerimeterNodes.front();
			for (std::shared_ptr<Node> pNode : pPerimeterNodes)
			{
				// If pNode is closer than pNearestNode
				if (Utils::magnitude(pNode->getPosition() - m_position) <= Utils::magnitude(pNearestNode->getPosition() - m_position))
				{
					pNearestNode.swap(pNode);
				}
			}

			//	If destination Node doesn't exist
			if (pNearestNode == nullptr) {}
			// Destination Node exists and path isn't leading to Water source
			else
			{
				// If path exists
				if (!m_pPathfinding->getPath().empty()) 
				{
					// If path is leading to the Water source
					if (m_pPathfinding->getPath().back() == pNearestNode->getPosition()) {}
					else
					{
						// Generate path to Water Source
						m_pPathfinding->createPathTo(pNearestNode);
					}
				}
				else
				{
					// Generate path to Water Source
					m_pPathfinding->createPathTo(pNearestNode);
				}
			}

			// If Water source is within reach
			if (inReach(pNearestWater->getPosition(), pNearestWater->getRadius()))
			{
				// Replenishes thirst 
				m_needs.setThirst(0.0f);
			}
		}

		// Else - No knowledge of water
		else
		{
			// Go to labour
			labour();
		}
	}
}

// Void: Processes REPRODUCE state functionality
void Colonist::reproduce()
{
	// Adds a cooldown to the Colonist before it can reproduce again
	m_fBirthCooldown = 300.0f; // 5 Minute cooldown

	// Adds a new Colonist Entity to the Environment
	m_pEnvironment->getEntityVec()->push_back
	(
		std::shared_ptr<Entity>(new Colonist(m_pEnvironment, m_position, m_fHeading))
	);

	sf::err() << "[COLONIST] New Colonist produced at x(" << m_position.x << ") y(" << m_position.y << ") h(" << m_fHeading << ")" << std::endl;
}

// Void: Processes LABOUR state functionality
void Colonist::labour()
{
	// If X: Explore Environment 
	if (true) // TEMPORARY
	{
		// If path queue is empty
		if (m_pPathfinding->getPath().empty())
		{
			// Declares a cone that the randPos will sit within infront of the Colonist
			float fCone = 60.0f;

			// Defines a random angle ((0 and fCone) - 30) so -30 to 30
			float fRandomAngle = (rand() % (int)(fCone + 1)) - (fCone*0.5f);

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
		}
	}
}

// Void: Consumes Food to replenish hunger
void Colonist::eat(std::shared_ptr<Food> pFood)
{
	// Hunger is completely replenished
	m_needs.setHunger(0.0f); // TODO - May add a level of replenishment Food can restore instead of 100%

	// Removes the Food object pointer
	pFood = nullptr;
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

// Bool: Whether the position is within vision
bool Colonist::inVision(const sf::Vector2f kPosition, const float kfRadius)
{
	if (Utils::magnitude(kPosition - m_position) - kfRadius < m_fVision) return true;
	return false;
}

// Bool: Whether the position is within reach
bool Colonist::inReach(const sf::Vector2f kPosition, const float kfRadius)
{
	if (Utils::magnitude(kPosition - m_position) - kfRadius < m_fReach) return true;
	return false;
}

// Void: Called to draw the Colonist
void Colonist::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Declares new CircleShape to draw
	sf::CircleShape circle;

	// Sets circle colour: Black RGB for alive Colonist's, grey for deceased
	if (m_state != DECEASED) circle.setFillColor(sf::Color(0, 0, 0, 255));
	else circle.setFillColor(sf::Color(80, 80, 80, 255));

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
		sf::Color colour;

		// Sets line colour: Black RGB for alive Colonist's, grey for deceased
		if (m_state != DECEASED) colour = sf::Color(0, 0, 0, 255);
		else colour = sf::Color(80, 80, 80, 255);

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

		///////////////////// REACH /////////////////////
		// Sets the colour to black
		colour = sf::Color(0, 0, 0, 255);

		// Sets circle colour: Transparent with black outline
		circle.setFillColor(sf::Color(0, 0, 0, 0));
		circle.setOutlineColor(colour);
		circle.setOutlineThickness(1.0f);

		// Sets the circle radius to radius member
		circle.setRadius(m_fReach);
		// Sets the origin to the center of the circle
		circle.setOrigin(sf::Vector2f(m_fReach, m_fReach));
		// Sets the circle pos to position member
		circle.setPosition(m_position);

		// Draws circle to target
		target.draw(circle);

		///////////////////// HOME POSITION /////////////////////
		// Sets the colour to translucent black
		colour = sf::Color(0, 0, 0, 75);
		// Sets the first point of the line at the Colonist position
		line[0] = sf::Vertex(m_position, colour);
		// Sets the second point of the line at the Home position
		line[1] = sf::Vertex(m_homePos, colour);

		// Draws the line to target
		target.draw(line, 2, sf::Lines);

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

		///////////////////// NEEDS /////////////////////
		// Declares new RectShape to draw need bars
		sf::RectangleShape rect;
		// Defines the status bars size
		sf::Vector2f barSize(m_fRadius*3.0f, m_fRadius*0.5f);

		// Thirst
		// Sets the rect to blue
		rect.setFillColor(sf::Color(0, 0, 255, 255));
		// Sets the rect size
		rect.setSize(barSize);
		// Sets the origin to the bottom right corner of the rect
		rect.setOrigin(sf::Vector2f(rect.getSize().x, rect.getSize().y));

		// Sets the rect pos to above the Colonist
		rect.setPosition(sf::Vector2f(m_position.x + rect.getSize().x*0.5f, (m_position.y - m_fRadius - rect.getSize().y*0.5f)));
		// Draws rect to target
		target.draw(rect);

		// Sets the rect to red
		rect.setFillColor(sf::Color(255, 0, 0, 255));
		// Sets the rect size
		rect.setSize(sf::Vector2f(barSize.x*(m_needs.getThirstPerc() / 100), barSize.y));
		// Sets the origin to the center of the rect
		rect.setOrigin(sf::Vector2f(rect.getSize().x, rect.getSize().y));

		// Draws rect to target
		target.draw(rect);

		// Hunger
		// Sets the rect to green
		rect.setFillColor(sf::Color(0, 255, 0, 255));
		// Sets the rect size
		rect.setSize(barSize);
		// Sets the origin to the bottom right corner of the rect
		rect.setOrigin(sf::Vector2f(rect.getSize().x, rect.getSize().y));

		// Sets the rect pos to above the Colonist
		rect.setPosition(sf::Vector2f(m_position.x + rect.getSize().x*0.5f, (m_position.y - m_fRadius - rect.getSize().y*0.5f) - rect.getSize().y*1.5f));
		// Draws rect to target
		target.draw(rect);

		// Sets the rect to red
		rect.setFillColor(sf::Color(255, 0, 0, 255));
		// Sets the rect size
		rect.setSize(sf::Vector2f(barSize.x*(m_needs.getHungerPerc() / 100), barSize.y));
		// Sets the origin to the center of the rect
		rect.setOrigin(sf::Vector2f(rect.getSize().x, rect.getSize().y));

		// Draws rect to target
		target.draw(rect);
	}
}