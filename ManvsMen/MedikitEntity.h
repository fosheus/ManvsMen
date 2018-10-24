#pragma once
#include "Entity.h"
#include "SFML\Graphics.hpp"
#include "Character.h"

#define MEDIKIT_HEALTH 35

class MedikitEntity : public Entity
{

private:
	int health;
public:
	int getHealth() { return health; }
	MedikitEntity(GameDataRef data,sf::Vector2f position);
	Entity* pickedBy(Character* character);
	Entity::EntityType getEntityType();

	~MedikitEntity();

};

