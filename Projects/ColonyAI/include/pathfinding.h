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

	std::queue<sf::Vector2f> m_path; //!< Queue of positions forming a route

	std::shared_ptr<Colonist> m_pColonist; //!< Pointer to the Colonist that is utilising the Pathfinding
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
	/// \return The calculated G value
	///
	///////////////////////////////////////////////// 
	float calcG(const std::shared_ptr<Node> kpCurrentNode, const std::shared_ptr<Node> kpTargetNode);

	/////////////////////////////////////////////////
	///
	/// \brief Detemines whether a Node is within a vector of Nodes
	///
	/// \return Whether the Node is in the vector
	///
	///////////////////////////////////////////////// 
	bool nodeInVector(const std::shared_ptr<Node> kpNodeToFind, const std::vector<std::shared_ptr<Node>> kpVector);

	/////////////////////////////////////////////////
	///
	/// \brief Detemines the distance from one Node to another with the manhattan algorithm
	///
	/// \return Distance to the end Node
	///
	///////////////////////////////////////////////// 
	float distance(const std::shared_ptr<Node> kpStartNode, const std::shared_ptr<Node> kpEndNode);

	/////////////////////////////////////////////////
	///
	/// \brief Generates a vector of accessible adjacent Nodes
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
	void queuePath(std::shared_ptr<Node> pNode);

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
	Pathfinding(Colonist * pColonist, const std::shared_ptr<Environment> kpEnvironment);

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
	///	\param kPosition A point in the Environment
	///	\param kfRadius The radius arounf the point that will be inaccessible
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void calcAccess(const sf::Vector2f kPosition, const float kfRadius);

	/////////////////////////////////////////////////
	///
	/// \brief Detemines the Node that a given position falls within
	///
	/// \return Node at position
	///
	///////////////////////////////////////////////// 
	std::shared_ptr<Node> nodeFromPos(const sf::Vector2f kPosition);

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
	/// \brief Draws the path to a display
	///
	/// \param target The RenderTarget to draw on
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void draw(sf::RenderTarget& target);
};

#endif