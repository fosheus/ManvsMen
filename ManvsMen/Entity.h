#pragma once
#include <string>
#include "SFML\Graphics.hpp"
#include "Game.h"
#include "Character.h"
class Entity : public sf::Drawable
{
public :
	enum class EntityType
	{
		Weapon,
		Medikit,
		Kevlar,
		Backpack
	};
private :
	int pickTimeout;
protected:
	sf::Sprite sprite;
	GameDataRef _data;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

public:
	Entity(GameDataRef data,sf::Vector2f position);
	int getPickTimeout() { return pickTimeout; }
	sf::Sprite getSprite() { return sprite; }
	sf::RectangleShape getMinBounds();
	void update(float multiplier);
	const sf::Vector2f& getPosition() { return sprite.getPosition(); }
	virtual Entity* pickedBy(Character* character) = 0;
	virtual Entity::EntityType getEntityType() = 0;
	virtual ~Entity()=0;
};


