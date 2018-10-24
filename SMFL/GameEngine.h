#pragma once
#include "SFML\Graphics.hpp"
#include "Player.h"
#include "Enemy.h"
#include "ImageManager.h"
#include "MapManager.h"
#include "SoundManager.h"
#include "WeaponEntity.h"
#include "MedikitEntity.h"
#include <iostream>
#include <SFML\Audio.hpp>
#include "Utils.h"
#include <vector>
#include "GameMenu.h"
#include "TileMap.h"

#define BLOCKSIZE 100
#define MAP_WIDTH 40
#define MAP_HEIGHT 40
#define MINWORLDBOUNDS sf::Vector2f(20,20)
#define MAXWORLDBOUNDS sf::Vector2f(BLOCKSIZE*MAP_WIDTH,BLOCKSIZE*MAP_HEIGHT)
#define BACKGROUND_TEXTURE "images/background.png"
#define ARIAL_FONT "font/arial.ttf"
#define PIXEL_FONT "font/pixel.ttf"
#define NUMBER_ENEMIES 3
#define HEALTHMAXSIZE sf::Vector2f(100,10)
#define PLAYERVIEWFIELDRADIUS 1200


using namespace sf;


class GameEngine
{

public :
	MapManager mapManager;
	ImageManager imageManager;
	SoundManager soundManager;
private :

	static GameEngine m_instance;



	TileMap map;
	// an SFML RenderWindow
	bool Running;
	bool displayGameOver;
	bool displayMenu;
	bool focus;

	sf::RenderWindow window;
	sf::View mainView;
	// declare a Sprite and Texture for the background
	

	Player player;
	vector<Enemy*> enemies;
	vector<Entity*> entities;

	Font font;
	Text text, text2,textFps;
	Text ammoInfo;
	RectangleShape healthBar;
	Sprite gameover;

	GameMenu menu;

	bool zoomhold;
	bool uphold;
	bool downhold;
	bool enterhold;
	
	
	Clock clock;
	float dt = 0.f;
	float multiplier=60.0f;
	int count = 0;

	void entitiesUpdate(float multiplier);
	void playerUpdate(float multiplier);
	void enemiesUpdate(float multiplier);
	void playerBulletsCollisionEnemies(float multiplier);
	void playerCollideEntities(float mulitplier);
	bool managerMenu();
	void spawnEnemy(int health, Weapon::WeaponType weaponType, int ammo);
	void spawnEnemy();
	void spawnWeapon(sf::Vector2f position, Weapon& weapon);

	

public:
	static GameEngine& getInstance();


	void InitGame();
	void InputManagement();
	bool IsRunning();
	void Update();
	void Draw();
	
private : 
	GameEngine& operator= (const GameEngine&);
	GameEngine(const GameEngine&);
	GameEngine();
	~GameEngine();

	
};


