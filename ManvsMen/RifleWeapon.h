#pragma once

#include "Weapon.h"
class RifleWeapon :public Weapon
{
public:
	RifleWeapon(GameDataRef data, int ammo);

	~RifleWeapon();

	int getShootSpeed() ;
	int getShootRange();
	int getReloadingTime() ;

	void autoReload();

	std::string weaponInfos() ;
	void shoot(float x, float y, float angle) ;


	// Hérité via Weapon
	virtual bool magazineFull() override;


	// Hérité via Weapon
	virtual int getAimingTime() override;

};

