/**
@file environment.cpp
*/

// Imports
#include "environment.h"

// Void: Loads Env data from a file
void Environment::loadFromFile(const std::string ksFilePath)
{
	// Declares a file stream with the input file path
	std::fstream envFile(ksFilePath, std::ios_base::in);

	// If the file is not open
	if (!envFile.is_open())
	{
		// Prints status
		sf::err() << "[FILE] Unable to open: " << ksFilePath << std::endl;
		// Exits the method
		return;	
	}
	// Else the file is open
	else
	{
		// Prints status
		sf::err() << "[FILE] Opened: " << ksFilePath << std::endl;
	}

	// Declares a string to store file lines
	std::string sLine;

	// While new lines can be gotten
	while (std::getline(envFile, sLine)) // Gets next line
	{
		// Creates a string stream with the line
		std::istringstream iss(sLine);

		// Declares a string to store the first word of a line
		std::string sPrefix = "";
		// Assigns the first word of the line to sPrefix
		iss >> sPrefix;

		// If prefix is Size:
		if (sPrefix == "Size:")
		{
			readSizeLine(iss); // Reads the size line
		}

		// If prefix is Colour:
		else if (sPrefix == "Colour:")
		{
			readColourLine(iss); // Reads the colour line
		}

		// If prefix is Object:
		else if (sPrefix == "Object:")
		{
			readObjectLine(iss); // Reads the object line
		}

		// If prefix is Entity:
		else if (sPrefix == "Entity:")
		{
			readEntityLine(iss); // Reads the entity line
		}

		// Else the prefix isn't recognised
		else {}
	}

	// Closes Environment file
	envFile.close();
}

// Void: Called to update the environment
void Environment::update(const float kfElapsedTime)
{
	// For every Entity in the Environment
	for (std::shared_ptr<Entity> entity : m_pEntities)
	{
		// Updates the Entity
		entity->update(kfElapsedTime);
	}
}

// Void: Called to draw the environment
void Environment::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Clears previous frame and draws the Environment colour to the target
	target.clear(m_colour);

	// For every Entity in the Environment
	for (std::shared_ptr<Entity> entity : m_pEntities)
	{
		// Draws Entity to RenderTarget
		entity->draw(target, states);
	}

	// For every Object in the Environment
	for (std::shared_ptr<Object> object : m_pObjects)
	{
		// Draws Object to RenderTarget
		object->draw(target, states);
	}
}

// Void: Reads a size file line
void Environment::readSizeLine(std::istringstream& iss)
{
	// Declares two unsigned ints to store the line data
	unsigned int uiX = 0, uiY = 0;
	// Declares a string to store current word being processed
	std::string word = ""; 

	// While words can be gotten
	while (iss >> word)
	{
		// If word begins with 'x'
		if (word.front() == 'x')
		{
			// uiX is set by converting the word string to an int
			uiX = std::stoi(word.substr(1, word.size() - 1));
		}
		// If word begins with 'y'
		else if (word.front() == 'y')
		{
			// uiY is set by converting the word string to an int
			uiY = std::stoi(word.substr(1, word.size() - 1));
		}
	}

	sf::err() << "[FILE] Size read from file: x" << uiX << " y" << uiY << std::endl;

	// Sets member to derived size
	m_size = sf::Vector2u(uiX, uiY);
}

// Void: Reads a colour file line
void Environment::readColourLine(std::istringstream& iss)
{
	// Declares three unsigned ints to store the colour data
	unsigned int uiR = 0, uiG = 0, uiB = 0;
	// Declares a string to store current word being processed
	std::string word = "";

	// While words can be gotten
	while (iss >> word)
	{
		// If word begins with 'r'
		if (word.front() == 'r')
		{
			// uiR is set by converting the word string to an int
			uiR = std::stoi(word.substr(1, word.size() - 1));
		}
		// If word begins with 'g'
		else if (word.front() == 'g')
		{
			// uiG is set by converting the word string to an int
			uiG = std::stoi(word.substr(1, word.size() - 1));
		}
		// If word begins with 'b'
		else if (word.front() == 'b')
		{
			// uiB is set by converting the word string to an int
			uiB = std::stoi(word.substr(1, word.size() - 1));
		}
	}

	sf::err() << "[FILE] Colour read from file: r" << uiR << " g" << uiG << " b" << uiB << std::endl;

	// Sets member to derived colour with 100% opacity
	m_colour = sf::Color(uiR, uiG, uiB, 255);
}

