#pragma once
#include <iostream>
#include "SFML\Graphics.hpp"
#include <SFML\Audio.hpp>
#include "Bullet.h"
#include <string>
#include "DEFINITIONS.h"
#include "Game.h"



class Weapon
{
public :
	//GUN must be last weapon
	enum class WeaponType
	{
		Shotgun,
		Rifle,
		Sniper,
		DesertEagle,
		Gun,
		Count
	};


protected :
	int ammo;
	int magazine;
	bool reloading;
	bool ableToAim;
	float reloadStatus;
	float rearmStatus;
	float aimingStatus;
	WeaponType weaponType;
	std::vector<Bullet> bullets;
	sf::Sound weaponSound;
	GameDataRef _data;
	

protected :
	void resetRearmStatus() { rearmStatus = 0; }
	void reloadWithMagazine(int magSize);
public:


	Weapon(GameDataRef data,WeaponType weaponType, int ammo);

	int getAmmo() { return ammo; }
	int getMagazine() { return magazine; }
	std::vector<Bullet>& getBullets() { return bullets; }
	WeaponType getWeaponType() { return weaponType; }
	bool noAmmo() { return (ammo == 0) && (magazine ==0);  }
	bool isMagazineEmpty() { return magazine == 0; }
	bool asInfiniteAmmo() { return ammo == -1; }
	void addAmmo(int value) { ammo += value; }
	void updateAndKillBulletsOutOfBounds(float multiplier, sf::Vector2f minWorldBounds, sf::Vector2f maxWorldBounds);
	float getReloadingProgress();


	static Weapon* weaponFromWeaponType(GameDataRef data,WeaponType type, int ammo);
	static Weapon* weaponFromWeaponType(GameDataRef data, WeaponType type);

	void reload();

	virtual void update(float multiplier,bool aiming);
	virtual bool canShoot();
	virtual bool canAim();

	virtual int getShootSpeed() = 0;
	virtual int getShootRange() = 0;
	virtual int getReloadingTime() = 0;
	virtual bool magazineFull() = 0;
	virtual int getAimingTime() = 0;

	virtual void autoReload()=0;
	

	virtual std::string weaponInfos() = 0;
	virtual void shoot(float x, float y, float angle) = 0;


	~Weapon();
};

