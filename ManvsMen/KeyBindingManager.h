#pragma once
#include "KeyConversionManager.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>



class KeyBindingManager
{
public :
	enum ErrorCode {
		NO_ERROR,
		UNKNOWN_ACTION,
		UNKOWN_INPUT
	};
	enum InputType {
		KEYBOARD,
		MOUSE
	};
	struct Input {
		InputType type;
		sf::Event::EventType eventType;
		sf::Keyboard::Key key;
		sf::Mouse::Button mouse;
	};
public:
	KeyBindingManager();
	int changeInputForAction(std::string action, Input input);
	int changeInputForAction(std::string action, std::string input);
	bool testEvent(std::string action, sf::Event event);
	bool testInputDown(std::string action);


	~KeyBindingManager();
private:
	void initBaseInputs();
	
private:
	std::map<std::string, Input> inputs;
	std::vector<std::string> authorizedInputs;
	KeyConversionManager keyConversion;
	
};

