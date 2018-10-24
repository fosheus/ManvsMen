#include "GunWeapon.h"

GunWeapon::GunWeapon(GameDataRef data,int ammo) : Weapon(data,Weapon::WeaponType::Gun, ammo)
{
	if (ammo < 0) {
		ammo = -1;
	}
	if (ammo > GUN_MAGAZINE || ammo == -1) {
		this->magazine = GUN_MAGAZINE;
	}
	else {
		this->magazine = ammo;
	}
	ableToAim = false;

	weaponSound.setBuffer(*this->_data->assets.GetSound(GUN_SOUND));
	weaponSound.setVolume(50.0f);
	
}

GunWeapon::~GunWeapon()
{
}

int GunWeapon::getShootSpeed()
{
	return GUN_REARMTIME;
}

int GunWeapon::getShootRange()
{
	return GUN_RANGE;
}

int GunWeapon::getReloadingTime()
{
	return GUN_RELOADTIME;
}

void GunWeapon::autoReload()
{
	reloadWithMagazine(GUN_MAGAZINE);
}

std::string GunWeapon::weaponInfos()
{
	return "Gun " + std::to_string(this->magazine)+ " | Infinite";
}

void GunWeapon::shoot(float x, float y, float angle)
{
	weaponSound.play();
	bullets.push_back(Bullet(x, y, angle, GUN_DAMAGES, GUN_RANGE));
	magazine--;
	resetRearmStatus();
}

bool GunWeapon::magazineFull()
{
	return magazine == GUN_MAGAZINE;
}

int GunWeapon::getAimingTime()
{
	return 0;

}

