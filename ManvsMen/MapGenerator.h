#pragma once
#include <cstdlib>
#include <SFML\Graphics.hpp>
#include "Point.h"
#include "AVL.h"
#include "Utils.h"
#include "ImageManager.h"
class MapGenerator
{
public :
	enum Cell_Type {
		WALL = 0,
		PATH = 1,
		CONCRETE = 2
	};

private :
	int **matrix;
	size_t height;
	size_t width;
	size_t blocWidth;
	size_t blocHeight;

	AVL tree;
	std::vector<nodeptr> roots;


	int** openMatrix(int** matrix, int multiplier);
	int** closeMatrix(int** matrix, int multiplier);

	void fillAVL();
	void addAdjToSameTree(int x, int y, int **visitedMatrix, int currentTree);
	std::vector<Point> getNeighbours(Point point);
	void removeDiagonal();
	void optimizeRendering();
public:


	MapGenerator(size_t height, size_t width, size_t blocWidth, size_t blocHeight);

	int** getMatrix() { return matrix; }
	int getWidth() { return width; }
	int getHeight() { return height;}
	int getBlockSize() { return blocWidth; }
	
	Point getWorldPosition(Point mapPosition);
	Point getMapPosition(Point worldPosition);
	
	Point getRandomPathPoint();
	Point MapGenerator::getRandomPathPointInRange(int range,Point point);
	Point MapGenerator::getRandomPathPointOutRange(int range, Point point);

	bool isPositionAPath(Point p);
	Point getFirstPathCellTopLeft();

	
	~MapGenerator();
};

