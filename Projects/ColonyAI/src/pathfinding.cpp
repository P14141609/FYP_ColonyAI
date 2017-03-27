/**
@file pathfinding.cpp
*/

// Imports
#include "pathfinding.h"

// Constructor
Pathfinding::Pathfinding(const std::shared_ptr<Colonist> kpColonist, const std::shared_ptr<Environment> kpEnvironment)
{
	// Sets member values to corresponding input
	m_pColonist = kpColonist;
	m_pEnv = kpEnvironment;

	sf::err() << "[PATHFINDING] Generating nodes for A* pathfinding..." << std::endl;

	// Divides the map size by the diameter of the Nodes to create a required number of columns and rows in the grid
	m_uiNodeCols = (unsigned int)floor(m_pEnv->getSize().x / m_fNodeDiameter);
	m_uiNodeRows = (unsigned int)floor(m_pEnv->getSize().y / m_fNodeDiameter);

	// For the number of Nodes needed for the height
	for (unsigned int y = 1; y <= m_uiNodeRows; y++)
	{
		// For the number of Nodes needed for the width
		for (unsigned int x = 1; x <= m_uiNodeCols; x++)
		{
			// Pushes a new Node onto vector member of Nodes
			m_pNodes.push_back(std::shared_ptr<Node>(new Node(sf::Vector2f((m_fNodeDiameter * x) - m_fNodeRadius, (m_fNodeDiameter * y) - m_fNodeRadius), (unsigned int)m_pNodes.size())));
		}
	}

	// Flags Nodes as initialised
	m_bNodesInit = true;

	sf::err() << "[PATHFINDING] Generating nodes for A* pathfinding... Finished." << std::endl;
}

// Void: Calculates which Nodes are accessible
void Pathfinding::calcAccess(const sf::Vector2f kPosition, const float kfRadius)
{
	sf::err() << "[PATHFINDING] Calculating node accessibility..." << std::endl;

	// For all Nodes
	for (std::shared_ptr<Node> pNode : m_pNodes)
	{
		// If Node is accessible
		if (pNode->isAccessible())
		{
			// Defines the distance from the Node to the point
			float fDistToMem = Utils::magnitude(kPosition - pNode->getPosition());
			// Defines the clearance, amount of the distance that isn't in the radius
			float fClearance = fDistToMem - kfRadius;

			// If clearance is less than the radius of the Colonist
			if (fClearance <= m_pColonist->getRadius()*1.25)
			{
				// Sets Node as inaccessible
				pNode->setAccessible(false);
			}
		}
	}

	sf::err() << "[PATHFINDING] Calculating node accessibility... Finished." << std::endl;
}

