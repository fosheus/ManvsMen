#pragma once
#include "Game.h"
#include "Entity.h"
#include "Character.h"
class KevlarEntity :public Entity
{
private :
	int protection;
public:

	int getProtection() { return protection; }
	Entity* pickedBy(Character* character);
	Entity::EntityType getEntityType();
	KevlarEntity(GameDataRef data, sf::Vector2f position,int protection);
	~KevlarEntity();
};

