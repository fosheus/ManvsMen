#pragma once
#include "Weapon.h"

class DesertEagleWeapon :
	public Weapon
{
public:
	DesertEagleWeapon(GameDataRef _data, int ammo);
	~DesertEagleWeapon();

	// Hérité via Weapon
	virtual int getShootSpeed() override;
	virtual int getShootRange() override;
	virtual int getReloadingTime() override;
	virtual bool magazineFull() override;
	virtual int getAimingTime() override;
	virtual void autoReload() override;
	virtual std::string weaponInfos() override;
	virtual void shoot(float x, float y, float angle) override;
};

