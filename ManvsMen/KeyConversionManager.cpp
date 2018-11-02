#include "KeyConversionManager.h"


KeyConversionManager::KeyConversionManager()
{
	init();
}


std::string KeyConversionManager::keyToString(Input input)
{
	std::map<std::string, sf::Keyboard::Key>::iterator keyIt;
	std::map<std::string, sf::Mouse::Button>::iterator buttonIt;

	if (input.type == MOUSE) {
		for (buttonIt = strButton.begin(); buttonIt != strButton.end(); buttonIt++) {
			if (buttonIt->second == input.input.button) {
				return buttonIt->first;
			}
		}
	}
	if (input.type == KEY) {
		for (keyIt = strKey.begin(); keyIt != strKey.end(); keyIt++) {
			if (keyIt->second == input.input.key) {
				return keyIt->first;
			}
		}
	}
	return std::string();
}

KeyConversionManager::Input KeyConversionManager::stringToInput(std::string key)
{
	Input input;
	std::map<std::string, sf::Mouse::Button>::iterator buttonIt;
	buttonIt = strButton.find(key);
	if (buttonIt != strButton.end()) {
		
		input.type = MOUSE;
		input.input.button = buttonIt->second;
		return input;
	}
	std::map<std::string, sf::Keyboard::Key>::iterator keyIt;

	keyIt = strKey.find(key);
	if (keyIt != strKey.end()) {

		input.type = KEY;
		input.input.key = keyIt->second;
		return input;
	}
	input.type = UNKOWN;
	return input;
	
}

bool KeyConversionManager::isMouseInput(std::string button)
{
	auto it = strButton.find(button);
	if (it != strButton.end()) {
		return true;
	}
	return false;
}

bool KeyConversionManager::isKeyboardInput(std::string key)
{
	auto it = strKey.find(key);
	if (it != strKey.end()) {
		return true;
	}
	return false;
}

void KeyConversionManager::init()
{
	//init mouse
	strButton["LMOUSE"] = sf::Mouse::Button::Left;
	strButton["RMOUSE"] = sf::Mouse::Button::Right;
	strButton["MMOUSE"] = sf::Mouse::Button::Middle;

	strKey["A"] = sf::Keyboard::Key::A;
	strKey["B"] = sf::Keyboard::Key::B;
	strKey["C"] = sf::Keyboard::Key::C;
	strKey["D"] = sf::Keyboard::Key::D;
	strKey["E"] = sf::Keyboard::Key::E;
	strKey["F"] = sf::Keyboard::Key::F;
	strKey["G"] = sf::Keyboard::Key::G;
	strKey["H"] = sf::Keyboard::Key::H;
	strKey["I"] = sf::Keyboard::Key::I;
	strKey["J"] = sf::Keyboard::Key::J;
	strKey["K"] = sf::Keyboard::Key::K;
	strKey["L"] = sf::Keyboard::Key::L;
	strKey["M"] = sf::Keyboard::Key::M;
	strKey["N"] = sf::Keyboard::Key::N;
	strKey["O"] = sf::Keyboard::Key::O;
	strKey["P"] = sf::Keyboard::Key::P;
	strKey["Q"] = sf::Keyboard::Key::Q;
	strKey["R"] = sf::Keyboard::Key::R;
	strKey["S"] = sf::Keyboard::Key::S;
	strKey["T"] = sf::Keyboard::Key::T;
	strKey["U"] = sf::Keyboard::Key::U;
	strKey["V"] = sf::Keyboard::Key::V;
	strKey["W"] = sf::Keyboard::Key::W;
	strKey["X"] = sf::Keyboard::Key::X;
	strKey["Y"] = sf::Keyboard::Key::Y;
	strKey["Z"] = sf::Keyboard::Key::Z;

	strKey["LCTRL"] = sf::Keyboard::Key::LControl;
	strKey["LSHIFT"] = sf::Keyboard::Key::LShift;
	strKey["SPACE"] = sf::Keyboard::Key::Space;
	strKey["LALT"] = sf::Keyboard::Key::LAlt;
	strKey["RCTRL"] = sf::Keyboard::Key::RControl;
	strKey["RSHIFT"] = sf::Keyboard::Key::RShift;
	strKey["ENTER"] = sf::Keyboard::Key::Enter;
	strKey["BACKSPACE"] = sf::Keyboard::Key::Backspace;
	strKey["TAB"] = sf::Keyboard::Key::Tab;
	strKey["ESCAPE"] = sf::Keyboard::Key::Escape;

	strKey["0"] = sf::Keyboard::Key::Num0;
	strKey["1"] = sf::Keyboard::Key::Num1;
	strKey["2"] = sf::Keyboard::Key::Num2;
	strKey["3"] = sf::Keyboard::Key::Num3;
	strKey["4"] = sf::Keyboard::Key::Num4;
	strKey["5"] = sf::Keyboard::Key::Num5;
	strKey["6"] = sf::Keyboard::Key::Num6;
	strKey["7"] = sf::Keyboard::Key::Num7;
	strKey["8"] = sf::Keyboard::Key::Num8;
	strKey["9"] = sf::Keyboard::Key::Num9;

	strKey["NUMPAD0"] = sf::Keyboard::Key::Numpad0;
	strKey["NUMPAD1"] = sf::Keyboard::Key::Numpad1;
	strKey["NUMPAD2"] = sf::Keyboard::Key::Numpad2;
	strKey["NUMPAD3"] = sf::Keyboard::Key::Numpad3;
	strKey["NUMPAD4"] = sf::Keyboard::Key::Numpad4;
	strKey["NUMPAD5"] = sf::Keyboard::Key::Numpad5;
	strKey["NUMPAD6"] = sf::Keyboard::Key::Numpad6;
	strKey["NUMPAD7"] = sf::Keyboard::Key::Numpad7;
	strKey["NUMPAD8"] = sf::Keyboard::Key::Numpad8;
	strKey["NUMPAD9"] = sf::Keyboard::Key::Numpad9;

	


}



