#pragma once
#include "Entity.h"
#include <vector>
#include "DEFINITIONS.h"
class BackpackEntity : public Entity
{
private :
	std::vector<Entity*> entities;
public:
	BackpackEntity(GameDataRef data,sf::Vector2f position,std::vector<Entity*> entities);
	~BackpackEntity();

	Entity * pickedBy(Character* character);
	Entity::EntityType getEntityType();
};

