/**
@file pathfinding.cpp
*/

// Imports
#include "pathfinding.h"

// Constructor
Pathfinding::Pathfinding(Colonist * pColonist, const std::shared_ptr<Environment> kpEnvironment)
{
	m_pColonist = std::shared_ptr<Colonist>(pColonist);
	m_pEnv = kpEnvironment;

	sf::err() << "[PATHFINDING] Generating nodes for A* pathfinding..." << std::endl;

	// Divides the map size by the diameter of the Nodes to create a required number of columns and rows in the grid
	m_uiNodeCols = floor(m_pEnv->getSize().x / m_fNodeDiameter);
	m_uiNodeRows = floor(m_pEnv->getSize().y / m_fNodeDiameter);

	// For the number of Nodes needed for the width
	for (unsigned int x = 1; x <= m_uiNodeCols; x++)
	{
		// For the number of Nodes needed for the height
		for (unsigned int y = 1; y <= m_uiNodeRows; y++)
		{
			// Pushes a new Node onto vector member of Nodes
			m_pNodes.push_back(std::shared_ptr<Node>(new Node(sf::Vector2f((m_fNodeDiameter * x) - m_fNodeRadius, (m_fNodeDiameter * y) - m_fNodeRadius), m_pNodes.size())));
		}
	}

	// Flags Nodes as initialised
	m_bNodesInit = true;

	sf::err() << "[PATHFINDING] Generating nodes for A* pathfinding... Finished." << std::endl;
}

// Calculates which Nodes are accessible
void Pathfinding::calcAccess()
{
	sf::err() << "[PATHFINDING] Calculating node accessibility..." << std::endl;

	// TEMPORARY - Need memories to determine inaccessible nodes

	sf::err() << "[PATHFINDING] Calculating node accessibility... Finished." << std::endl;
}

