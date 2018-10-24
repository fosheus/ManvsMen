#pragma once
#include <SFML\Graphics.hpp>
#include "Game.h"

#define NB_ITEMS 3
class GameMenu
{

public :
	enum Items {
		Resume=0,
		Restart=1,
		Quit=2
	};
private :
	sf::Sprite background;
	sf::Font pixelFont;
	sf::Text resumeText;
	sf::Text restartText;
	sf::Text quitText;
	int selectedIndex;
	std::vector<sf::Text> texts;

	GameDataRef _data;

	
public:
	GameMenu();
	void SelectItemUp();
	void SelectItemDown();
	Items getSelectedItem();
	void Draw(sf::RenderWindow& window);
	~GameMenu();
};

