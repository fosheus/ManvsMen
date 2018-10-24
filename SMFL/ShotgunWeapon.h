#pragma once
#include "Weapon.h"


class ShotgunWeapon : public Weapon
{
public:
	ShotgunWeapon(GameDataRef data, int ammo);

	int getShootSpeed();
	int getShootRange();
	int getReloadingTime();

	void autoReload();

	std::string weaponInfos();
	void shoot(float x, float y, float angle);

	~ShotgunWeapon();

	// H�rit� via Weapon
	virtual bool magazineFull() override;

	// H�rit� via Weapon
	virtual int getAimingTime() override;
};