// Void: Creates a path to target Node
void Pathfinding::createPathTo(const std::shared_ptr<Node> kpTargetNode)
{
	sf::err() << "[PATHFINDING] Generating path..." << std::endl;

	// Clears current path
	clearPath();

	// If targetNode doesn't exist
	if (kpTargetNode == nullptr) 
	{
		sf::err() << "[PATHFINDING] Generating path... Error - Target node nullptr." << std::endl;
		return;
	}

	// If targetNode is inaccessible
	if (!kpTargetNode->isAccessible())
	{
		sf::err() << "[PATHFINDING] Generating path... Error - Target node inaccessible." << std::endl;
		return;
	}

	// If Nodes initialised
	if (m_bNodesInit)
	{
		///////////////////// Creating Open and Closed Lists /////////////////////

		// Declares vectors to store nodes to check and checked nodes
		std::vector<std::shared_ptr<Node>> pOpenNodes;
		std::vector<std::shared_ptr<Node>> pClosedNodes;

		// If Node closest to your current location exists
		if (nodeFromPos(m_pColonist->getPosition()) != nullptr)
		{
			// Add Node at your current location to closed list
			pClosedNodes.push_back(nodeFromPos(m_pColonist->getPosition()));
			// Resets the current Node
			pClosedNodes.back()->reset();
		}
		// There is no Node 
		else
		{
			sf::err() << "[PATHFINDING] Generating path... Error - No available node." << std::endl;
			return;
		}

		///////////////////// Calculating The Path /////////////////////

		// Bool for whether a path has been found
		bool bPathFound = false;

		// While a path has not been found
		while (!bPathFound)
		{
			///////////////////// Setting Up The Current Node /////////////////////

			// Current Node being tested
			std::shared_ptr<Node> pCurrentNode = pClosedNodes.back();

			// Sets the currentNode's heuristic as the manhattan distance from the Node to the target
			pCurrentNode->setH(distance(pCurrentNode, kpTargetNode));

			// If Current Node is actually the Target Node
			if (pCurrentNode == kpTargetNode)
			{
				sf::err() << "[PATHFINDING] Generating path... CurrentNode equal to TargetNode." << std::endl;
				bPathFound = true;
				return;
			}

			///////////////////// Calculating Open Adjacent Nodes /////////////////////

			// Get all adjacent nodes
			std::vector<std::shared_ptr<Node>> pAdjNodes = adjacentNodes(pCurrentNode, true);

			// For all adjacent nodes
			for (std::shared_ptr<Node> pAdjNode : pAdjNodes)
			{
				// If adjNode is inaccessible
				if (!pAdjNode->isAccessible()) { /* Ignores inaccessible Nodes */ }

				// Else If adjNode is the target destination
				else if (pAdjNode == kpTargetNode)
				{
					// Resets the adjacent Node
					pAdjNode->reset();

					// Sets adj parent to the current node
					pAdjNode->setParent(pCurrentNode);
					// Sets the adjNode's heuristic as the manhattan distance from the Node to the target
					pAdjNode->setH(distance(pAdjNode, kpTargetNode));
					// Sets the adjNode's G value
					pAdjNode->setG(calcG(pCurrentNode, pAdjNode));
					// Sets the adjNode's F value (G+H)
					pAdjNode->setF(pAdjNode->getG() + pAdjNode->getH());

					// Destination found, create path
					queuePath(pAdjNode);
					sf::err() << "[PATHFINDING] Generating path... Finished." << std::endl;
					bPathFound = true;
					return;
				}

				// Else If adjNode is on the open list already
				else if (nodeInVector(pAdjNode, pOpenNodes))
				{
					// If (total movement cost to adjcent node through current Node) is less than (total movement cost to adjNode)
					if (pCurrentNode->getG() + Utils::magnitude(pCurrentNode->getPosition() - pAdjNode->getPosition()) < pAdjNode->getG())
					{
						// Sets adj parent to the closed node
						pAdjNode->setParent(pCurrentNode);
					}
				}

				// Else if adjNode is on the closed list
				else if (nodeInVector(pAdjNode, pClosedNodes)) { /* Ignores Nodes already processed */ }

				// Else: adjNode is accessible, not the destination and is not on the open or closed list
				else
				{
					// Resets the adjacent Node
					pAdjNode->reset();

					// Sets adj parent to the current node
					pAdjNode->setParent(pCurrentNode);
					// Sets the adjNode's heuristic as the manhattan distance from the Node to the target
					pAdjNode->setH(distance(pAdjNode, kpTargetNode));
					// Sets the adjNode's G value
					pAdjNode->setG(calcG(pCurrentNode, pAdjNode));
					// Sets the adjNode's F value (G+H)
					pAdjNode->setF(pAdjNode->getG() + pAdjNode->getH());

					// Add adjacent nodes to open list
					pOpenNodes.push_back(pAdjNode);
				}
			}

			///////////////////// Determining Next Closed Node /////////////////////

			// If there is an open list
			if (!pOpenNodes.empty())
			{
				// Variable to store current Node with smallest F value
				std::shared_ptr<Node> pSmallestFNode = pOpenNodes.back(); // Sets starting Node with smallest F value to the first Node in the vector

				// For all open nodes
				for (std::shared_ptr<Node> pNode : pOpenNodes)
				{
					// If Node.F is less than currSmallest->f
					if (pNode->getF() < pSmallestFNode->getF())
					{
						// nodeWithSmallestF replaced with this Node
						pSmallestFNode.swap(pNode);
					}
					// Else If two Nodes have the same F value
					else if (pNode->getF() == pSmallestFNode->getF())
					{
						// The Node with lowest H is set
						if (pNode->getH() < pSmallestFNode->getH()) pSmallestFNode.swap(pNode);
					}
				}

				// Adds Node with smallest F value to the closed list
				pClosedNodes.push_back(pSmallestFNode);

				// Removes the Node with the smalled F value from the open list
				std::vector<std::shared_ptr<Node>> pNewOpenNodes;
				for (std::shared_ptr<Node> pOpenNode : pOpenNodes)
				{
					// If the openNode doesn't have the smallest F value
					if (pOpenNode != pSmallestFNode)
					{
						// Pushes onto new open list
						pNewOpenNodes.push_back(pOpenNode);
					}
				}
				// Replaces open list with new edited copy
				pOpenNodes.swap(pNewOpenNodes);
			}
			// Else the open list is empty
			else
			{
				sf::err() << "[PATHFINDING] Generating path... Error - No path found." << std::endl;
				return;
			}
		}
	}

	// Else Nodes have not been intitialised
	else { sf::err() << "[PATHFINDING] Generating path... Error - Nodes not intitialised." << std::endl; }
}

