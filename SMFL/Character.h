#pragma once
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include "ImageManager.h"
#include "MapManager.h"
#include "Animation.h"
#include "Weapon.h"
#include "RifleWeapon.h"
#include "ShotgunWeapon.h"
#include "SniperWeapon.h"
#include "GunWeapon.h"
#include "Utils.h"
#include <string>
#include <thread>
#include "Game.h"


#define TEXTURE_SCALE 0.5
#define TEXTURE_CENTER sf::Vector2f(96,120)
#define TEXTURE_NUMBER 2
#define TEXTURE_TOP_LEFT_OFFSET sf::Vector2f((35-TEXTURE_CENTER.x)*TEXTURE_SCALE,(35-TEXTURE_CENTER.y)*TEXTURE_SCALE)
#define TEXTURE_WEAPON_POINT_OFFSET sf::Vector2f((291-TEXTURE_CENTER.x)*TEXTURE_SCALE,(150-TEXTURE_CENTER.y)*TEXTURE_SCALE)
#define TEXTURE_HITBOX_SIZE sf::Vector2f(205*TEXTURE_SCALE,155*TEXTURE_SCALE)
#define BULLETSHAPE_WIDTH 30
#define BULLETSHAPE_HEIGHT 6
#define DEFAULT_SPEED 5
#define HEAL_PROCESS_TIMEOUT 120
#define CHARACTER_RADIUS 90*TEXTURE_SCALE

class Character 
{

public:
	enum Direction {
		Forward=1,
		Backward=2,
		Left=4,
		Right=8,
		Up=16,
		Down=32,
		West=64,
		East=128,
		Target=256,
		Back=512,
		None=0
	};

private:



protected :
	float x;
	float y;
	float angle;
	float speed;


	Weapon* weapon=NULL;
	int currentTexture;
	int maxHealth;
	int health;
	int protection;
	int maxProtection;
	float movingStatus;
	bool pickWeapon;
	int killCount;
	bool shooting;
	bool aiming;

	sf::Sprite characterSprite;
	sf::RectangleShape bulletShape;
	Animation *animation = NULL;

	std::string textureMovement[2];
	float healProcess;
	Direction lastDirection;

	GameDataRef _data;


private : 
	void init();


protected :
	void moveForward(float multiplier);
	void moveBackward(float multiplier);
	void moveLeft(float multiplier);
	void moveRight(float multiplier);
	void moveDown(float multiplier);
	void moveUp(float multiplier);
	void moveWest(float multiplier);
	void moveEast(float multiplier);
	void moveBack(float multiplier);
	void updateTexture(float multipler);
	virtual void updateWeapon(float multiplier) = 0;

	

public:
	Character(GameDataRef data);
	Character(GameDataRef data,float x, float y, float angle);
	/*GET SET*/
	float getX() const { return x; }
	float getY() const { return y; }
	float getRotation() const { return angle; }
	float getSpeed() const { return speed; }
	int getProtection() { return protection; }
	float getPercentProtection() { return (float)((float)this->protection / (float)this->maxProtection); }
	bool canPickWeapon() { return pickWeapon; }
	int getKillCount() { return killCount; }
	bool isShooting() { return shooting; }
	bool isAiming() { return aiming; }

	sf::Vector2f getPosition() { return sf::Vector2f(x, y); }
	void setPosition(sf::Vector2f position) { x = position.x; y = position.y; }
	void setPosition(float x, float y);
	void setRotation(float angle);
	void setProtection(int protection);
	void addProtection(int protection);

	/*health*/
	bool hurt(int damages);
	bool heal(int points);
	
	int getHealth() { return health; }
	void setHealth(int health) { this->health = health; }
	int setMaxHealth(int maxHealth) { this->maxHealth = maxHealth; }
	float getPercentHealth() { return (float)((float)this->health / (float)this->maxHealth); }
	bool isDead() { return health <= 0; }

	/*weapon*/
	Weapon* getWeapon() { return weapon; }
	std::vector<Bullet>& getBullets();
	int getBulletNumber();
	int getMagazine();
	int getAmmo();

	sf::Sprite getSprite() { return characterSprite; }

	void move(float angle,float multiplier);

	/*OPERATIONS*/
	
	std::string getWeaponInfo();
	sf::RectangleShape Character::getMinBounds();
	//Entity* pickAndDropEntity(Entity* entity);
	bool outOfBounds(sf::Vector2f minWorldBounds, sf::Vector2f maxWorldBounds);
	void hurtCharacter(Character* character,int damages);

	void testBoundsAndRePosition(sf::Vector2f minWorldBounds, sf::Vector2f maxWorldBounds);
	std::string toString();
	
	/*weapon*/
	void changeWeapon(Weapon* weapon);
	
	virtual void update(float multiplier) = 0;
	virtual void shoot() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual ~Character()=0;
};

