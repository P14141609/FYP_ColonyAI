/**
@file main.cpp
*/

/*! \mainpage
*
* Final Year Project - Colony-based AI
*/

// Imports 
// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

//!< Entry point for the application
int main()
{
	// Instantiates window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "ColonyAI - P14141609", sf::Style::Default);

	// Initialises a clock for the update loop
	sf::Clock clock;
	// Declares var to track elapsed time
	sf::Time elapsedTime;

	// While the window is open
	while (window.isOpen())
	{
		// Event object for windows event calls
		sf::Event event;
		while (window.pollEvent(event))
		{
			// If Closed event is called
			if (event.type == sf::Event::Closed)
			{
				// Closes window
				window.close();
			}

			// If KeyPressed event is called
			if (event.type == sf::Event::KeyPressed)
			{
				// If Esc is pressed
				if (event.key.code == sf::Keyboard::Escape)
				{
					// Closes window
					window.close();
				}
			}
		}

		// Gets elapsed time from clock
		elapsedTime = clock.getElapsedTime();

		// Triggers the update loop 128 times a second
		if (elapsedTime.asMilliseconds() > 1000/128)
		{
			// Restarts the clock
			clock.restart();

			// Update objects
			// TODO
		}

		// Draw objects
		// TODO

		// Displays the current frame
		window.display();
	}

	// Finishes 'main' with return
	return 0;
}