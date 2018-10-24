#include "KevlarEntity.h"



Entity* KevlarEntity::pickedBy(Character * character)
{
	Entity* newEntity = NULL;

	if (!character) {
		return NULL;
	}
	character->addProtection(protection);
	return NULL;

	/*
	if (character->getProtection()) {
		newEntity = new KevlarEntity(_data, this->sprite.getPosition(), character->getProtection());
		character->setProtection(this->protection);
	}
	else {
		character->setProtection(this->protection);
	}

	return newEntity;

	*/
}

Entity::EntityType KevlarEntity::getEntityType()
{
	return EntityType::Kevlar;
}

KevlarEntity::KevlarEntity(GameDataRef data,sf::Vector2f position, int protection) : Entity(data,position)
{
	this->protection = protection;
	if (this->protection < 0 ) {
		this->protection = 0;
	} 
	if (this->protection > 100) {
		this->protection = 100;
	}
	this->sprite.setTexture(data->assets.GetImage("images/kevlar.png"));
}

KevlarEntity::~KevlarEntity()
{
}