// Void: Reads an Object file line
void Environment::readObjectLine(std::istringstream& iss)
{
	// Declares an enum to store which Object subclass is indicated
	enum subclass { NONE, BUSH, ROCK, TREE }; subclass objectType = NONE;
	// Declares three floats to store the position and size data
	float fX = 0, fY = 0, fR = 0;
	// Declares a string to store current word being processed
	std::string word = "";

	// While words can be gotten
	while (iss >> word)
	{
		// If word begins with 'x'
		if (word.front() == 'x')
		{
			// fX is set by converting the word string to a float
			fX = std::stof(word.substr(1, word.size() - 1));
		}
		// If word begins with 'y'
		else if (word.front() == 'y')
		{
			// fY is set by converting the word string to a float
			fY = std::stof(word.substr(1, word.size() - 1));
		}
		// If word begins with 'r'
		else if (word.front() == 'r')
		{
			// fR is set by converting the word string to a float
			fR = std::stof(word.substr(1, word.size() - 1));
		}
		// Else
		else
		{
			// If the word is Bush
			if (word == "Bush") objectType = BUSH; // Sets objectType to corresponding value

			// If the word is Rock
			else if (word == "Rock") objectType = ROCK; // Sets objectType to corresponding value

			// If the word is Tree
			else if (word == "Tree") objectType = TREE; // Sets objectType to corresponding value
		}
	}

	sf::err() << "[FILE] Object data read from file: type" << objectType << " x" << fX << " y" << fY << " r" << fR << std::endl;

	// With objectType creates a new Object subclass and stores a reference in the m_pObjects member
	if (objectType == BUSH) m_pObjects.push_back(std::shared_ptr<Object>(new Bush(sf::Vector2f(fX, fY))));
	else if (objectType == ROCK) m_pObjects.push_back(std::shared_ptr<Object>(new Tree(sf::Vector2f(fX, fY))));
	else if (objectType == TREE) m_pObjects.push_back(std::shared_ptr<Object>(new Rock(sf::Vector2f(fX, fY))));
}

// Void: Reads an Entity file line
void Environment::readEntityLine(std::istringstream& iss)
{
	// Declares an enum to store which Entity subclass is indicated
	enum subclass { NONE, COLONIST }; subclass entityType = NONE;
	// Declares two floats to store the position data
	float fX = 0, fY = 0;
	// Declares a string to store current word being processed
	std::string word = "";

	// While words can be gotten
	while (iss >> word)
	{
		// If word begins with 'x'
		if (word.front() == 'x')
		{
			// fX is set by converting the word string to a float
			fX = std::stof(word.substr(1, word.size() - 1));
		}
		// If word begins with 'y'
		else if (word.front() == 'y')
		{
			// fY is set by converting the word string to a float
			fY = std::stof(word.substr(1, word.size() - 1));
		}
		// Else
		else
		{
			// If the word is Colonist
			if (word == "Colonist") entityType = COLONIST; // Sets entityType to corresponding value
		}
	}

	sf::err() << "[FILE] Entity data read from file: type" << entityType << " x" << fX << " y" << fY << std::endl;

	// With entityType creates a new Entity subclass and stores a reference in the m_pEntities member
	if (entityType == COLONIST) m_pEntities.push_back(std::shared_ptr<Entity>(new Colonist(sf::Vector2f(fX, fY))));
}