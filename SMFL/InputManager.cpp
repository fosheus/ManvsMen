#include "InputManager.h"

bool InputManager::IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow & window)
{
	if (sf::Mouse::isButtonPressed(button)) {
		sf::IntRect rect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);
		if (rect.contains(sf::Mouse::getPosition(window))) {
			return true;
		}
	}
	return false;
}

bool InputManager::IsMouseOver(sf::Sprite object, sf::RenderWindow & window)
{
	sf::Vector2f mouse = sf::Vector2f(GetMousePosition(window).x, GetMousePosition(window).y);
	if (object.getGlobalBounds().contains(mouse)) {
		return true;
	}
	return false;
}

sf::Vector2i InputManager::GetMousePosition(sf::RenderWindow & window)
{
	return sf::Mouse::getPosition(window);
}
