#include "MedikitEntity.h"


MedikitEntity::MedikitEntity(GameDataRef data,sf::Vector2f position) : Entity(data,position)
{
	sprite.setTexture(this->_data->assets.GetImage(MEDIKIT_ENTITY));
	health = MEDIKIT_HEALTH;

}

Entity * MedikitEntity::pickedBy(Character * character)
{
	if (character->heal(getHealth())) {
		return NULL;
	}
	else {
		return this;
	}
}

Entity::EntityType MedikitEntity::getEntityType()
{
	return Entity::EntityType::Medikit;
}

MedikitEntity::~MedikitEntity()
{
}
