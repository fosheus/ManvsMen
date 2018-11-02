#include "Game.h"
#include "SplashState.h"
#include "DEFINITIONS.h"

Game::Game(std::string settingsFileName)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	//load settings
	loadSettings(settingsFileName);
	loadKeyBinding(settingsFileName);
	//load keyBinding


	this->_data->window.create(sf::VideoMode(_data->config.width, _data->config.height), GAME_NAME, sf::Style::Close | sf::Style::Titlebar| (sf::Style::Fullscreen*_data->config.fullscreen),settings);
	this->_data->window.setVerticalSyncEnabled(true);

	this->_data->machine.AddState(StateRef(new SplashState(this->_data)));

	this->Run();
}

Game::~Game()
{
}

bool Game::loadSettings(std::string settingsFileName)
{

	if (settings.loadFromFile(settingsFileName)) {

		settings.get("Width", _data->config.width);
		settings.get("Height", _data->config.height);
		settings.get("Fullscreen", _data->config.fullscreen);
		_data->config.fullscreen = _data->config.fullscreen > 0 ? 1 : 0;
	}
	else {
		std::cout << "Error loading settings file!" << std::endl;
		_data->config.width = SCREEN_WIDTH;
		_data->config.height = SCREEN_HEIGHT;
		_data->config.fullscreen = FULLSCREEN;
	}
	return true;
}

bool Game::loadKeyBinding(std::string settingsFileName)
{
	std::string strKey;
	KeyBindingManager::ErrorCode error;
	if (settings.loadFromFile(settingsFileName)) {
		settings.get("Up", strKey);
		_data->keys.changeInputForAction("Up", strKey);
		settings.get("Left", strKey);
		_data->keys.changeInputForAction("Left", strKey);
		settings.get("Right", strKey);
		_data->keys.changeInputForAction("Right", strKey);
		settings.get("Down", strKey);
		_data->keys.changeInputForAction("Down", strKey);
		settings.get("PickUp", strKey);
		_data->keys.changeInputForAction("PickUp", strKey);
		settings.get("Shoot", strKey);
		_data->keys.changeInputForAction("Shoot", strKey);
		settings.get("Aim", strKey);
		_data->keys.changeInputForAction("Aim", strKey);
		settings.get("Reload", strKey);
		_data->keys.changeInputForAction("Reload", strKey);
		settings.get("Run", strKey);
		_data->keys.changeInputForAction("Run", strKey);
		settings.get("Pause", strKey);
		_data->keys.changeInputForAction("Pause", strKey);
		settings.get("Back", strKey);
		_data->keys.changeInputForAction("Back", strKey);
		
	}
	return true;
}

void Game::Run()
{
	float newTime, frameTime, interpolation;

	float currentTime = this->_clock.getElapsedTime().asSeconds();

	float accumulator = 0.0f;

	while (this->_data->window.isOpen()) {

		this->_data->machine.ProcessStateChanges();

		newTime = this->_clock.getElapsedTime().asSeconds();

		frameTime = newTime - currentTime;

		if (frameTime > 0.25f) {
			frameTime = 0.25f;
		}
		currentTime = newTime;
		accumulator += frameTime;

		while (accumulator >= dt) {
			this->_data->machine.GetActiveState()->HandleInput();
			this->_data->machine.GetActiveState()->Update(dt);

			accumulator -= dt;
		}

		interpolation = accumulator / dt;
		this->_data->machine.GetActiveState()->Draw(interpolation);
	}


}