// std::shared_ptr<Node>: Detemines the Node that a given position falls within
std::shared_ptr<Node> Pathfinding::nodeFromPos(const sf::Vector2f kPosition)
{
	// If Nodes exist
	if (!m_pNodes.empty())
	{
		// Determines the column that the Node is in
		unsigned int uiNodeCol = (unsigned int)floor(kPosition.x / m_fNodeDiameter);

		// Determines the row that the Node is in
		unsigned int uiNodeRow = (unsigned int)floor(kPosition.y / m_fNodeDiameter);

		// Determines the Index using the column and row
		unsigned int uiNodeIndex = uiNodeCol + (m_uiNodeCols * uiNodeRow);

		// If the index is within the vector
		if (uiNodeIndex < m_pNodes.size())
		{
			// Returns the Node
			return m_pNodes.at(uiNodeIndex);
		}
	}

	// Nodes don't exist: return nullptr
	return nullptr;
}

// std::shared_ptr<Node>: Detemines the closest accessible Node to a given position
std::vector<std::shared_ptr<Node>> Pathfinding::perimeterNodes(const sf::Vector2f kPosition)
{
	// Declares vector to store the result
	std::vector<std::shared_ptr<Node>> pResultNodes;

	// If Nodes exist
	if (!m_pNodes.empty())
	{
		// If Node at the position is accessible
		if (nodeFromPos(kPosition)->isAccessible())
		{
			pResultNodes.push_back(nodeFromPos(kPosition)); // Adds Node at position to result vector
			return pResultNodes; // Returns vector with single Node within
		}

		// Else - Node at position is inaccessible
		else
		{
			// Declares vectors to store nodes to check and checked nodes
			std::vector<std::shared_ptr<Node>> pOpenNodes;
			std::vector<std::shared_ptr<Node>> pClosedNodes;

			// Add Node the position to closed list - It's been checked
			pClosedNodes.push_back(nodeFromPos(kPosition));

			// Bool for whether a Node has been found
			bool bNodeFound = false;

			// While a Node has not been found
			while (!bNodeFound)
			{
				// Current Node being tested
				std::shared_ptr<Node> pCurrentNode = pClosedNodes.back();

				// Get all adjacent nodes
				std::vector<std::shared_ptr<Node>> pAdjNodes = adjacentNodes(pCurrentNode, false);

				// For all adjacent nodes
				for (std::shared_ptr<Node> pAdjNode : pAdjNodes)
				{
					// If adjNode is accessible
					if (pAdjNode->isAccessible()) 
					{
						// If Node isn't diagonal to currentNode
						if (
							// If Node is Left of CurrentNode
							(pAdjNode->getIndex() == pCurrentNode->getIndex() - 1) ||
							// If Node is Right of CurrentNode
							(pAdjNode->getIndex() == pCurrentNode->getIndex() + 1) ||
							// If Node is Above of CurrentNode
							(pAdjNode->getIndex() == pCurrentNode->getIndex() - m_uiNodeCols) ||
							// If Node is Below of CurrentNode
							(pAdjNode->getIndex() == pCurrentNode->getIndex() + m_uiNodeCols)
						)
						{
							// Add adjacent node to resultant list
							pResultNodes.push_back(pAdjNode);
						}
					}
					
					// Else if adjNode is on the closed list
					else if (nodeInVector(pAdjNode, pClosedNodes)) { /* Ignores Nodes already processed */ }

					// Else: adjNode is inaccessible and not on the closed list
					else
					{
						// Add adjacent nodes to open list
						pOpenNodes.push_back(pAdjNode);
					}
				}

				// If there is an open list
				if (!pOpenNodes.empty())
				{
					// Adds the first open Node to the closed list
					pClosedNodes.push_back(pOpenNodes.front());

					// Removes the Node now on the closed list from the open list
					std::vector<std::shared_ptr<Node>> pNewOpenNodes;
					for (std::shared_ptr<Node> pOpenNode : pOpenNodes)
					{
						// If the openNode isn't the Node just placed onto the closed list
						if (pOpenNode != pClosedNodes.back())
						{
							// Pushes onto new open list
							pNewOpenNodes.push_back(pOpenNode);
						}
					}
					// Replaces open list with new edited copy
					pOpenNodes.swap(pNewOpenNodes);
				}
				// Else the open list is empty
				else
				{
					// Returns a vector of accessible Nodes around the perimeter of the position
					return pResultNodes;
				}
			}
		}
	}

	// Nodes don't exist: return empty vector
	return pResultNodes;
}

