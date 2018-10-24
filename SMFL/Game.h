#pragma once

#include <memory>
#include <string>
#include "SFML\Graphics.hpp"
#include "StateMachine.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "MapManager.h"
#include "SettingsParser.h"


struct GameData {
	StateMachine machine;
	sf::RenderWindow window;
	AssetManager assets;
	InputManager inputs;
	MapManager map;
	SettingsParser settings;

};

typedef std::shared_ptr<GameData> GameDataRef;
class Game
{
public:
	Game(std::string settingsFileName);
	~Game();

private :
	const float dt = 1.0f / 60.0f;
	sf::Clock _clock;
	GameDataRef _data = std::make_shared<GameData>();

	void Run();

};

