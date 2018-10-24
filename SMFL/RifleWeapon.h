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


	// H�rit� via Weapon
	virtual bool magazineFull() override;


	// H�rit� via Weapon
	virtual int getAimingTime() override;

};