// Float: Calculates G value of a Node - Returns the calculated G value
float Pathfinding::calcG(const std::shared_ptr<Node> kpCurrentNode, const std::shared_ptr<Node> kpTargetNode)
{
	// Distance from the current Node and open Node
	float fDistToNode = Utils::magnitude(kpTargetNode->getPosition() - kpCurrentNode->getPosition());

	// Returns resultant G value (distance to next Node plus current Node's G)
	return fDistToNode + kpCurrentNode->getG();
}

// Bool: Detemines whether a Node is within a vector of Nodes - Returns whether the Node is in the vector
bool Pathfinding::nodeInVector(const std::shared_ptr<Node> kpNodeToFind, const std::vector<std::shared_ptr<Node>> kpVector)
{
	// If a Node in the vector is the nodeToFind: Return True
	for (std::shared_ptr<Node> pNode : kpVector)
	{
		if (pNode == kpNodeToFind) return true;
	}

	// Return False: nodeToFind not found
	return false;
}

// Float: Detemines the distance from one Node to another with the manhattan algorithm - Returns distance to the end Node
float Pathfinding::distance(const std::shared_ptr<Node> kpStartNode, const std::shared_ptr<Node> kpEndNode)
{
	// Number of Nodes needed horizontally to match destination
	float fXDist = (kpEndNode->getPosition().x - kpStartNode->getPosition().x) / m_fNodeDiameter;

	// Number of Nodes needed vertically to match destination
	float fYDist = (kpEndNode->getPosition().y - kpStartNode->getPosition().y) / m_fNodeDiameter;

	// Combines the distances in the x and y axis
	float fDistance = abs(fXDist) + abs(fYDist);

	// Returns the combined distances, bound to positive
	return fDistance;
}

