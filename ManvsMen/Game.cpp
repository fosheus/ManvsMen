#include "Game.h"
#include "SplashState.h"
#include "DEFINITIONS.h"

Game::Game(std::string settingsFileName)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	

	int width, height,fullscreen;

	if (this->_data->settings.loadFromFile(settingsFileName)) {
		
		this->_data->settings.get("width", width);
		this->_data->settings.get("height", height);
		this->_data->settings.get("fullscreen", fullscreen);
	}
	else {
		std::cout << "Error loading settings file!" << std::endl;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		fullscreen = FULLSCREEN;
	}


	this->_data->window.create(sf::VideoMode(width, height), GAME_NAME, sf::Style::Close | sf::Style::Titlebar| (sf::Style::Fullscreen*fullscreen),settings);
	_data->window.setVerticalSyncEnabled(true);

	this->_data->machine.AddState(StateRef(new SplashState(this->_data)));

	this->Run();
}

Game::~Game()
{
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
