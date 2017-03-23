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
#include "editor.h"

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

//!< sf::View: Modifies a View to fit a display with letterboxing - Returns a View correctly letterboxed for the display
sf::View letterboxView(sf::View view, const sf::Vector2u kWindowSize)
{
	// Defines view aspect ratio
	float fViewRatio = (float)view.getSize().x / (float)view.getSize().y;
	// Defines window aspect ratio
	float fWindowRatio = (float)kWindowSize.x / (float)kWindowSize.y;

	// Defines vector for new view size ratio
	sf::Vector2f newSize(1.0f, 1.0f);
	// Defines vector for new view position ratio
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

		// If input is invalid
		if (sInput != "load" && sInput != "create") std::cout << "Invalid input" << std::endl;
	} 
	while (sInput != "load" && sInput != "create"); // Checks whether input isn't valid

	// Proceed with program
	// If 'Load' selected
	if (sInput == "load")
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

	// If 'Create' selected
	else if (sInput == "create")
	{
		// EDITOR NAME
		// Declares string for Editor directory
		std::string sEditDir;

		// Gets user input for the Editor directory
		std::cout << "Choose Environment name: 'environments/YOURINPUT.cfg': ";
		std::cin >> sInput;

		// Defines Editor directory
		sEditDir = "environments/" + sInput + ".cfg";

		// EDITOR SIZE
		// Declares string for Editor directory
		sf::Vector2u editorSize;
		// Unsigned int for storing user input
		unsigned int uiInput;

		// Gets user input for the Editor X size
		std::cout << "Environment size:" << std::endl;
		std::cout << "    x: ";
		std::cin >> uiInput;
		// Defines Editor X size
		editorSize.x = uiInput;

		// Gets user input for the Editor Y size
		std::cout << "    y: ";
		std::cin >> uiInput;
		// Defines Editor Y size
		editorSize.y = uiInput;

		// Instantiates new Editor
		Editor editor = Editor(sEditDir, editorSize);

		// Defines WindowsProperties with given title and size
		WindowProperties winProps("Level Editor - Final Year Project - P14141609", sf::Vector2u(1280, 720));

		// Instantiates window
		sf::RenderWindow window(sf::VideoMode(winProps.m_size.x, winProps.m_size.y), winProps.m_sTitle.c_str(), sf::Style::Default);

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

					// If Esc is pressed
					if (event.key.code == sf::Keyboard::Escape)
					{
						// Closes window
						window.close();
					}

					// If Up is pressed
					if (event.key.code == sf::Keyboard::Up)
					{
						editor.setHandRadius(editor.getHandRadius() + 0.25f);
					}

					// If Down is pressed
					if (event.key.code == sf::Keyboard::Down)
					{
						if (editor.getHandRadius()-0.25 > 0)
						{
							editor.setHandRadius(editor.getHandRadius() - 0.25f);
						}
					}

					// If Left is pressed
					if (event.key.code == sf::Keyboard::Left)
					{
						editor.setHandHeading(editor.getHandHeading() - 0.1f);
					}

					// If Right is pressed
					if (event.key.code == sf::Keyboard::Right)
					{
						editor.setHandHeading(editor.getHandHeading() + 0.1f);
					}

					// If Space is pressed
					if (event.key.code == sf::Keyboard::Space)
					{
						editor.save();
					}
				}

				// If MouseButtonPressed event is called
				if (event.type == sf::Event::MouseButtonPressed)
				{
					// If LMouseButton is pressed
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						editor.placeSelected();
					}

					// Else If RMouseButton is pressed
					if (event.mouseButton.button == sf::Mouse::Right)
					{
						editor.undoPlace();
					}
				}

				// If MouseMoved event is called
				if (event.type == sf::Event::MouseMoved)
				{
					// Defines MousePos as a Vector2f
					sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
					// Defines MousePosition as a ratio of window size 
					sf::Vector2f mousePosRatio(mousePos.x / (float)window.getSize().x, event.mouseMove.y / (float)window.getSize().y);

					// Defines the View of the Editor
					sf::View editorView(letterboxView(sf::View(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(editor.getSize()))), window.getSize()));

					// Defines the size of the letterboxing
					sf::Vector2f letterboxSize(
						editorView.getViewport().left * window.getSize().x,
						editorView.getViewport().top * window.getSize().y
					);

					// Defines the center of the view including letterboxing
					sf::Vector2f trueCenter(editorView.getCenter() + letterboxSize*2.0f);

					// Defines the position using the size of the view times mousePos ratio
					sf::Vector2f position(
						((editorView.getSize().x + letterboxSize.x*2) * mousePosRatio.x) - letterboxSize.x,
						((editorView.getSize().y + letterboxSize.y*2) * mousePosRatio.y) - letterboxSize.y
					);

					// Need to apply offset to account for letterboxing
					// Defines view aspect ratio
					float fViewAspectRatio = (float)editorView.getSize().x / (float)editorView.getSize().y;
					// Defines window aspect ratio
					float fWindowAspectRatio = (float)window.getSize().x / (float)window.getSize().y;

					// If aspect ratios don't match
					if (fViewAspectRatio != fWindowAspectRatio)
					{
						// Defines distance from center
						sf::Vector2f distance(
							mousePos.x - trueCenter.x,
							mousePos.y - trueCenter.y
						);

						// Defines ratio of position from center to window edge
						sf::Vector2f distRatio(
							abs(distance.x) / trueCenter.x,
							abs(distance.y) / trueCenter.y
						);

						// Defines offset
						sf::Vector2f offset(0, 0);
						//	(trueCenter.x * distRatio.x),
						//	(trueCenter.y * distRatio.y)
						//);

						// Letterboxing on width: offset X axis
						if (fWindowAspectRatio > fViewAspectRatio)
						{
							position.x -= offset.x;
							//// If distance is positive
							//if (distance.x > 0) position.x += offset.x;
							//// If distance is negative
							//else position.x -= offset.x;
						}
						// Letterboxing on height: offset Y axis
						else
						{
							position.y -= offset.y;
							//// If distance is positive
							//if (distance.y > 0) position.y += offset.y;
							//// If distance is negative
							//else position.y -= offset.y;
						}
					}
					
					// Sets the Editor's hand position
					editor.setHandPos(position);
				}

				// If MouseWheelScrolled event is called
				if (event.type == sf::Event::MouseWheelScrolled)
				{
					// Scrolled up
					if (event.mouseWheelScroll.delta > 0) editor.cycleSelected(1);
					// Scrolled down
					else editor.cycleSelected(-1);
				}
			}

			// Clears window making it entirely black
			window.clear(sf::Color(0, 0, 0, 255));

			// Defines the View of the Editor
			sf::View editorView(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(editor.getSize())));

			// Sets view to size of editor
			window.setView(letterboxView(editorView, window.getSize()));

			// Draws editor
			window.draw(editor);

			// Displays the current frame
			window.display();
		}
	}

	// Finishes 'main' with return
	return 0;
}