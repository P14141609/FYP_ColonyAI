#pragma once
#ifndef MEMORY_H
#define MEMORY_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "object.h"

class Object;

enum MemoryType { NULL_OBJECT, OBSTRUCTION, FOOD_ENTITY, FOOD_SOURCE, STONE_ENTITY, STONE_SOURCE, LUMBER_ENTITY, LUMBER_SOURCE, WATER_SOURCE }; //!< Enum for Memory types

/////////////////////////////////////////////////
///
/// \brief Class for holding Colonist Memory data
/// 
/////////////////////////////////////////////////
class Memory
{
private:

	long m_lTime; //!< Memory time
	MemoryType m_type; //!< Memory type
	std::shared_ptr<Object> m_pObject; //!< Memory Object

protected:

public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	/// \param klTime Time of Memory creation
	/// \param kType Type of Memory
	/// \param kpObject Object the Memory refers to
	///
	///////////////////////////////////////////////// 
	Memory(const long klTime, const std::shared_ptr<Object> kpObject, const MemoryType kType)
	{
		// Sets member values to corresponding input
		m_lTime = klTime;
		m_type = kType;
		m_pObject = kpObject;
	}

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
	/// \brief Gets the Memory time
	///
	/// \return The Memory time
	///
	///////////////////////////////////////////////// 
	long getTime() { return m_lTime; }

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
	/// \brief Gets the Memory type
	///
	/// \return The Memory type
	///
	///////////////////////////////////////////////// 
	MemoryType getType() { return m_type; }

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Memory Object
	///
	/// \return The Memory Object
	///
	///////////////////////////////////////////////// 
	std::shared_ptr<Object> getObject() { return m_pObject; }

	/////////////////////////////////////////////////
	///
	/// \brief Gets the Memory Object
	///
	/// \param kType Type to check for
	/// \param kpMemories Memory vector to check within
	///
	/// \return The Memory Object
	///
	///////////////////////////////////////////////// 
	static bool typeInMem(const MemoryType kType, const std::vector<std::shared_ptr<Memory>> kpMemories) 
	{
		// If a Type in the vector is the kType: Return True
		for (std::shared_ptr<Memory> pMemory : kpMemories)
		{
			if (pMemory->getType() == kType) return true;
		}

		// Return False: kType not found
		return false;
	}
};

#endif