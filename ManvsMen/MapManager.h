#pragma once
#include "MapGenerator.h"
#include "AStar.h"
#include "SFML\Graphics.hpp"
#include "ImageManager.h"
class MapManager
{

private:
	MapGenerator* map;
	AStar pathFinder;


public:
	MapManager();

	void MapManager::generateMap(int width, int height, int blockSize);
	std::vector<sf::Vector2f> getPathFromTo(sf::Vector2f from, sf::Vector2f to);
	std::vector<sf::Vector2f> getPathFromToAvoidingPlayer(sf::Vector2f from, sf::Vector2f to, sf::Vector2f playerPos, float playerViewFieldRadius);
	bool isEntityInMapBounds(sf::Vector2f entityPos);
	bool isEntityOnPathCell(sf::Vector2f entityPos);
	sf::Vector2f getClosestPathCell(sf::Vector2f entityPos);
	sf::Vector2f getFirstPathCellTopLeft();
	sf::Vector2f getRandomPathPoint();
	sf::Vector2f MapManager::getRandomPathPointInOrOutRange(float range, bool inRange,sf::Vector2f position);


	int** getMatrix() {	return map->getMatrix(); }
	sf::Vector2u getMatrixSize() { if (map) { return sf::Vector2u(map->getWidth(), map->getHeight()); } return sf::Vector2u(0, 0); }
	std::vector<sf::Vector2f> getSafePoints(sf::Vector2f entityPos, sf::Vector2f enemysEntityPos);

	sf::Vector2f getMinWorldBounds() { return sf::Vector2f(0, 0); }
	sf::Vector2f getMaxWorldBounds() { if (map) { return sf::Vector2f((float)map->getWidth()*map->getBlockSize(), (float)map->getHeight()*map->getBlockSize()); } return sf::Vector2f(0.0f, 0.0f); }

	~MapManager();
};