// std::vector<std::shared_ptr<Node>>: Generates a vector of adjacent Nodes that can be pathed to - Returns accessible Nodes adjacent to the given Node
std::vector<std::shared_ptr<Node>> Pathfinding::adjacentNodes(const std::shared_ptr<Node> kpNode, const bool kbStrictDiagonal)
{
	// Defines vector of Nodes to store adjacent Nodes that are identified
	std::vector<std::shared_ptr<Node>> adjNodes;

	// If Node exists
	if (kpNode != nullptr)
	{
		// Declares Node ptr to hold adjacent Nodes as they're processed
		std::shared_ptr<Node> adjNode;

		// For 8 cycles
		for (unsigned int i = 1; i <= 8; i++)
		{
			switch (i)
			{
				case 1:
				{
					// x o o
					// o n o
					// o o o
					adjNode = nodeFromPos(sf::Vector2f(kpNode->getPosition().x - m_fNodeDiameter, kpNode->getPosition().y - m_fNodeDiameter));

					// If Node at position exists
					if (adjNode != nullptr)
					{
						// If diagonal pathing is strict
						if (kbStrictDiagonal)
						{
							// Nodes used to check diagonal accessibility
							// x o o
							// c n o
							// o o o
							std::shared_ptr<Node> checkNode1 = nodeFromPos(sf::Vector2f(kpNode->getPosition().x - m_fNodeDiameter, kpNode->getPosition().y));
							// x c o
							// o n o
							// o o o
							std::shared_ptr<Node> checkNode2 = nodeFromPos(sf::Vector2f(kpNode->getPosition().x, kpNode->getPosition().y - m_fNodeDiameter));

							// If checkNode1 Node exists and is not accessible
							if ((checkNode1 != nullptr) && !(checkNode1->isAccessible())) {}

							// Else If checkNode2 Node exists and is not accessible
							else if ((checkNode2 != nullptr) && !(checkNode2->isAccessible())) {}

							// Both checkNodes exist and are accessible
							else
							{
								// Adds adjacent Node to vector
								adjNodes.push_back(adjNode);
							}
						}
						else
						{
							// Adds adjacent Node to vector
							adjNodes.push_back(adjNode);
						}
					}
				} break;

				case 2:
				{
					// o x o
					// o n o
					// o o o
					adjNode = nodeFromPos(sf::Vector2f(kpNode->getPosition().x, kpNode->getPosition().y - m_fNodeDiameter));

					// If Node at position exists
					if (adjNode != nullptr)
					{
						// Adds adjacent Node to vector
						adjNodes.push_back(adjNode);
					}
				} break;

				case 3:
				{
					// o o x
					// o n o
					// o o o
					adjNode = nodeFromPos(sf::Vector2f(kpNode->getPosition().x + m_fNodeDiameter, kpNode->getPosition().y - m_fNodeDiameter));

					// If Node at position exists
					if (adjNode != nullptr)
					{
						// If diagonal pathing is strict
						if (kbStrictDiagonal)
						{
							// Nodes used to check diagonal accessibility
							// o c x
							// o n o
							// o o o
							std::shared_ptr<Node> checkNode1 = nodeFromPos(sf::Vector2f(kpNode->getPosition().x, kpNode->getPosition().y - m_fNodeDiameter));
							// o o x
							// o n c
							// o o o
							std::shared_ptr<Node> checkNode2 = nodeFromPos(sf::Vector2f(kpNode->getPosition().x + m_fNodeDiameter, kpNode->getPosition().y));

							// If checkNode1 Node exists and is not accessible
							if ((checkNode1 != nullptr) && !(checkNode1->isAccessible())) {}

							// Else If checkNode2 Node exists and is not accessible
							else if ((checkNode2 != nullptr) && !(checkNode2->isAccessible())) {}

							// Both checkNodes exist and are accessible
							else
							{
								// Adds adjacent Node to vector
								adjNodes.push_back(adjNode);
							}
						}
						else
						{
							// Adds adjacent Node to vector
							adjNodes.push_back(adjNode);
						}
					}
				} break;

				case 4:
				{
					// o o o
					// o n x
					// o o o
					adjNode = nodeFromPos(sf::Vector2f(kpNode->getPosition().x + m_fNodeDiameter, kpNode->getPosition().y));

					// If Node at position exists
					if (adjNode != nullptr)
					{
						// Adds adjacent Node to vector
						adjNodes.push_back(adjNode);
					}
				} break;

				case 5:
				{
					// o o o
					// o n o
					// o o x
					adjNode = nodeFromPos(sf::Vector2f(kpNode->getPosition().x + m_fNodeDiameter, kpNode->getPosition().y + m_fNodeDiameter));

					// If Node at position exists
					if (adjNode != nullptr)
					{
						// If diagonal pathing is strict
						if (kbStrictDiagonal)
						{
							// Nodes used to check diagonal accessibility
							// o o o
							// o n c
							// o o x
							std::shared_ptr<Node> checkNode1 = nodeFromPos(sf::Vector2f(kpNode->getPosition().x + m_fNodeDiameter, kpNode->getPosition().y));
							// o o o
							// o n o
							// o c x
							std::shared_ptr<Node> checkNode2 = nodeFromPos(sf::Vector2f(kpNode->getPosition().x, kpNode->getPosition().y + m_fNodeDiameter));

							// If checkNode1 Node exists and is not accessible
							if ((checkNode1 != nullptr) && !(checkNode1->isAccessible())) {}

							// Else If checkNode2 Node exists and is not accessible
							else if ((checkNode2 != nullptr) && !(checkNode2->isAccessible())) {}

							// Both checkNodes exist and are accessible
							else
							{
								// Adds adjacent Node to vector
								adjNodes.push_back(adjNode);
							}
						}
						else
						{
							// Adds adjacent Node to vector
							adjNodes.push_back(adjNode);
						}
					}
				} break;

				case 6:
				{
					// o o o
					// o n o
					// o x o
					adjNode = nodeFromPos(sf::Vector2f(kpNode->getPosition().x, kpNode->getPosition().y + m_fNodeDiameter));

					// If Node at position exists
					if (adjNode != nullptr)
					{
						// Adds adjacent Node to vector
						adjNodes.push_back(adjNode);
					}
				} break;

				case 7:
				{
					// o o o
					// o n o
					// x o o
					adjNode = nodeFromPos(sf::Vector2f(kpNode->getPosition().x - m_fNodeDiameter, kpNode->getPosition().y + m_fNodeDiameter));

					// If Node at position exists
					if (adjNode != nullptr)
					{
						// If diagonal pathing is strict
						if (kbStrictDiagonal)
						{
							// Nodes used to check diagonal accessibility
							// o o o
							// o n o
							// x c o
							std::shared_ptr<Node> checkNode1 = nodeFromPos(sf::Vector2f(kpNode->getPosition().x, kpNode->getPosition().y + m_fNodeDiameter));
							// o o o
							// c n o
							// x o o
							std::shared_ptr<Node> checkNode2 = nodeFromPos(sf::Vector2f(kpNode->getPosition().x - m_fNodeDiameter, kpNode->getPosition().y));

							// If checkNode1 Node exists and is not accessible
							if ((checkNode1 != nullptr) && !(checkNode1->isAccessible())) {}

							// Else If checkNode2 Node exists and is not accessible
							else if ((checkNode2 != nullptr) && !(checkNode2->isAccessible())) {}

							// Both checkNodes exist and are accessible
							else
							{
								// Adds adjacent Node to vector
								adjNodes.push_back(adjNode);
							}
						}
						else
						{
							// Adds adjacent Node to vector
							adjNodes.push_back(adjNode);
						}
					}
				} break;

				case 8:
				{
					// o o o
					// x n o
					// o o o
					adjNode = nodeFromPos(sf::Vector2f(kpNode->getPosition().x - m_fNodeDiameter, kpNode->getPosition().y));

					// If Node at position exists
					if (adjNode != nullptr)
					{
						// Adds adjacent Node to vector
						adjNodes.push_back(adjNode);
					}
				} break;
			}
		}
	}

	return adjNodes;
}

