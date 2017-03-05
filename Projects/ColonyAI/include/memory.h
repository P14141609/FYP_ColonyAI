#ifndef MEMORY_H
#define MEMORY_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/////////////////////////////////////////////////
///
/// \brief 
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
};

#endif