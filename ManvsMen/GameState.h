#pragma once
#include <SFML\Graphics.hpp>
#include "Game.h"
#include "State.h"
#include "DEFINITIONS.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Entity.h"
#include "GameMenu.h"
#include "Segment.h"
#include "IntersectionPoint.h"
#include "WeaponEntity.h"
#include "MedikitEntity.h"
#include "KevlarEntity.h"
#include "BackpackEntity.h"

#include "RifleWeapon.h"
#include "ShotgunWeapon.h"
#include "SniperWeapon.h"
#include "DesertEagleWeapon.h"
#include "GunWeapon.h"
#include <string>
#include <iostream>




class GameState : public State
{
public:
	GameState(GameDataRef data,Weapon::WeaponType playerWeapon);
	~GameState();

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
	void Pause();
	void Resume();

private:

	//DO NOT USE//
	Weapon::WeaponType playerInitialWeaponType;
	//
	GameDataRef _data;
	TileMap map;
	sf::Clock _clock;

	sf::Mutex mutex;
	sf::Thread fovThread;
	bool fovThreadStarted;
	sf::Vector2f playerPositionForFOVRendering;
	std::map<float, IntersectionPoint> intersectionPoints;

	bool Running;
	bool displayMenu;
	bool focus;

	sf::View mainView;


	Player player;
	vector<Enemy*> enemies;
	vector<Entity*> entities;
	int enemySpawned;

	sf::Text killCountDisplay, enemyNumberDisplay, textFps,healthStatus;
	sf::Text ammoInfo;
	sf::RectangleShape healthBar;
	sf::RectangleShape protectionBar;
	sf::RectangleShape reloadingBar;


	bool zoomhold;
	float zoomStatus;
	bool uphold;
	bool downhold;
	bool enterhold;


	float multiplier = 60.0f;
	int count = 0;

	void entitiesUpdate(float multiplier);
	void playerUpdate(float multiplier);
	void enemiesUpdate(float multiplier);
	void collisionManagement(float multiplier);
	void moveCharactersOpposite(Character * character1, Character * character2,float multiplier);
	void moveCharacterAwayFromPosition(Character * character, sf::Vector2f position, float multiplier);
	bool entityIsInPlayerFieldOfView(sf::Vector2f entity);
	void playerBulletsCollisionEnemies(float multiplier);
	void playerUpdateZoom(float multiplier);
	void playerCollideEntities(float mulitplier);
	void spawnEnemy();
	void spawnWeapon(sf::Vector2f position, Weapon* weapon);
	void spawnBackback( Character* character);
	IntersectionPoint* getIntersection(Segment ray, Segment segment);
	void fovOperator();


};

