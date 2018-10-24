#include "ShotgunWeapon.h"



ShotgunWeapon::~ShotgunWeapon()
{
	bullets.clear();
}

bool ShotgunWeapon::magazineFull()
{
	return magazine==SHOTGUN_MAGAZINE;
}

int ShotgunWeapon::getAimingTime()
{
	return 0;
}

ShotgunWeapon::ShotgunWeapon(GameDataRef data, int ammo) : Weapon(data, Weapon::WeaponType::Shotgun, ammo)
{
	if (ammo < 0) {
		ammo = -1;
	}
	if (ammo > SHOTGUN_MAGAZINE || ammo == -1) {
		this->magazine = SHOTGUN_MAGAZINE;
	}
	else {
		this->magazine = ammo;
	}
	ableToAim = false;

	weaponSound.setBuffer(*this->_data->assets.GetSound(SHOTGUN_SOUND));
	weaponSound.setVolume(50.0f);
}


int ShotgunWeapon::getShootSpeed()
{
	return SHOTGUN_REARMTIME;
}

int ShotgunWeapon::getShootRange()
{
	return SHOTGUN_RANGE;
}

int ShotgunWeapon::getReloadingTime()
{
	return SHOTGUN_RELOADTIME;
}

void ShotgunWeapon::autoReload()
{
	reloadWithMagazine(SHOTGUN_MAGAZINE);
}

std::string ShotgunWeapon::weaponInfos()
{
	return "Shotgun "+std::to_string(magazine) +" | "+std::to_string(ammo);
}

void ShotgunWeapon::shoot(float x, float y, float angle)
{
	weaponSound.play();
	std::vector<Bullet> projectiles;
	projectiles.push_back(Bullet(x, y, angle, SHOTGUN_DAMAGES, SHOTGUN_RANGE));
	projectiles.push_back(Bullet(x, y, angle + 30, SHOTGUN_DAMAGES*0.8, SHOTGUN_RANGE));
	projectiles.push_back(Bullet(x, y, angle - 30, SHOTGUN_DAMAGES*0.8, SHOTGUN_RANGE));
	bullets.insert(bullets.begin(), projectiles.begin(), projectiles.end());
	magazine--;
	resetRearmStatus();
}

