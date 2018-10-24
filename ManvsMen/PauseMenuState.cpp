#include "PauseMenuState.h"
#include "MainMenuState.h"
#include "SelectWeaponState.h"



PauseMenuState::PauseMenuState(GameDataRef _data, sf::Image background) :_data(_data),backgroundImage(background)
{
	selected = 0;
}

PauseMenuState::~PauseMenuState()
{
}

void PauseMenuState::Init()
{
	texture.loadFromImage(backgroundImage);
	_background.setTexture(texture);
	_restartButton.setTexture(_data->assets.GetImage(PAUSE_MENU_RESTART_BUTTON));
	_resumeButton.setTexture(_data->assets.GetImage(PAUSE_MENU_RESUME_BUTTON));
	_mainMenuButton.setTexture(_data->assets.GetImage(PAUSE_MENU_EXIT_BUTTON));


	_background.setPosition(_data->window.getSize().x / 2 - texture.getSize().x / 2, _data->window.getSize().y / 2 - texture.getSize().y / 2);
	_resumeButton.setPosition(_data->window.getSize().x / 2 - _resumeButton.getTexture()->getSize().x / 2, _data->window.getSize().y * 5 / 11);
	_restartButton.setPosition(_data->window.getSize().x / 2 - _restartButton.getTexture()->getSize().x / 2, _data->window.getSize().y * 6 / 11);
	_mainMenuButton.setPosition(_data->window.getSize().x / 2 - _mainMenuButton.getTexture()->getSize().x / 2, _data->window.getSize().y * 7 / 11);


}

void PauseMenuState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			this->_data->window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed) {

			if (this->_data->inputs.IsSpriteClicked(_resumeButton, sf::Mouse::Left, _data->window)) {
				this->_data->machine.RemoveState();
			}
			if (this->_data->inputs.IsSpriteClicked(_restartButton, sf::Mouse::Left, _data->window)) {
				this->_data->machine.RemoveState();
				this->_data->machine.AddState(StateRef(new SelectWeaponState(_data)));
			}
			if (this->_data->inputs.IsSpriteClicked(_mainMenuButton, sf::Mouse::Left, _data->window)) {
				this->_data->machine.RemoveState();
				this->_data->machine.AddState(StateRef(new MainMenuState(_data)));
			}
		}
		if (this->_data->inputs.IsMouseOver(_resumeButton, _data->window)) {
			selected = 0;
		} else if (this->_data->inputs.IsMouseOver(_restartButton, _data->window)) {
			selected = 1;
		} else if (this->_data->inputs.IsMouseOver(_mainMenuButton, _data->window)) {
			selected = 2;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			selected++;
			if (selected == 3) {
				selected = 0;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			selected--;
			if (selected == -1) {
				selected = 2;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			switch (selected)
			{
			case 0:
				this->_data->machine.RemoveState();
				break;
			case 1:
				this->_data->machine.RemoveState();
				this->_data->machine.AddState(StateRef(new SelectWeaponState(_data)));
				break;
			case 2:
				this->_data->machine.RemoveState();
				this->_data->machine.AddState(StateRef(new MainMenuState(_data)));
				break;
			default:
				break;
			}
		}
	}

}

void PauseMenuState::Update(float dt)
{
	if (selected == 0) {
		_resumeButton.setColor(sf::Color::Red);
		_restartButton.setColor(sf::Color::White);
		_mainMenuButton.setColor(sf::Color::White);
	} else if (selected == 1) {
		_resumeButton.setColor(sf::Color::White);
		_restartButton.setColor(sf::Color::Red);
		_mainMenuButton.setColor(sf::Color::White);
	} else if (selected == 2) {
		_resumeButton.setColor(sf::Color::White);
		_restartButton.setColor(sf::Color::White);
		_mainMenuButton.setColor(sf::Color::Red);
	}
	
}

void PauseMenuState::Draw(float dt)
{
	_data->window.clear();
	_data->window.draw(_background);
	_data->window.draw(_restartButton);
	_data->window.draw(_resumeButton);
	_data->window.draw(_mainMenuButton);
	_data->window.display();

}