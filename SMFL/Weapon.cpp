#include "Weapon.h"
#include "GunWeapon.h"
#include "RifleWeapon.h"
#include "ShotgunWeapon.h"
#include "SniperWeapon.h"
#include "DesertEagleWeapon.h"

void Weapon::reloadWithMagazine(int magSize)
{
	if (asInfiniteAmmo()) {
		magazine = magSize;
	}
	else if (!noAmmo()) {
		if (magazine == magSize) {
			//nothing
		}
		else if (ammo <= magSize) {

			if (magazine == 0) {
				magazine = ammo;
				ammo = 0;
			}
			else {
				int oldMag = magazine;
				magazine = magazine + ammo;
				ammo = ammo - (magSize - oldMag);
				if (ammo < 0) {
					ammo = 0;
				}
				else {
					magazine = magSize;
				}
			}
		}
		else {
			ammo = ammo - (magSize - magazine);
			magazine = magSize;

		}
	}
}

Weapon::Weapon(GameDataRef data,WeaponType weaponType,int ammo) : _data(data), weaponType(weaponType),ammo(ammo)
{
	reloading = false;
	reloadStatus = 0;
	rearmStatus = 0;
}



void Weapon::updateAndKillBulletsOutOfBounds(float multiplier, sf::Vector2f minWorldBounds, sf::Vector2f maxWorldBounds) {
	for (size_t i = 0; i < bullets.size(); i++)
	{
		bullets[i].move(multiplier);
		if (bullets[i].isDead()) {
			bullets.erase(bullets.begin() + i);
		}
	}
}

Weapon * Weapon::weaponFromWeaponType(GameDataRef data, WeaponType type, int ammo)
{
	switch (type)
	{
	case WeaponType::Gun:
		return new GunWeapon(data, ammo);
		break;
	case WeaponType::Rifle:
		return new RifleWeapon(data, ammo);
		break;
	case WeaponType::Shotgun:
		return new ShotgunWeapon(data, ammo);
		break;
	case WeaponType::Sniper:
		return new SniperWeapon(data, ammo);
		break;
	case WeaponType::DesertEagle:
		return new DesertEagleWeapon(data, ammo);
		break;
	default:
		return nullptr;
		break;
	}
}
Weapon * Weapon::weaponFromWeaponType(GameDataRef data, WeaponType type)
{
	switch (type)
	{
	case WeaponType::Gun:
		return new GunWeapon(data, GUN_MAGAZINE*3);
		break;
	case WeaponType::Rifle:
		return new RifleWeapon(data, RIFLE_MAGAZINE*3);
		break;
	case WeaponType::Shotgun:
		return new ShotgunWeapon(data, SHOTGUN_MAGAZINE*3);
		break;
	case WeaponType::Sniper:
		return new SniperWeapon(data, SNIPER_MAGAZINE*3);
		break;
	case WeaponType::DesertEagle:
		return new DesertEagleWeapon(data, DESERT_EAGLE_MAGAZINE * 3);
		break;
	default:
		return nullptr;
		break;
	}
}

void Weapon::reload()
{
	if (!magazineFull() && reloading == false) {
		reloading = true;
		reloadStatus = 0;
	}
}

void Weapon::update(float multiplier,bool aiming)
{
	if (rearmStatus < getShootSpeed()) {
		rearmStatus += multiplier;
	}
	
	if (isMagazineEmpty() && reloading == false) {
		reloading = true;
		reloadStatus = 0;
	}
	if (reloading) {
		reloadStatus += multiplier;
		if (reloadStatus >= getReloadingTime() && rearmStatus >= getShootSpeed()) {
			reloading = false;
			reloadStatus = 0;
			autoReload();
		}
	}
	if (canAim()) {
		if (aiming) {
			if (aimingStatus > 1.0f) {
				aimingStatus -= multiplier;
				if (aimingStatus<1.0f) {
					aimingStatus = 1.0f;
				}

			}
			
		}
		else {
			aimingStatus = getAimingTime();
		}
	}

}

bool Weapon::canShoot()
{
	return (rearmStatus >= getShootSpeed()) && !reloading;
}

bool Weapon::canAim()
{
	return ableToAim;
}

Weapon::~Weapon()
{
	bullets.clear();
}

float Weapon::getReloadingProgress()
{

	if (reloading == false) {
		return -1.0f;
	}
	else {
		float result = reloadStatus / (float)getReloadingTime();
		return result > 1.0f ? 1.0f : result;
	}
}
