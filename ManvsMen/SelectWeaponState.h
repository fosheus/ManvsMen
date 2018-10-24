#pragma once

#include "Animation.h"
#include "State.h"
#include "Game.h"
#include "Weapon.h"
class SelectWeaponState :
	public State
{
public:
	SelectWeaponState(GameDataRef data);
	~SelectWeaponState();

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
private :
	Weapon::WeaponType weaponTypeFromSelectedIndex(int selectedIndex);
private:

	float multiplier = 60.0f;
	GameDataRef _data;

	sf::Clock _clock;

	Animation* animation; 

	sf::Sprite _background;

	sf::Sprite _selectLeft;
	sf::Sprite _selectRight;
	sf::Sprite _selectedWeapon;
	sf::RectangleShape _weaponDamage;
	sf::RectangleShape _weaponDamageMax;
	sf::RectangleShape _weaponRange;
	sf::RectangleShape _weaponRangeMax;
	sf::RectangleShape _weaponShootSpeed;
	sf::RectangleShape _weaponShootSpeedMax;

	sf::Text damage;
	sf::Text range;
	sf::Text shoot;
	sf::Text magazine;
	sf::Text weaponName;

	sf::Sprite _returnButton;
	sf::Sprite _playButton;



	int selectedWeaponIndex;
};

