#pragma once

#include <map>
#include <string>
#include <SFML\Graphics.hpp>
class KeyBindingManager
{
private :
	union input_u {
		sf::Keyboard key;
		sf::Mouse mouse;
	};
public :
	struct input_t {
		input_u input;
		int type;
	};
public:
	enum Action {
		Up,
		Down,
		Left,
		Right,
		Pick,
		Reload,
		Run,
		Shoot,
		Aim
	};
	
	std::map<Action, input_t> actionInput;
	
public:
	KeyBindingManager(std::string filename);
	KeyBindingManager();
	~KeyBindingManager();

private:
	void loadDefault();
	void loadFromFile(std::string filename);
};

