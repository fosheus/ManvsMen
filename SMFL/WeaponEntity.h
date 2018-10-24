#pragma once
#include "Entity.h"
#include "SFML\Graphics.hpp"
#include "Weapon.h"
#include "Character.h"
class WeaponEntity :
	public Entity
{
private :
	Weapon* weapon;
public:
	WeaponEntity(GameDataRef data,sf::Vector2f position,Weapon* weapon);
	Weapon* getWeapon() { return weapon; }
	Entity* pickedBy(Character* character);
	Entity::EntityType getEntityType();

	~WeaponEntity();


};

