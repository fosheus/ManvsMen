#include "Entity.h"
#include "Utils.h"




void Entity::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Entity::Entity(GameDataRef data,sf::Vector2f position) : _data(data)
{
	sprite.setPosition(position);
	pickTimeout = 0;
}




sf::RectangleShape Entity::getMinBounds()
{
	sf::RectangleShape rect;
	rect.setPosition(sprite.getPosition());
	rect.setSize(sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height));
	return rect;
}

void Entity::update(float multiplier)
{
	if (pickTimeout > 0) {
		pickTimeout -= multiplier;
		if (pickTimeout < 0) {
			pickTimeout = 0;
		}
	}
}



Entity::~Entity()
{
}
