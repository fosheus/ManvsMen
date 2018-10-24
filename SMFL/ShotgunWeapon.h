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

	// Hérité via Weapon
	virtual bool magazineFull() override;

	// Hérité via Weapon
	virtual int getAimingTime() override;
};

