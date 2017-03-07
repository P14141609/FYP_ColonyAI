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

//!< Struct that holds window properties
struct WindowProperties
{
	//!< Constructor
	WindowProperties(const std::string ksTitle, const sf::Vector2u kSize)
	{
		// Sets member values to corresponding input
		m_sTitle = ksTitle;
		m_size = kSize;
	}

	bool m_bFullscreen = false; //!< Whether the window is in fullscreen mode
	std::string m_sTitle = ""; //!< The window title
	sf::Vector2u m_size = sf::Vector2u(1,1); //!< The window size
};

//!< Returns a View correctly letterboxed for the display
sf::View letterboxView(sf::View view, const sf::Vector2u kWindowSize)
{
	// Defines view aspect ratio
	float fViewRatio = (float)view.getSize().x / (float)view.getSize().y;
	// Defines window aspect ratio
	float fWindowRatio = (float)kWindowSize.x / (float)kWindowSize.y;

	// Defines vector for new view size
	sf::Vector2f newSize(1.0f, 1.0f);
	// Defines vector for new view position
	sf::Vector2f newPosition(0.0f, 0.0f);

	// Declares bool for whether the view needs letter boxing left&right: true or top&bottom: false
	bool bViewFitsHeight;

	// If the window aspect ratio is smaller than the view aspect ratio
	if (fWindowRatio < fViewRatio) bViewFitsHeight = false; // The view doesn't fit the fit the height
	// Else 
	else bViewFitsHeight = true; // The view fits the height

	// Fits height: modify view X axis
	if (bViewFitsHeight)
	{
		// Scales the width by the window aspect ratio 
		newSize.x = fViewRatio / fWindowRatio;
		// Moves the position across
		newPosition.x = (1 - newSize.x) / 2;
	}
	// Fits width: modify view Y axis
	else 
	{
		// Scales the height by the window aspect ratio 
		newSize.y = fWindowRatio / fViewRatio;
		// Moves the position down
		newPosition.y = (1 - newSize.y) / 2;
	}

	// Sets the view's viewport with newly configured params
	view.setViewport(sf::FloatRect(newPosition, newSize));

	// Returns letterboxed view
	return view;
}

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

		// Defines WindowsProperties with given title and size
		WindowProperties winProps("Colony-based AI in a 2D environment - Final Year Project - P14141609", sf::Vector2u(1280, 720));

		// Instantiates window
		sf::RenderWindow window(sf::VideoMode(winProps.m_size.x, winProps.m_size.y), winProps.m_sTitle.c_str(), sf::Style::Default);

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
					// If F11 is pressed
					if (event.key.code == sf::Keyboard::F11)
					{
						// Toggles fullscreen mode
						winProps.m_bFullscreen = !winProps.m_bFullscreen;

						// If now in fullscreen mode
						if (winProps.m_bFullscreen)
						{
							// Recreates the window with size that's the size of the display, standard title and style of none
							window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), winProps.m_sTitle.c_str(), sf::Style::None);
							// Positions window at 0,0
							window.setPosition(sf::Vector2i(0, 0));
						}
						// Else in windowed mode
						else
						{
							// Recreates the window with default size, standard title and default style
							window.create(sf::VideoMode(winProps.m_size.x, winProps.m_size.y), winProps.m_sTitle.c_str(), sf::Style::Default);
						}
					}

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

			// Defines the View of the Environment
			sf::View envView(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(environment.getSize())));
			
			// Sets view to size of environment
			window.setView(letterboxView(envView, window.getSize()));

			// Draws environment
			window.draw(environment);

			// Displays the current frame
			window.display();
		}
	}

	// Finishes 'main' with return
	return 0;
}