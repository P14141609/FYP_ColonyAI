#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "entity.h"
#include "object.h"

/////////////////////////////////////////////////
///
/// \brief Class for level setup, interation and display
/// 
/////////////////////////////////////////////////
class Environment: public sf::Drawable
{
private:

	sf::Color m_bgColour; //!< Color for the Environment background

	std::vector<Entity> m_Entities; //!< Vector of Entities in the Environment
	std::vector<Object> m_Objects; //!< Vector of Objects in the Environment

	/////////////////////////////////////////////////
	///
	/// \brief Draws the Environment to the RenderTarget
	///
	/// \param target The RenderTarget to draw on
	/// \param states The corresponding RenderStates
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	
public:

	/////////////////////////////////////////////////
	///
	/// \brief Constructor
	///
	/// \param ksFilename The filename of an Environment file
	///
	///////////////////////////////////////////////// 
	Environment(const std::string ksFilename);

	/////////////////////////////////////////////////
	///
	/// \brief Updates the Environment and its contents with elapsed time
	///
	/// \param kfElapsedTime The time passed since last update in seconds
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void update(const float kfElapsedTime);
};

#endif