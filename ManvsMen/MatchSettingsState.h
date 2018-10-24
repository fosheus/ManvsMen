#pragma once
#include "State.h"
#include "Game.h"
class MatchSettingsState :
	public State
{
public:


	MatchSettingsState(GameDataRef data);

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

	~MatchSettingsState();
private : 
	GameDataRef _data;

	sf::Clock _clock;

	sf::Sprite _background;

	sf::Sprite _playButton;
	sf::Sprite _optionsButton;
	sf::Sprite _onlineButton;
	sf::Sprite _exitButton;

};

