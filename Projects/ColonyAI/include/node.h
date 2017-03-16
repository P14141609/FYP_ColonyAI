#pragma once
#ifndef NODE_H
#define NODE_H

// Imports
#include <memory>
#include "utils.h"

/////////////////////////////////////////////////
///
/// \brief Class used in pathfinding for calculation
/// 
/////////////////////////////////////////////////
class Node
{
private:

	bool m_bAccessible = true; //!< Whether the Node is accessible for pathing

	unsigned int m_uiIndex; //!< Index position within a vector of Nodes

	sf::Vector2f m_position; //!< In-world coordinates

	std::shared_ptr<Node> m_pParent; //!< Previous Node in the path

	float m_fG = 0.f; //!< Total Movement Cost from start Node to this Node
	float m_fH = 0.f; //!< Distance to target Node
	float m_fF = 0.f; //!< Combined Heuristic

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Default Constructor
	///
	///////////////////////////////////////////////// 
	Node() {}

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	/// \param kPosition Node position
	/// \param kuiIndex Node index
	///
	///////////////////////////////////////////////// 
	Node(const sf::Vector2f kPosition, const unsigned int kuiIndex) 
	{
		// Sets member values to corresponding input
		m_position = kPosition;
		m_uiIndex = kuiIndex;
	}

	/////////////////////////////////////////////////
	///
	/// \brief Resets the Node ready for a new path
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void reset() 
	{ 
		// Clears the existing parent Node
		m_pParent = nullptr;

		// Clears existing G/H/F values
		m_fG = 0.f;
		m_fH = 0.f;
		m_fF = 0.f;
	}

	/////////////////////////////////////////////////
	///
	/// \brief Assigns a new value to the accessible member
	///
	/// \param kbValue New accessbility value
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setAccessible(const bool kbValue) { m_bAccessible = kbValue; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the accessibility of the Node
	///
	/// \return Whether the Node is accessible
	///
	///////////////////////////////////////////////// 
	bool isAccessible() { return m_bAccessible; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the index of the Node
	///
	/// \return The Node index value
	///
	///////////////////////////////////////////////// 
	unsigned int getIndex() { return m_uiIndex; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the position of the Node
	///
	/// \return The Node position
	///
	///////////////////////////////////////////////// 
	sf::Vector2f getPosition() { return m_position; }

	/////////////////////////////////////////////////
	///
	/// \brief Assigns a new Node to the parent member
	///
	/// \param kpParent New parent Node
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setParent(const std::shared_ptr<Node> kpParent) { m_pParent = kpParent; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the parent Node
	///
	/// \return The Node's parent Node
	///
	///////////////////////////////////////////////// 
	std::shared_ptr<Node> getParent() { return m_pParent; }

	/////////////////////////////////////////////////
	///
	/// \brief Assigns a new value to the G member
	///
	/// \param kfG New G value
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setG(const float kfG) { m_fG = kfG; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the G value of the Node
	///
	/// \return The Node G value
	///
	///////////////////////////////////////////////// 
	float getG() { return m_fG; }

	/////////////////////////////////////////////////
	///
	/// \brief Assigns a new value to the H member
	///
	/// \param kfH New H value
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setH(const float kfH) { m_fH = kfH; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the H value of the Node
	///
	/// \return The Node H value
	///
	///////////////////////////////////////////////// 
	float getH() { return m_fH; }

	/////////////////////////////////////////////////
	///
	/// \brief Assigns a new value to the F member
	///
	/// \param kfF New F value
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void setF(const float kfF) { m_fF = kfF; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the F value of the Node
	///
	/// \return The Node F value
	///
	///////////////////////////////////////////////// 
	float getF() { return m_fF; }
};

#endif