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
#include "environment.h"

//!< Entry point for the application
int main()
{
	// String for storing user input
	std::string sInput;

	// Get user input for  whether to load or create while a valid answer has not been given
	do 
	{
		// Gets user input whether to load or create Environment
		std::cout << "Load or Create Environment file?: ";
		std::cin >> sInput;

		// Changes the input to all lowercase chars
		sInput = Utils::toLowercase(sInput);
	} 
	while (sInput != "load" && sInput != "create"); // Checks whether input is valid

	// Proceed with program
	// If 'Create' selected
	if (sInput == "create")
	{
		// TEMPORARY - For level editor
	}

	// If 'Load' selected
	else if (sInput == "load")
	{
		// Instantiates new Environment
		Environment environment = Environment();
		// Declares string for Environment directory
		std::string sEnvDir;

		// Get user input for EnvDirectory while a valid directory has not been given
		do 
		{
			// Gets user input for the Environment directory
			std::cout << "Select Environment: 'environments/YOURINPUT.cfg': ";
			std::cin >> sInput;

			// Defines Environment directory
			sEnvDir = "environments/" + sInput + ".cfg";
		} 
		while (!environment.loadFromFile(sEnvDir)); // Attempts to load an Evironment from file directory
		
		// Proceed with program
		// Intitialises a seed for rand()
		srand((unsigned int)time(NULL));

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
					// If Tab is pressed
					if (event.key.code == sf::Keyboard::Tab)
					{
						// Toggles debugging boolean
						g_bDebugging = !g_bDebugging;
						sf::err() << "[DEBUGGING] Debug Mode set to " << Utils::boolToStr(g_bDebugging) << "." << std::endl;
					}

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
			if (elapsedTime.asMilliseconds() > 1000 / 128)
			{
				// Restarts the clock
				clock.restart();

				// Updates environment with elapsed time
				environment.update(elapsedTime.asSeconds());
			}

			// Clears window making it entirely black
			window.clear(sf::Color(0, 0, 0, 255));

			// Sets view to size of environment
			window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)environment.getSize().x, (float)environment.getSize().y)));

			// Draws environment
			window.draw(environment);

			// Displays the current frame
			window.display();
		}
	}

	// Finishes 'main' with return
	return 0;
}