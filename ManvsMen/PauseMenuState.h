#pragma once
#include "State.h"
#include <SFML\Graphics.hpp>
#include "Game.h"
#include "DEFINITIONS.h"
class PauseMenuState :
	public State
{
private : 
	int selected;
public:
	PauseMenuState(GameDataRef _data,sf::Image background);
	~PauseMenuState();

	// Hérité via State
	virtual void Init() override;
	virtual void HandleInput() override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

private:
	sf::Texture texture;
	GameDataRef _data;
	sf::Image backgroundImage;

	sf::Sprite _background;
	sf::Sprite _resumeButton;
	sf::Sprite _restartButton;
	sf::Sprite _mainMenuButton;


};

