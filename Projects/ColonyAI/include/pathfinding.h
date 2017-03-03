#ifndef PATHFINDING_H
#define PATHFINDING_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <memory>
#include "colonist.h"
#include "environment.h"
#include "node.h"
#include "utils.h"

class Colonist;

/////////////////////////////////////////////////
///
/// \brief Class for pathfinding
/// 
/////////////////////////////////////////////////
class Pathfinding
{
private:

	std::vector<std::shared_ptr<Node>> m_pNodes; //!< Vector of Nodes

	std::queue<sf::Vector2f> m_path; //!< Queue of positions forming a route

	std::shared_ptr<Colonist> m_pColonist; //!< Pointer to the Colonist that is utilising the Pathfinding
	std::shared_ptr<Environment> m_pEnv; //!< Pointer to the Environment the Pathfinding is operating in

	float m_fNodeDiameter = 20.0f; //!< Diameter of the Nodes
	float m_fNodeRadius = m_fNodeDiameter/2; //!< Radius of the Nodes

	bool m_bNodesInit = false; //!< Whether the Nodes have been initialised


	/////////////////////////////////////////////////
	///
	/// \brief Generates all Nodes for pathfinding
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void createNodes();

	/////////////////////////////////////////////////
	///
	/// \brief Calculates G value of a Node
	///
	/// \return The calculated G value
	///
	///////////////////////////////////////////////// 
	float calcG(const std::shared_ptr<Node> kpCurrentNode, const std::shared_ptr<Node> kpTargetNode);

	/////////////////////////////////////////////////
	///
	/// \brief Returns whether a Node is within a vector of Nodes
	///
	/// \return Whether the Node is in the vector
	///
	///////////////////////////////////////////////// 
	bool nodeInVector(const std::shared_ptr<Node> kpNodeToFind, const std::vector<std::shared_ptr<Node>> kpVector);

	/////////////////////////////////////////////////
	///
	/// \brief Returns the distance from one Node to another with the manhattan algorithm
	///
	/// \return Distance to the end Node
	///
	///////////////////////////////////////////////// 
	int distance(const std::shared_ptr<Node> kpStartNode, const std::shared_ptr<Node> kpEndNode);

	/////////////////////////////////////////////////
	///
	/// \brief Returns a vector of accessible adjacent Nodes
	///
	/// \return Accessible Nodes adjacent to the given Node
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Node>> getAdjacentNodes(const std::shared_ptr<Node> kpNode);

	/////////////////////////////////////////////////
	///
	/// \brief Forms a queue of Nodes to the given Node
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void queuePath(const std::shared_ptr<Node> node);

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	///	\param pColonist The Colonist that is utilising the Pathfinding
	///	\param kpEnvironment The Environment the Pathfinding is operating in
	///
	///////////////////////////////////////////////// 
	Pathfinding(Colonist * kpColonist, const std::shared_ptr<Environment> kpEnvironment);

	/////////////////////////////////////////////////
	///
	/// \brief Creates a path to target Node
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void createPathTo(const std::shared_ptr<Node> kpTargetNode);

	/////////////////////////////////////////////////
	///
	/// \brief Calculates which Nodes are accessible
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void calcAccess();

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Node that a given position falls within
	///
	/// \return Node at position
	///
	///////////////////////////////////////////////// 
	std::shared_ptr<Node> nodeFromPos(const sf::Vector2f kPosition);

	/////////////////////////////////////////////////
	///
	/// \brief Returns the vector of Nodes
	///
	/// \return Vector of Nodes
	///
	///////////////////////////////////////////////// 
	std::vector<std::shared_ptr<Node>> getNodes() { return m_pNodes; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the path queue
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
	/// \brief Draws pathfinding representation to a display
	///
	/// \param target The RenderTarget to draw on
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void draw(sf::RenderTarget& target);
};

#endif