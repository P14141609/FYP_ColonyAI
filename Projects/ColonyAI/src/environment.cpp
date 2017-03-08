/**
@file environment.cpp
*/

// Imports
#include "environment.h"

// Bool: Loads Env data from a file - Returns whether the Environment was loaded successfully
bool Environment::loadFromFile(const std::string ksFilePath)
{
	// Declares a file stream with the input file path
	std::fstream envFile(ksFilePath, std::ios_base::in);

	// If the file is not open
	if (!envFile.is_open())
	{
		// Prints status
		sf::err() << "[FILE] Unable to open: " << ksFilePath << std::endl;
		// Exits the method
		return false;	
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

		// If prefix is Environment:
		if (sPrefix == "Environment:")
		{
			readEnvLine(iss); // Reads the size line
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

	return true;
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
	// Rectangle shape for drawing
	sf::RectangleShape rectShape;
	rectShape.setSize(sf::Vector2f(m_size.x, m_size.y)); // Size of Environment
	rectShape.setFillColor(m_colour);
	// Draws shape to display
	target.draw(rectShape);

	// For every Object in the Environment
	for (std::shared_ptr<Object> object : m_pObjects)
	{
		// If Object is not a Tree
		if (object->getType() != TREE)
		{
			// Draws Object to RenderTarget
			object->draw(target, states);
		}
	}

	// For every Entity in the Environment
	for (std::shared_ptr<Entity> entity : m_pEntities)
	{
		// Draws Entity to RenderTarget
		entity->draw(target, states);
	}

	// For every Object in the Environment
	for (std::shared_ptr<Object> object : m_pObjects)
	{
		// If Object is a Tree
		if (object->getType() == TREE)
		{
			// Draws Object to RenderTarget
			object->draw(target, states);
		}
	}
}

// Void: Reads a size file line
void Environment::readEnvLine(std::istringstream& iss)
{
	// Declares two unsigned ints to store the size data
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

	sf::err() << "[FILE] Environment read: x(" << uiX << ") y(" << uiY << ")" << std::endl;

	// Sets member to derived size
	m_size = sf::Vector2u(uiX, uiY);
}

// Void: Reads an Object file line
void Environment::readObjectLine(std::istringstream& iss)
{
	// Declares an ObjectType object to store the type data
	ObjectType type;
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
			if (word == "Bush") type = BUSH; // Sets objectType to corresponding value

			// If the word is Rock
			else if (word == "Rock") type = ROCK; // Sets objectType to corresponding value

			// If the word is Tree
			else if (word == "Tree") type = TREE; // Sets objectType to corresponding value

			// If the word is Water
			else if (word == "Water") type = WATER; // Sets objectType to corresponding value
		}
	}

	sf::err() << "[FILE] Object read: type(" << Object::typeToStr(type) << ") x(" << fX << ") y(" << fY << ") r(" << fR << ")" << std::endl;

	// With objectType creates a new Object subclass and stores a reference in the m_pObjects member
	if (type == BUSH) m_pObjects.push_back(std::shared_ptr<Object>(new Bush(sf::Vector2f(fX, fY), fR)));
	else if (type == ROCK) m_pObjects.push_back(std::shared_ptr<Object>(new Rock(sf::Vector2f(fX, fY), fR)));
	else if (type == TREE) m_pObjects.push_back(std::shared_ptr<Object>(new Tree(sf::Vector2f(fX, fY), fR)));
	else if (type == WATER) m_pObjects.push_back(std::shared_ptr<Object>(new Water(sf::Vector2f(fX, fY), fR)));
}

// Void: Reads an Entity file line
void Environment::readEntityLine(std::istringstream& iss)
{
	// Declares an EntityType object to store the type data
	EntityType type;
	// Declares two floats to store the position data
	float fX = 0, fY = 0, fH = 0;
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
		// If word begins with 'h'
		else if (word.front() == 'h')
		{
			// fH is set by converting the word string to a float
			fH = std::stof(word.substr(1, word.size() - 1));
		}
		// Else
		else
		{
			// If the word is Colonist
			if (word == "Colonist") type = COLONIST; // Sets entityType to corresponding value
		}
	}

	sf::err() << "[FILE] Entity read: type(" << Entity::typeToStr(type) << ") x(" << fX << ") y(" << fY << ") h(" << fH << ")" << std::endl;

	// With entityType creates a new Entity subclass and stores a reference in the m_pEntities member
	if (type == COLONIST) m_pEntities.push_back( std::shared_ptr<Entity>( new Colonist(this, sf::Vector2f(fX, fY), fH) ) );
}