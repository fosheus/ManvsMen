#include "SniperWeapon.h"



SniperWeapon::SniperWeapon(GameDataRef data, int ammo) : Weapon(data, Weapon::WeaponType::Sniper, ammo)
{
	if (ammo < 0) {
		ammo = -1;
	}
	if (ammo > SNIPER_MAGAZINE || ammo == -1) {
		this->magazine = SNIPER_MAGAZINE;
	}
	else {
		this->magazine = ammo;
	}
	ableToAim = true;
	aimingStatus = 120.0f;

	weaponSound.setBuffer(*this->_data->assets.GetSound(SNIPER_SOUND));
	weaponSound.setVolume(50.0f);
}


SniperWeapon::~SniperWeapon()
{
}

int SniperWeapon::getShootSpeed()
{
	return SNIPER_REARMTIME;
}

int SniperWeapon::getShootRange()
{
	return SNIPER_RANGE;
}

int SniperWeapon::getReloadingTime()
{
	return SNIPER_RELOADTIME;
}

bool SniperWeapon::magazineFull()
{
	return magazine == SNIPER_MAGAZINE;
}


float SniperWeapon::getCurrentPrecisionAngle()
{
	return SNIPER_PRECISION_ANGLE*aimingStatus / SNIPER_AIMINGTIME;
}

float SniperWeapon::getAimingProgress()
{
	return (float)(SNIPER_AIMINGTIME - aimingStatus) / (float)SNIPER_AIMINGTIME;
}


void SniperWeapon::autoReload()
{
	reloadWithMagazine(SNIPER_MAGAZINE);
}

std::string SniperWeapon::weaponInfos()
{
	return "Sniper " + std::to_string(magazine) + " | " + std::to_string(ammo);

}

void SniperWeapon::shoot(float x, float y, float angle)
{
	weaponSound.play();
	float precisionAngle = getCurrentPrecisionAngle();
	float precision = (rand() % (((int)precisionAngle+1)*2))-precisionAngle;

	bullets.push_back(Bullet(x, y, angle+precision, SNIPER_DAMAGES, SNIPER_RANGE));
	magazine--;
	resetRearmStatus();
}

int SniperWeapon::getAimingTime()
{
	return SNIPER_AIMINGTIME;
}
