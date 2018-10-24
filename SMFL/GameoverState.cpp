#include "GameoverState.h"



GameoverState::GameoverState(GameDataRef data, sf::Image background) : _data(data)
{
	_backgroundTexture.loadFromImage(background);
}

GameoverState::~GameoverState()
{
}

void GameoverState::Init()
{
	this->_background.setTexture(_backgroundTexture);
	this->_background.setPosition(this->_data->window.getSize().x / 2 - this->_background.getGlobalBounds().width / 2, this->_data->window.getSize().y / 2 - this->_background.getGlobalBounds().height / 2);
	this->_gameover.setTexture(this->_data->assets.GetImage(GAMEOVER_TEXTURE));
	this->_gameover.setPosition(this->_data->window.getSize().x / 2 - this->_gameover.getGlobalBounds().width / 2, this->_data->window.getSize().y / 2 - this->_gameover.getGlobalBounds().height / 2);

}

void GameoverState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			this->_data->window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)));
		}
	}
}

void GameoverState::Update(float dt)
{
}

void GameoverState::Draw(float dt)
{
	this->_data->window.clear();

	this->_data->window.draw(_background);
	this->_data->window.draw(_gameover);

	this->_data->window.display();

}
