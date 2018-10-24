#pragma once
#include <SFML\Graphics.hpp>
#include "Game.h"
#include "State.h"
#include "DEFINITIONS.h"
#include "MainMenuState.h"

class GameoverState : public State
{
public:
	GameoverState(GameDataRef data,sf::Image background);
	~GameoverState();

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef _data;

	sf::Clock _clock;

	sf::Texture _backgroundTexture;
	sf::Sprite _background;
	sf::Sprite _gameover;
};

