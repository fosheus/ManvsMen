#include "RifleWeapon.h"



RifleWeapon::RifleWeapon(GameDataRef data, int ammo) : Weapon(data,Weapon::WeaponType::Rifle,ammo)
{
	if (ammo < 0) {
		ammo = -1;
	}
	if (ammo > RIFLE_MAGAZINE || ammo ==-1) {
		this->magazine = RIFLE_MAGAZINE;
	}
	else {
		this->magazine = ammo;
	}
	ableToAim = false;

	weaponSound.setBuffer(*this->_data->assets.GetSound(RIFLE_SOUND));
	weaponSound.setVolume(50.0f);
}


RifleWeapon::~RifleWeapon()
{
}

int RifleWeapon::getShootSpeed()
{
	return RIFLE_REARMTIME;
}

int RifleWeapon::getShootRange()
{
	return RIFLE_RANGE;
}

int RifleWeapon::getReloadingTime()
{
	return RIFLE_RELOADTIME;
}

std::string RifleWeapon::weaponInfos() {
	return "Rifle " + std::to_string(magazine) + " | " + std::to_string(ammo);
}

void RifleWeapon::shoot(float x, float y, float angle)
{
	weaponSound.play();
	bullets.push_back(Bullet(x, y, angle, RIFLE_DAMAGES, RIFLE_RANGE));
	magazine--;
	resetRearmStatus();
}

bool RifleWeapon::magazineFull()
{
	return magazine==RIFLE_MAGAZINE;
}

int RifleWeapon::getAimingTime()
{
	return 0;
}

void RifleWeapon::autoReload() {
	reloadWithMagazine(RIFLE_MAGAZINE);
}