// Creates a path to target Node
void Pathfinding::createPathTo(std::shared_ptr<Node> targetNode)
{
	sf::err() << "[PATHFINDING] Generating path..." << std::endl;

	// If Nodes initialised
	if (m_bNodesInit)
	{
		///////////////////// Creating Open and Closed Lists /////////////////////

		// Declares vectors to store nodes to check and checked nodes
		std::vector<std::shared_ptr<Node>> openNodes;
		std::vector<std::shared_ptr<Node>> closedNodes;

		// If Node closest to your current location exists
		if (nodeFromPos(m_pColonist->getPosition()) != nullptr)
		{
			// Add Node at your current location to closed list
			closedNodes.push_back(nodeFromPos(m_pColonist->getPosition()));
		}
		// There is no Node 
		else
		{
			sf::err() << "[PATHFINDING] Generating path... Error - No available node." << std::endl;
			return;
		}

		///////////////////// Calculating The Path /////////////////////

		// Bool for whether a path was been found
		bool bPathFound = false;

		// While a path has not been found
		while (!bPathFound)
		{
			///////////////////// Setting Up The Current Node /////////////////////

			// Current Node being tested
			std::shared_ptr<Node> currentNode = closedNodes.back();

			// Resets the current Node
			currentNode->reset();
			// Sets the currentNode's heuristic as the manhattan distance from the Node to the target
			currentNode->setH(distance(currentNode, targetNode));

			// If Current Node is actually the Target Node
			if (currentNode == targetNode)
			{
				sf::err() << "[PATHFINDING] Generating path... CurrentNode equal to TargetNode." << std::endl;
				bPathFound = true;
				return;
			}

			///////////////////// Calculating Open Adjacent Nodes /////////////////////

			// Get all adjacent nodes
			std::vector<std::shared_ptr<Node>> adjNodes = getAdjacentNodes(currentNode);

			// For all adjacent nodes
			for (std::shared_ptr<Node> adjNode : adjNodes)
			{
				// Resets the adjacent Node
				adjNode->reset();
				// Calculates the adjNode heuristic
				adjNode->setH(distance(adjNode, targetNode));

				// If adjNode is inaccessible
				if (!adjNode->isAccessible()) {}

				// Else If adjNode is the target destination
				else if (adjNode == targetNode)
				{
					// Sets adj parent to the current node
					adjNode->setParent(currentNode);

					// Create G and F value for the adj Node
					adjNode->setG(calcG(currentNode, adjNode));
					// F = G+H
					adjNode->setF(adjNode->getG() + adjNode->getH());

					// Destination found, create path
					queuePath(adjNode);
					sf::err() << "[PATHFINDING] Generating path... Finished." << std::endl;
					bPathFound = true;
					return;
				}

				// Else If adjNode is on the open list already
				else if (nodeInVector(adjNode, openNodes))
				{
					// If (total movement cost to adjcent node through current Node) is less than (total movement cost to adjNode)
					if (currentNode->getG() + Utils::magnitude(currentNode->getPosition() - adjNode->getPosition()) < adjNode->getG())
					{
						// Sets adj parent to the closed node
						adjNode->setParent(currentNode);
					}
				}

				// Else if adjNode is on the closed list
				else if (nodeInVector(adjNode, closedNodes)) {}

				// Otherwise
				else
				{
					// Parent the open node to current node
					adjNode->setParent(currentNode);

					// Create G and F value for each open node
					adjNode->setG(calcG(currentNode, adjNode));

					// F = G+H
					adjNode->setF(adjNode->getG() + adjNode->getH());

					// Add adjacent nodes to open list
					openNodes.push_back(adjNode);
				}
			}

			///////////////////// Determining Next Closed Node /////////////////////

			// If there is an open list
			if (!openNodes.empty())
			{
				// Variable to store current Node with smallest F value
				std::shared_ptr<Node> nodeWithSmallestF = openNodes.at(0); // Sets starting Node with smallest F value to the first Node in the vector

				// For all open nodes
				for (std::shared_ptr<Node> node : openNodes)
				{
					// If Node.F is less than currSmallest->f
					if (node->getF() < nodeWithSmallestF->getF())
					{
						// nodeWithSmallestF replaced with this Node
						nodeWithSmallestF.swap(node);
					}
					// Else If two Nodes have the same F value
					else if (node->getF() == nodeWithSmallestF->getF())
					{
						// The Node with lowest H is set
						if (node->getH() < nodeWithSmallestF->getH()) nodeWithSmallestF.swap(node);
					}
				}

				// Adds Node with smallest F value to the closed list
				closedNodes.push_back(nodeWithSmallestF);

				// Removes the Node with the smalled F value from the open list
				std::vector<std::shared_ptr<Node>> newOpenNodes;
				for (std::shared_ptr<Node> openNode : openNodes)
				{
					// If the openNode doesn't have the smallest F value
					if (openNode != nodeWithSmallestF)
					{
						// Pushes onto new open list
						newOpenNodes.push_back(openNode);
					}
				}
				// Replaces open list with new edited copy
				openNodes.swap(newOpenNodes);
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

// Returns the Node that a given position falls within
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

// Calculates G value of a Node
float Pathfinding::calcG(std::shared_ptr<Node> currentNode, std::shared_ptr<Node> targetNode)
{
	// Distance from the current Node and open Node
	double dDistToNode = Utils::magnitude(targetNode->getPosition() - currentNode->getPosition());

	// Returns resultant G value (distance to next Node plus current Node's G)
	return dDistToNode + currentNode->getG();
}

// Returns whether a Node is within a vector of Nodes
bool Pathfinding::nodeInVector(std::shared_ptr<Node> nodeToFind, std::vector<std::shared_ptr<Node>> vector)
{
	// If a Node in the vector is the nodeToFind: Return True
	for (std::shared_ptr<Node> node : vector)
	{
		if (node == nodeToFind) return true;
	}

	// Return False: nodeToFind not found
	return false;
}

// Returns the distance from one Node to another
int Pathfinding::distance(std::shared_ptr<Node> startNode, std::shared_ptr<Node> endNode)
{
	// Number of Nodes needed horizontally to match destination
	int xDist = (endNode->getPosition().x - startNode->getPosition().x) / m_fNodeDiameter;

	// Number of Nodes needed vertically to match destination
	int yDist = (endNode->getPosition().y - startNode->getPosition().y) / m_fNodeDiameter;

	// Combines the distances in the x and y axis
	int iDistance = abs(xDist) + abs(yDist);

	// Returns the combined distances, bound to positive
	return iDistance;
}

// Returns a vector of accessible adjacent Nodes
std::vector<std::shared_ptr<Node>> Pathfinding::getAdjacentNodes(std::shared_ptr<Node> node)
{
	// Defines vector of Nodes to store adjacent Nodes that are identified
	std::vector<std::shared_ptr<Node>> adjNodes;

	// If Node exists
	if (node != nullptr)
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
				adjNode = nodeFromPos(sf::Vector2f(node->getPosition().x - m_fNodeDiameter, node->getPosition().y - m_fNodeDiameter));

				// If Node at position exists
				if (adjNode != nullptr)
				{
					// Nodes used to check diagonal accessibility
					// x o o
					// c n o
					// o o o
					std::shared_ptr<Node> checkNode1 = nodeFromPos(sf::Vector2f(node->getPosition().x - m_fNodeDiameter, node->getPosition().y));
					// x c o
					// o n o
					// o o o
					std::shared_ptr<Node> checkNode2 = nodeFromPos(sf::Vector2f(node->getPosition().x, node->getPosition().y - m_fNodeDiameter));

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
			} break;

			case 2:
			{
				// o x o
				// o n o
				// o o o
				adjNode = nodeFromPos(sf::Vector2f(node->getPosition().x, node->getPosition().y - m_fNodeDiameter));

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
				adjNode = nodeFromPos(sf::Vector2f(node->getPosition().x + m_fNodeDiameter, node->getPosition().y - m_fNodeDiameter));

				// If Node at position exists
				if (adjNode != nullptr)
				{
					// Nodes used to check diagonal accessibility
					// o c x
					// o n o
					// o o o
					std::shared_ptr<Node> checkNode1 = nodeFromPos(sf::Vector2f(node->getPosition().x, node->getPosition().y - m_fNodeDiameter));
					// o o x
					// o n c
					// o o o
					std::shared_ptr<Node> checkNode2 = nodeFromPos(sf::Vector2f(node->getPosition().x + m_fNodeDiameter, node->getPosition().y));

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
			} break;

			case 4:
			{
				// o o o
				// o n x
				// o o o
				adjNode = nodeFromPos(sf::Vector2f(node->getPosition().x + m_fNodeDiameter, node->getPosition().y));

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
				adjNode = nodeFromPos(sf::Vector2f(node->getPosition().x + m_fNodeDiameter, node->getPosition().y + m_fNodeDiameter));

				// If Node at position exists
				if (adjNode != nullptr)
				{
					// Nodes used to check diagonal accessibility
					// o o o
					// o n c
					// o o x
					std::shared_ptr<Node> checkNode1 = nodeFromPos(sf::Vector2f(node->getPosition().x + m_fNodeDiameter, node->getPosition().y));
					// o o o
					// o n o
					// o c x
					std::shared_ptr<Node> checkNode2 = nodeFromPos(sf::Vector2f(node->getPosition().x, node->getPosition().y + m_fNodeDiameter));

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
			} break;

			case 6:
			{
				// o o o
				// o n o
				// o x o
				adjNode = nodeFromPos(sf::Vector2f(node->getPosition().x, node->getPosition().y + m_fNodeDiameter));

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
				adjNode = nodeFromPos(sf::Vector2f(node->getPosition().x - m_fNodeDiameter, node->getPosition().y + m_fNodeDiameter));

				// If Node at position exists
				if (adjNode != nullptr)
				{
					// Nodes used to check diagonal accessibility
					// o o o
					// o n o
					// x c o
					std::shared_ptr<Node> checkNode1 = nodeFromPos(sf::Vector2f(node->getPosition().x, node->getPosition().y + m_fNodeDiameter));
					// o o o
					// c n o
					// x o o
					std::shared_ptr<Node> checkNode2 = nodeFromPos(sf::Vector2f(node->getPosition().x - m_fNodeDiameter, node->getPosition().y));

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
			} break;

			case 8:
			{
				// o o o
				// x n o
				// o o o
				adjNode = nodeFromPos(sf::Vector2f(node->getPosition().x - m_fNodeDiameter, node->getPosition().y));

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

// Forms a queue of Nodes to the given Node
void Pathfinding::queuePath(std::shared_ptr<Node> targetNode)
{
	// Vector for stack of Nodes to be used in path
	std::vector<std::shared_ptr<Node>> pNodes;

	// Pushes destination Node onto vector
	pNodes.push_back(targetNode);

	// While Node has a parent
	while (targetNode->getParent() != nullptr)
	{
		// Make the Node it's Parent
		targetNode = targetNode->getParent();
		// Push the parent onto the path of Nodes
		pNodes.push_back(targetNode);
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

// Draws pathfinding representation to a display
void Pathfinding::draw(sf::RenderTarget& target)
{
	// If Nodes initialised
	if (m_bNodesInit)
	{
		// PATH
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
	}
}