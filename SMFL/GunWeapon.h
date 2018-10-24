#pragma once
#include "Weapon.h"

class GunWeapon : public Weapon
{
public:
	GunWeapon(GameDataRef data,int ammo);
	~GunWeapon();

	// Hérité via Weapon
	virtual int getShootSpeed() override;
	virtual int getShootRange() override;
	virtual int getReloadingTime() override;
	virtual void autoReload() override;
	virtual std::string weaponInfos() override;
	virtual void shoot(float x, float y, float angle) override;

	// Hérité via Weapon
	virtual bool magazineFull() override;


	// Hérité via Weapon
	virtual int getAimingTime() override;

};

