#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// Imports
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/////////////////////////////////////////////////
///
/// \brief Class for level setup, interation and display
/// 
/////////////////////////////////////////////////
class Environment: public sf::Drawable
{
private:
	
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
	/// \brief Default constructor
	///
	///////////////////////////////////////////////// 
	Environment() {};

	/////////////////////////////////////////////////
	///
	/// \brief Updates the Environment with elapsed time
	///
	/// \param kfElapsedTime The time passed since last update in seconds
	///
	/// \return void
	///
	///////////////////////////////////////////////// 
	void update(const float kfElapsedTime);
};

#endif