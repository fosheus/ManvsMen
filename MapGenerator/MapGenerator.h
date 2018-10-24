#pragma once
#include <cstdlib>
#include <SFML\Graphics.hpp>
#include "Point.h"
#include "AVL.h"
class MapGenerator
{

public :
	enum Cell_Type {
		WALL = 0,
		PATH = 1
	};
private:
	int **matrix;
	size_t height;
	size_t width;
	size_t blocWidth;
	size_t blocHeight;

	sf::RectangleShape vert;
	sf::RectangleShape bleu;
	AVL tree;
	std::vector<nodeptr> roots;


	int** openMatrix(int** matrix, int multiplier);
	int** closeMatrix(int** matrix, int multiplier);

	void fillAVL();
	void addAdjToSameTree(int x, int y, int **visitedMatrix, int currentTree);
	bool isSafeAndPath(int x, int y);
	std::vector<Point> getNeighbours(Point point);

public:

	int** getMatrix() { return matrix; }
	int getWidth() { return width; }
	int getHeight() { return height;}
	MapGenerator(size_t height, size_t width, size_t blocWidth, size_t blocHeight);
	Point getWorldPosition(Point mapPosition);
	Point getRandomPathPoint();
	void draw(sf::RenderWindow &window);
	~MapGenerator();
};

