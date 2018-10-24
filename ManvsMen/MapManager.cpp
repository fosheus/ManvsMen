#include "MapManager.h"


MapManager::MapManager() :  pathFinder() {

}

void MapManager::generateMap(int width, int height, int blockSize) {
	this->map = new MapGenerator(width, height, blockSize, blockSize);
}

std::vector<sf::Vector2f> MapManager::getPathFromTo(sf::Vector2f from, sf::Vector2f to) {
	Point start = map->getMapPosition(Point(from.x, from.y));
	Point end = map->getMapPosition(Point(to.x, to.y));
	std::vector<Point> points;
	std::vector<sf::Vector2f> path;
	pathFinder.setStartPoint(start);
	pathFinder.setDestPoint(end);
	points = pathFinder.pathFinding(map->getMatrix(), map->getWidth(), map->getHeight());
	for (size_t i = 0; i < points.size(); i++)
	{
		Point p = map->getWorldPosition(points[i]);
		path.push_back(sf::Vector2f(p.x, p.y));
	}
	return path;
}

std::vector<sf::Vector2f> MapManager::getPathFromToAvoidingPlayer(sf::Vector2f enemyPos, sf::Vector2f to, sf::Vector2f playerPos, float playerViewFieldRadius) {
	int** matrixWithPlayerPos=NULL;
	sf::Vector2f from = enemyPos;
	float distance = Utils::distance(from, playerPos);
	playerPos = Utils::getVector2fFromPoint(map->getMapPosition(Point(playerPos.x, playerPos.y)));
	float avgMapSize = (map->getWidth() + map->getHeight()) / 2;
	float mapDistanceViewField = avgMapSize * playerViewFieldRadius / (avgMapSize*map->getBlockSize()) ;
	
	if (distance > playerViewFieldRadius*0.8) {
		matrixWithPlayerPos = new int*[map->getWidth()];
		for (size_t i = 0; i < map->getWidth(); i++)
		{
			matrixWithPlayerPos[i] = new int[map->getHeight()];
			for (size_t j = 0; j < map->getHeight(); j++)
			{
				if (Utils::distance(playerPos, sf::Vector2f(i, j)) <= mapDistanceViewField) {
					matrixWithPlayerPos[i][j] = MapGenerator::Cell_Type::WALL;
				}
				else {
					matrixWithPlayerPos[i][j] = map->getMatrix()[i][j];
				}
			}
		}
	}

	Point start = map->getMapPosition(Point(from.x, from.y));
	Point end = map->getMapPosition(Point(to.x, to.y));
	std::vector<Point> points;
	std::vector<sf::Vector2f> path;
	pathFinder.setStartPoint(start);
	pathFinder.setDestPoint(end);
	if (distance > playerViewFieldRadius*0.8) {
		points = pathFinder.pathFinding(matrixWithPlayerPos, map->getWidth(), map->getHeight());
	}
	else {
		cout << distance<<" "<<playerViewFieldRadius*0.8 << endl;
		points = pathFinder.pathFinding(map->getMatrix(), map->getWidth(), map->getHeight());
	}
	for (size_t i = 0; i < points.size(); i++)
	{
		Point p = map->getWorldPosition(points[i]);
		path.push_back(sf::Vector2f(p.x, p.y));
	}
	return path;
}

bool MapManager::isEntityInMapBounds(sf::Vector2f entityPos) {
	Point p = map->getMapPosition(Point(entityPos.x, entityPos.y));
	if (p.x < 0 || p.x >= map->getWidth() || p.y < 0 || p.y >= map->getHeight()) {
		return false;
	}
	return true;
}
bool MapManager::isEntityOnPathCell(sf::Vector2f entityPos) {
	Point p = map->getMapPosition(Point(entityPos.x, entityPos.y));
	return map->isPositionAPath(p);
}
sf::Vector2f MapManager::getFirstPathCellTopLeft() {
	Point p = map->getFirstPathCellTopLeft();
	p = map->getWorldPosition(p);
	return sf::Vector2f(p.x, p.y);
}

sf::Vector2f MapManager::getRandomPathPoint() {
	Point p = map->getRandomPathPoint();
	p = map->getWorldPosition(p);
	return sf::Vector2f(p.x, p.y);
}

sf::Vector2f MapManager::getRandomPathPointInOrOutRange(float range,bool inRange,sf::Vector2f position) {
	Point p;
	if (inRange) {
		p = map->getRandomPathPointInRange(range / map->getBlockSize(),map->getMapPosition(Point(position.x,position.y)));
	}
	else {
		p = map->getRandomPathPointOutRange(range / map->getBlockSize(),map->getMapPosition(Point(position.x, position.y)));
	}
	p = map->getWorldPosition(p);
	return sf::Vector2f(p.x, p.y);
}
std::vector<sf::Vector2f> MapManager::getSafePoints(sf::Vector2f entityPos, sf::Vector2f enemysEntityPos) {
	float distanceFromThis;
	float distanceFromPlayer;
	std::vector<sf::Vector2f> safePoints;
	for (size_t j = 0; j < map->getHeight(); j++)
	{
		for (size_t i = 0; i < map->getWidth(); i++)
		{
			if (map->getMatrix()[i][j] == MapGenerator::Cell_Type::PATH) {
				sf::Vector2f v= Utils::getVector2fFromPoint(map->getWorldPosition(Point(i,j)));
				
				distanceFromThis = Utils::distance(entityPos, v);
				distanceFromPlayer = Utils::distance(enemysEntityPos, v);
				if (distanceFromThis < distanceFromPlayer) {
					safePoints.push_back(v);
				}
			}
		}
	}
	return safePoints;
}

sf::Vector2f MapManager::getClosestPathCell(sf::Vector2f entityPos) {
	int i = 1;
	sf::Vector2f point(0, 0);
	float distance = 999999999.9f;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0) continue;
			int checkX = entityPos.x + i*x;
			int checkY = entityPos.y + i*y;
			sf::Vector2f p = sf::Vector2f(checkX, checkY);
			float distanceTmp = Utils::distance(entityPos, p);
			if (distanceTmp < distance) {
				if (isEntityOnPathCell(p)) {
					point = p;
					distance = distanceTmp;
				}
			}
		}
	}
	return point;
}


MapManager::~MapManager()
{
}
