#include "BackpackEntity.h"
#include "WeaponEntity.h"


BackpackEntity::BackpackEntity(GameDataRef data, sf::Vector2f position,std::vector<Entity*> entities) : Entity(data, position)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		this->entities.push_back(entities[i]);
	}
	this->sprite.setTexture(_data->assets.GetImage(BACKPACK_ENTITY));
}

BackpackEntity::~BackpackEntity()
{
}

Entity* BackpackEntity::pickedBy(Character * character)
{
	Entity * ret= NULL;
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i]->getEntityType()==EntityType::Weapon)
			ret = entities[i]->pickedBy(character);
		else
			entities[i]->pickedBy(character);
	}
	return ret;
	
}

Entity::EntityType BackpackEntity::getEntityType()
{
	return EntityType::Backpack;
}
