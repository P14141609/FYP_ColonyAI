#ifndef MEMORY_H
#define MEMORY_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/////////////////////////////////////////////////
///
/// \brief Class for holding Colonist Memory data
/// 
/////////////////////////////////////////////////
class Memory
{
private:

	float m_lTime; //!< Memory time
	sf::Vector2f m_position; //!< Memory position
	
	enum MemoryType { OBSTRUCTION, FOOD_SOURCE, WATER_SOURCE }; //!< Enum for Memory types
	MemoryType m_type; //!< Memory type

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	/// \param klTime Time of Memory creation
	/// \param kPosition Position of the Memory
	/// \param kType Type of Memory
	///
	///////////////////////////////////////////////// 
	Memory(const long klTime, const sf::Vector2f kPosition, const MemoryType kType);

	/////////////////////////////////////////////////
	///
	/// \brief Assigns a new value to the Memory time
	///
	/// \param klTime New time value
	///
	/// \return void
	///
	/////////////////////////////////////////////////
	void setTime(const long klTime) { m_lTime = klTime; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Memory time
	///
	/// \return The Memory time
	///
	///////////////////////////////////////////////// 
	float getTime() { return m_lTime; }

	/////////////////////////////////////////////////
	///
	/// \brief Assigns a new value to the Memory type
	///
	/// \param kType New type value
	///
	/// \return void
	///
	/////////////////////////////////////////////////
	void setType(const MemoryType kType) { m_type = kType; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Memory type
	///
	/// \return The Memory type
	///
	///////////////////////////////////////////////// 
	MemoryType getType() { return m_type; }

	/////////////////////////////////////////////////
	///
	/// \brief Returns the Memory position
	///
	/// \return The Memory position
	///
	///////////////////////////////////////////////// 
	sf::Vector2f getPosition() { return m_position; }
};

#endif