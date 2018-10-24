#include "WeaponEntity.h"


WeaponEntity::WeaponEntity(GameDataRef data,sf::Vector2f position, Weapon* weapon) : weapon(weapon),Entity(data,position)
{
	if (weapon) {
		switch (weapon->getWeaponType())
		{
		case Weapon::WeaponType::Rifle:
			sprite.setTexture(this->_data->assets.GetImage(RIFLE_ENTITY));
			break;
		case Weapon::WeaponType::Shotgun:
			sprite.setTexture(this->_data->assets.GetImage(SHOTGUN_ENTITY));
			break;
		case Weapon::WeaponType::Sniper:
			sprite.setTexture(this->_data->assets.GetImage(SNIPER_ENTITY));
			break;
		case Weapon::WeaponType::DesertEagle:
			sprite.setTexture(this->_data->assets.GetImage(DESERTEAGLE_ENTITY));
			break;
		default:
			break;
		}
	}
}

Entity * WeaponEntity::pickedBy(Character * character)
{
	Entity* newEntity = NULL;
	if (weapon) {
		Weapon* oldWeapon = character->getWeapon();
		if (weapon->getWeaponType() == oldWeapon->getWeaponType())
		{
			character->getWeapon()->addAmmo(weapon->getAmmo());
			return NULL;
		}
		else {
			if (character->canPickWeapon()) {
				character->changeWeapon(weapon);
				newEntity = new WeaponEntity(this->_data, sprite.getPosition(), oldWeapon);
				return newEntity;
			}
			else {
				return this;
			}
		}
		
	}
	else {
		return this;
	}
}

Entity::EntityType WeaponEntity::getEntityType()
{
	return Entity::EntityType::Weapon;
}

WeaponEntity::~WeaponEntity() {

}