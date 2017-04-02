#pragma once
#ifndef PATHFINDING_H
#define PATHFINDING_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <memory>
#include "environment.h"
#include "node.h"
#include "utils.h"

/////////////////////////////////////////////////
///
/// \brief Class for pathfinding
/// 
/////////////////////////////////////////////////
class Pathfinding
{
private:

	std::queue<sf::Vector2f> m_path; //!< Queue of positions forming a route

	std::shared_ptr<Environment> m_pEnv; //!< Pointer to the Environment the Pathfinding is operating in

	std::vector<std::shared_ptr<Node>> m_pNodes; //!< Vector of Nodes

	bool m_bNodesInit = false; //!< Whether the Nodes have been initialised

	float m_fNodeDiameter = 20.0f; //!< Diameter of the Nodes
	float m_fNodeRadius = m_fNodeDiameter/2; //!< Radius of the Nodes

	unsigned int m_uiNodeCols; //!< Count for required columns of Nodes
	unsigned int m_uiNodeRows; //!< Count for required rows of Nodes
	
	/////////////////////////////////////////////////
	///
	/// \brief Calculates G value of a Node
	///
	/// \param kpCurrentNode The current Node to calculate G for
	/// \param kpTargetNode Node that proceeds the current Node in the path
	///
	/// \return The calculated G value
	///
	///////////////////////////////////////////////// 
	float calcG(const std::shared_ptr<Node> kpCurrentNode, const std::shared_ptr<Node> kpTargetNode);

	/////////////////////////////////////////////////
	///
	/// \brief Detemines whether a Node is within a vector of Nodes
	///
	/// \param kpNodeToFind Node to locate within vector
	/// \param kpVector Vector of Nodes to search through
	///
	/// \return Whether the Node is in the vector
	///
	///////////////////////////////////////////////// 
	bool nodeInVector(const std::shared_ptr<Node> kpNodeToFind, const std::vector<std::shared_ptr<Node>> kpVector);

	/////////////////////////////////////////////////
	///
	/// \brief Detemines the distance from one Node to another with the manhattan algorithm
	///
	/// \param kpStartNode Node to calculate distance from
	/// \param kpEndNode Node to calculate distance to
	///
	/// \return Distance to the end Node
	///
	///////////////////////////////////////////////// 
	float distance(const std::shared_ptr<Node> kpStartNode, const std::shared_ptr<Node> kpEndNode);

	/////////////////////////////////////////////////
	///
	/// \brief Generates a vector of adjacent Nodes that can be pathed to
	///
	/// \param kpNode Node to calculate surroundings with
	/// \param kbStrictDiagonal Boolean for whether to allow diagonal adjacency if it crosses another Node
	///
	/// \return Nodes adjacent to the given Node
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Node>> adjacentNodes(const std::shared_ptr<Node> kpNode, const bool kbStrictDiagonal);

	/////////////////////////////////////////////////
	///
	/// \brief Creates a queue of points from a given destination back to the current position
	///
	/// \param pNode Node to queue a path back from
	///
	/// \return A queue of positions from destination back to current position
	///
	///////////////////////////////////////////////// 
	std::queue<sf::Vector2f> queuePath(std::shared_ptr<Node> pNode);

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	///	\param kpEnvironment The Environment the Pathfinding is operating in
	///
	///////////////////////////////////////////////// 
	Pathfinding(const std::shared_ptr<Environment> kpEnvironment);

	/////////////////////////////////////////////////
	///
	/// \brief Creates a path to target Node
	///
	/// \param kCurrentPos Start point to path from
	/// \param kpTargetNode Node to route toward
	/// \param kbIgnoreAccess Whether to ignore Node (in)accessibility
	///
	/// \return A queue of points that represent a route
	///
	///////////////////////////////////////////////// 
	std::queue<sf::Vector2f> createPathTo(const sf::Vector2f kCurrentPos, const std::shared_ptr<Node> kpTargetNode, const bool kbIgnoreAccess);

	/////////////////////////////////////////////////
	///
	/// \brief Calculates which Nodes are accessible
	///
	///	\param kfColonistRadius The Colonist's radius for clearance calucating
	///	\param kPosition A point in the Environment
	///	\param kfRadius The radius around the point that will be inaccessible
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void calcAccess(const float kfColonistRadius, const sf::Vector2f kPosition, const float kfRadius);

	/////////////////////////////////////////////////
	///
	/// \brief Detemines the Node that a given position falls within
	///
	/// \param kPosition Position to calculate a local Node with
	///
	/// \return Node at position
	///
	///////////////////////////////////////////////// 
	std::shared_ptr<Node> nodeFromPos(const sf::Vector2f kPosition);

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Node at a given index with error checks
	///
	/// \param kuiIndex Index to get the Node with
	///
	/// \return Node at index
	///
	///////////////////////////////////////////////// 
	std::shared_ptr<Node> nodeFromIndex(const unsigned int kuiIndex);

	/////////////////////////////////////////////////
	///
	/// \brief Generates a perimeter of Nodes around a given position
	///
	/// \param kPosition Position to calculate perimeter around
	/// \param kfRadius Radius around the position to cap the search
	///
	/// \return Perimeter Nodes
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Node>> perimeterNodes(const sf::Vector2f kPosition, const float kfRadius);

	/////////////////////////////////////////////////
	///
	/// \brief Gets the vector of Nodes
	///
	/// \return Vector of Nodes
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Node>> getNodes() { return m_pNodes; }

	/////////////////////////////////////////////////
	///
	/// \brief Sets the path queue
	///
	/// \param Queue of positions
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setPath(std::queue<sf::Vector2f> kPath) { m_path = kPath; }

	/////////////////////////////////////////////////
	///
	/// \brief Gets the path queue
	///
	/// \return Queue of positions
	///
	///////////////////////////////////////////////// 
	std::queue<sf::Vector2f> getPath() { return m_path; }

	/////////////////////////////////////////////////
	///
	/// \brief Pops the first element off the path queue
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void popPath() { m_path.pop(); }

	/////////////////////////////////////////////////
	///
	/// \brief Pops off every element in the path queue
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void clearPath() 
	{
		// If path is not empty
		while (!m_path.empty())
		{
			// Pops element off the path
			m_path.pop();
		}
	}

	/////////////////////////////////////////////////
	///
	/// \brief Draws the path to a display
	///
	/// \param kColonistPos The Colonist's current position
	/// \param target The RenderTarget to draw on
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void draw(const sf::Vector2f kColonistPos, sf::RenderTarget& target);
};

#endif