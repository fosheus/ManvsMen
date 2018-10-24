#include "KeyBindingManager.h"



KeyBindingManager::KeyBindingManager()
{
	initBaseInputs();
}

int KeyBindingManager::changeInputForAction(std::string action, Input input)
{
	KeyConversionManager::Input convertedInput;
	if (input.type == MOUSE) {
		convertedInput.input.button = input.mouse;
		convertedInput.type = KeyConversionManager::MOUSE;
	}
	if (input.type == KEYBOARD) {
		convertedInput.input.key = input.key;
		convertedInput.type = KeyConversionManager::KEY;
	}
	std::string strInput = keyConversion.keyToString(convertedInput);
	if (strInput.size() < 0) {
		return UNKOWN_INPUT;
	}
	for (int i = 0; i < authorizedInputs.size(); i++) {
		if (action == authorizedInputs[i]) {
			inputs[action] = input;
			return NO_ERROR;
		}
	}
	return UNKNOWN_ACTION;
}

int KeyBindingManager::changeInputForAction(std::string action, std::string input)
{
	return 0;
}

bool KeyBindingManager::testEvent(std::string action, sf::Event event)
{
	auto it = inputs.find(action);
	if (it != inputs.end()) {
		if (event.type == it->second.eventType &&
			it->second.type == KEYBOARD &&
			event.key.code == it->second.key) 
		{
			return true;
		}
		if (event.type == it->second.eventType &&
			it->second.type == MOUSE &&
			event.mouseButton.button == it->second.mouse) 
		{
			return true;
		}
		return false;
	}
	return false;
}

bool KeyBindingManager::testInputDown(std::string action)
{
	auto it = inputs.find(action);
	if (it != inputs.end()) {
		if (it->second.type == KEYBOARD &&
			sf::Keyboard::isKeyPressed(it->second.key)) 
		{
			return true;
		}
		if (it->second.type == MOUSE &&
			sf::Mouse::isButtonPressed(it->second.mouse)) 
		{
			return true;
		}
		return false;
	}
	return false;
}



void KeyBindingManager::initBaseInputs()
{
	Input input;

	authorizedInputs.push_back("Up");
	authorizedInputs.push_back("Left");
	authorizedInputs.push_back("Right");
	authorizedInputs.push_back("Down");
	authorizedInputs.push_back("PickUp"); 
	authorizedInputs.push_back("Reload");
	authorizedInputs.push_back("Run");
	authorizedInputs.push_back("Pause");
	authorizedInputs.push_back("Back");
	authorizedInputs.push_back("Shoot");
	authorizedInputs.push_back("Aim");
	

	input.key = sf::Keyboard::Z;
	input.type = KEYBOARD;
	input.eventType = sf::Event::KeyPressed;
	inputs["Up"] = input;

	input.key = sf::Keyboard::Q;
	input.type = KEYBOARD;
	input.eventType = sf::Event::KeyPressed;
	inputs["Left"] = input;

	input.key = sf::Keyboard::D;
	input.type = KEYBOARD;
	input.eventType = sf::Event::KeyPressed;
	inputs["Right"] = input;

	input.key = sf::Keyboard::S;
	input.type = KEYBOARD;
	input.eventType = sf::Event::KeyPressed;
	inputs["Down"] = input;

	input.key = sf::Keyboard::E;
	input.type = KEYBOARD;
	input.eventType = sf::Event::KeyPressed;
	inputs["PickUp"] = input;

	input.key = sf::Keyboard::R;
	input.type = KEYBOARD;
	input.eventType = sf::Event::KeyPressed;
	inputs["Reload"] = input;

	input.key = sf::Keyboard::LShift;
	input.type = KEYBOARD;
	input.eventType = sf::Event::KeyPressed;
	inputs["Run"] = input;

	input.key = sf::Keyboard::Escape;
	input.type = KEYBOARD;
	input.eventType = sf::Event::KeyPressed;
	inputs["Pause"] = input;

	input.key = sf::Keyboard::Backspace;
	input.type = KEYBOARD;
	input.eventType = sf::Event::KeyPressed;
	inputs["Back"] = input;

	input.mouse = sf::Mouse::Button::Left;
	input.type = MOUSE;
	input.eventType = sf::Event::MouseButtonPressed;
	inputs["Shoot"] = input;

	input.mouse = sf::Mouse::Button::Right;
	input.type = MOUSE;
	input.eventType = sf::Event::MouseButtonPressed;
	inputs["Aim"] = input;


}


KeyBindingManager::~KeyBindingManager()
{
}
