#pragma once
#include "SFML\Graphics.hpp"
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include "Character.h"
#include "Bullet.h"
#include "Weapon.h"
#include "ImageManager.h"
#include <vector>
#include <string>
#include "Utils.h"
#include <math.h>
#include <iostream>


class Player :
	public Character
{

private : 
	
	
	bool holdPickWeapon;
	bool holdReload;

private:
	void updateWeapon(float multiplier);

public:
	Player(GameDataRef data);
	Player(GameDataRef data, float x, float y, float angle);

	void move(Direction direction,float multiplier);
	float getReloadingProgress();

	void update(float multiplier);
	void draw(sf::RenderWindow &window);	
	void shoot();

	std::string toString();

	~Player();
};

