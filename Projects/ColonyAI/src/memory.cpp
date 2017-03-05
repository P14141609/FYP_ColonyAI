/**
@file memory.cpp
*/

// Imports
#include "memory.h"

// Constructor
Memory::Memory(const long klTime, const sf::Vector2f kPosition, const float kfRadius, const MemoryType kType)
{
	// Sets member values to corresponding input
	m_lTime = klTime;
	m_position = kPosition;
	m_fRadius = kfRadius;
	m_type = kType;
}