// Void: Forms a queue of Nodes to the given Node
void Pathfinding::queuePath(std::shared_ptr<Node> pTargetNode)
{
	// Vector for stack of Nodes to be used in path
	std::vector<std::shared_ptr<Node>> pNodes;

	// Pushes destination Node onto vector
	pNodes.push_back(pTargetNode);

	// While Node has a parent
	while (pTargetNode->getParent() != nullptr)
	{
		// Make the Node it's Parent
		pTargetNode = pTargetNode->getParent();
		// Push the parent onto the path of Nodes
		pNodes.push_back(pTargetNode);
	}

	// pNodes is not empty
	while (!pNodes.empty())
	{
		// Pushes the position of node onto path
		m_path.push(pNodes.back()->getPosition());
		// Pops Node off Vector
		pNodes.pop_back();
	}
}

// Void: Draws the path to a display
void Pathfinding::draw(sf::RenderTarget& target)
{
	// If Nodes initialised
	if (m_bNodesInit)
	{
		///////////////////// PATH /////////////////////
		// Declares line and colour
		sf::Vertex line[2];
		sf::Color colour = sf::Color(255, 0, 0, 255);
		// Duplicates the path queue
		std::queue<sf::Vector2f> pathDupe = m_path;

		// If there is a queue
		if (!pathDupe.empty())
		{
			// Sets the last point to the first in the queue
			sf::Vector2f lastPoint = pathDupe.front();

			// Sets the first point of the line at the position in front of the queue
			line[0] = sf::Vertex(lastPoint, colour);

			// Sets the second point of the line at the position of the robot
			line[1] = sf::Vertex(m_pColonist->getPosition(), colour);

			// Draws the line to target
			target.draw(line, 2, sf::Lines);

			// For every point in the path queue
			while (!pathDupe.empty())
			{
				// Sets the first point of the line to the position of the last position drawn
				line[0] = sf::Vertex(lastPoint, colour);

				// Sets the second point of the line at the position in front of the queue
				line[1] = sf::Vertex(pathDupe.front(), colour);

				lastPoint = pathDupe.front();
				pathDupe.pop(); // Removes the point from the queue

				// Draws the line to target
				target.draw(line, 2, sf::Lines);
			}
		}

		///////////////////// NODES /////////////////////
		// Rectangle shape for drawing
		sf::RectangleShape rectShape;
		rectShape.setSize(sf::Vector2f(m_fNodeDiameter, m_fNodeDiameter)); // Size of Node
		rectShape.setFillColor(sf::Color(0, 0, 0, 0)); // Transparent
		rectShape.setOutlineColor(sf::Color(255, 0, 0, 255)); // Red
		rectShape.setOutlineThickness(1.f);
		rectShape.setOrigin(rectShape.getSize()*0.5f); // Origin center

		// For every Node
		for (std::shared_ptr<Node> pNode : m_pNodes)
		{
			// If Node is inaccessible
			if (!pNode->isAccessible())
			{
				// Moves rect to Node position
				rectShape.setPosition(pNode->getPosition());

				// Draws rect to target
				target.draw(rectShape);
			}
		}
	}
}