#pragma once
#include "Weapon.h"
class SniperWeapon :
	public Weapon
{
private :
public:
	SniperWeapon(GameDataRef data, int ammo);
	~SniperWeapon();
	float getCurrentPrecisionAngle();
	float getAimingProgress();

	// Hérité via Weapon
	virtual int getShootSpeed() override;
	virtual int getShootRange() override;
	virtual int getReloadingTime() override;
	virtual bool magazineFull() override;
	virtual void autoReload() override;
	virtual std::string weaponInfos() override;
	virtual void shoot(float x, float y, float angle) override;

	// Hérité via Weapon
	virtual int getAimingTime() override;
};

