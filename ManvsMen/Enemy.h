#pragma once
#include "Character.h"
#include "Player.h"
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <cstdlib>
#include "MapManager.h"
#include "AStar.h"
#include "Point.h"
#include <iostream>
#include "Utils.h"
#include <stack>

#define ENEMY_ESCAPEFAILTIMEOUT 30


class Enemy :
	public Character
{

private:
	float viewFieldRadius;
	bool moving;
	sf::Vector2f targetPoint;
	Player* targetPlayer;
	bool targetPointFound;
	bool targetSeen;
	int** matrixWithPlayerPos;
	bool pathFound;
	bool escapeFound;
	bool threadRunning;
	char escapeSearchFail;
	int shootTimeout;
	sf::Thread m_thread;
	sf::Thread thread_findPathAway;
	std::vector<sf::Vector2f> pathPoints;
	std::stack<sf::Vector2f> seenPositionStack;
	
	sf::Vector2f lastBoxTargetPoint[2] = { sf::Vector2f(0,0),sf::Vector2f(0,0) };
public:
	


private:
	void moveToTargetPoint(float multiplier);
	bool targetInViewFieldRadius();
	bool targetInViewFieldPeriphery();
	void findPath();
	void findPathAwayFromPlayer();
	void updatePosition(float multiplier);
	void updateWeapon(float multiplier);



	

public:
	Enemy(GameDataRef data);
	Enemy(GameDataRef data,float x, float y, float angle, float viewFieldRadius =ENEMY_VIEW_FIELD_RADIUS);

	//getter setter
	float getViewFieldRadius();
	void setViewFieldRadius(float val) { this->viewFieldRadius = val; };
	bool getMoving() { return moving; }
	void setTargetPlayer(Player* targetPlayer);
	void setTargetPoint(sf::Vector2f targetPoint);
	Player* getTargetPlayer();
	void startMoving() { this->moving = true; }
	void stopMoving() { this->moving = false; }
	sf::Vector2f getTargetPoint() { return targetPoint; }


	//operations
	void update(float multiplier);
	void findRandomTargetPoint(sf::Vector2f posMin, sf::Vector2f posMax);
	bool targetInViewField(sf::Vector2f target );
	void move(Direction direction, float multiplier);
	void draw(sf::RenderWindow& window);
	void shoot();

	~Enemy();
};
