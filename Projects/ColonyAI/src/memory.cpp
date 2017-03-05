/**
@file memory.cpp
*/

// Imports
#include "memory.h"

// Constructor
Memory::Memory(const long klTime, const sf::Vector2f kPosition, const MemoryType kType)
{
	// Sets member values to corresponding input
	m_lTime = klTime;
	m_position = kPosition;
	m_type = kType;
}