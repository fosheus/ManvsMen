#include "DesertEagleWeapon.h"

DesertEagleWeapon::DesertEagleWeapon(GameDataRef _data,int ammo) : Weapon(_data,Weapon::WeaponType::DesertEagle,ammo)
{
	if (ammo < 0) {
		ammo = -1;
	}
	if (ammo > DESERT_EAGLE_MAGAZINE || ammo == -1) {
		this->magazine = DESERT_EAGLE_MAGAZINE;
	}
	else {
		this->magazine = ammo;
	}
	ableToAim = false;

	weaponSound.setBuffer(*this->_data->assets.GetSound(DESERT_EAGLE_SOUND));
	weaponSound.setVolume(50.0f);
}

DesertEagleWeapon::~DesertEagleWeapon()
{
}

int DesertEagleWeapon::getShootSpeed()
{
	return DESERT_EAGLE_REARMTIME;
}

int DesertEagleWeapon::getShootRange()
{
	return DESERT_EAGLE_RANGE;
}

int DesertEagleWeapon::getReloadingTime()
{
	return DESERT_EAGLE_RELOADTIME;
}

bool DesertEagleWeapon::magazineFull()
{
	return magazine==DESERT_EAGLE_MAGAZINE;
}

int DesertEagleWeapon::getAimingTime()
{
	return 0;
}

void DesertEagleWeapon::autoReload()
{
	reloadWithMagazine(DESERT_EAGLE_MAGAZINE);
}

std::string DesertEagleWeapon::weaponInfos()
{
	return "Desert Eagle " + std::to_string(this->magazine) + " | "+std::to_string(this->ammo);
}

void DesertEagleWeapon::shoot(float x, float y, float angle)
{
	weaponSound.play();
	bullets.push_back(Bullet(x, y, angle, DESERT_EAGLE_DAMAGES, DESERT_EAGLE_RANGE));
	magazine--;
	resetRearmStatus();
}
