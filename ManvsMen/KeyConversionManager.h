#pragma once

#include <map>
#include <string>
#include <SFML\Graphics.hpp>




class KeyConversionManager
{
public :
	union KeyOrMouse {
		sf::Keyboard::Key key;
		sf::Mouse::Button button;
	};

	enum InputType {
		KEY,
		MOUSE,
		UNKOWN
	};
	struct Input {
		KeyOrMouse input;
		InputType type;
	};
public:
	KeyConversionManager();
	std::string keyToString(Input input);
	Input stringToInput(std::string key);
	bool isMouseInput(std::string button);
	bool isKeyboardInput(std::string key);

private:

	void init();

private:

	std::map<std::string, sf::Keyboard::Key> strKey;
	std::map<std::string, sf::Mouse::Button> strButton;

	
	
};

