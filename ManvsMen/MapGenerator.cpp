#include "MapGenerator.h"

MapGenerator::MapGenerator(size_t width, size_t height, size_t blocWidth, size_t blocHeight)
{
	srand(time(NULL));
	if (height > 0 && width > 0) {
		this->blocHeight = blocHeight;
		this->blocWidth = blocWidth;
		this->height = height;
		this->width = width;
		matrix = new int*[width];
		for (size_t i = 0; i < width; i++)
		{
			matrix[i] = new int[height];
			for (size_t j = 0; j < height; j++)
			{
				matrix[i][j] = rand() % 2;
			}
		}

		matrix = openMatrix(matrix, 2);
		matrix = closeMatrix(matrix, 1);
		matrix = closeMatrix(matrix, 2);

		fillAVL();
		int val = 0;
		int maxIndex = 0;
		if (roots.size() > 1) {
			for (size_t i = 0; i < roots.size(); i++)
			{
				int currentSize = tree.nonodes(roots[i]);
				if (currentSize > val) {
					val = currentSize;
					maxIndex = i;
				}
			}
			for (size_t i = 0; i < roots.size(); i++)
			{
				if (i == maxIndex) {
					continue;
				}
				else {
					while (!tree.isempty(roots[i]))
					{
						val = tree.deletemin(roots[i]);
						Point p(val%width, val / width);
						matrix[p.x][p.y] = WALL;
					}
				}
			}
		}
		removeDiagonal();

		optimizeRendering();

	}

}
Point MapGenerator::getRandomPathPoint() {
	bool pointCorrect = false;
	Point point;
	while (!pointCorrect) {
		int x = rand() % width;
		int y = rand() % height;
		if (matrix[x][y] == PATH) {
			point.x = x;
			point.y = y;
			return point;
		}
	}
	return Point(-1, -1);
}

Point MapGenerator::getRandomPathPointInRange(int range, Point point) {

	int tries = 0;
	int maxTries = 50;

	while (tries < maxTries) {
		tries++;
		int x = rand() % (range * 2) - range;
		int y = rand() % (range * 2) - range;
		Point p;
		p.x=point.x + x;
		p.y = point.y + y;
		if (isPositionAPath(p)) {
			return p;
		}
	}
	return Point(-1, -1);
}
Point MapGenerator::getRandomPathPointOutRange(int range,Point point) {
	bool pointCorrect = false;
	int tries = 0;
	int maxTries = 50;

	while (tries < maxTries) {
		tries++;
		int x = rand() % width;
		int y = rand() % height;
		if (matrix[x][y] ==PATH && Utils::distance(Utils::getVector2fFromPoint(point), sf::Vector2f(x,y))>range) {
			return Point(x, y);
		}
	}
	return Point(-1, -1);
}



void MapGenerator::fillAVL() {

	int **visitedMatrix;
	visitedMatrix = new int*[width];
	for (size_t i = 0; i < width; i++)
	{
		visitedMatrix[i] = new int[height];
		for (size_t j = 0; j < height; j++)
		{
			visitedMatrix[i][j] = 0;
		}
	}


	int currentTree = -1;
	bool cellFound = false;
	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			if (!visitedMatrix[i][j]) {
				if (isPositionAPath(Point(i, j))) {
					currentTree++;
					roots.push_back(nodeptr());
					addAdjToSameTree(i, j, visitedMatrix, currentTree);
				}
			}
		}
	}
}


void MapGenerator::addAdjToSameTree(int x, int y, int **visitedMatrix, int currentTree) {
	if (!visitedMatrix[x][y]) {
		int cellValue = x%width + y * width;
		tree.insert(cellValue, roots[currentTree]);
		visitedMatrix[x][y] = 1;
		for (Point var : getNeighbours(Point(x, y)))
		{
			addAdjToSameTree(var.x, var.y, visitedMatrix, currentTree);
		}
	}
}
std::vector<Point> MapGenerator::getNeighbours(Point point) {
	std::vector<Point> tmpNeighbours;
	std::vector<Point> neighbours;

	tmpNeighbours.push_back(Point(0, -1));
	tmpNeighbours.push_back(Point(-1, 0));
	tmpNeighbours.push_back(Point(1, 0));
	tmpNeighbours.push_back(Point(0, 1));
	for (size_t i = 0; i < tmpNeighbours.size(); i++)
	{
		int checkX = point.x + tmpNeighbours[i].x;
		int checkY = point.y + tmpNeighbours[i].y;

		if (isPositionAPath(Point(checkX, checkY))) {
			neighbours.push_back(Point(checkX, checkY));
		}
	}
	return neighbours;
}

void MapGenerator::removeDiagonal()
{
	int i;
	int j;
	for (j = 1; j < height-1; j++) {
		for (i = 1; i < width-1; i++) {
			if (matrix[i][j] == 1) {
				if (matrix[i - 1][j - 1] == 1 && matrix[i - 1][j] == 0 && matrix[i][j - 1] == 0) {
					matrix[i][j] = 0;
				}
				else if (matrix[i + 1][j - 1] == 1 && matrix[i + 1][j] == 0 && matrix[i][j - 1] == 0) {
					matrix[i][j] = 0;

				}
				else if (matrix[i + 1][j + 1] == 1 && matrix[i + 1][j] == 0 && matrix[i][j + 1] == 0){
					matrix[i][j] = 0;

				}
				else if (matrix[i - 1][j + 1] == 1 && matrix[i - 1][j] == 0 && matrix[i][j + 1] == 0) {
					matrix[i][j] = 0;
				}
			}
		}
	}
}

void MapGenerator::optimizeRendering()
{
	for (int i = 1; i < width-1; i++) {
		for (int j = 1; j < height-1; j++) {
			if (matrix[i][j] == WALL) {
				if ((matrix[i - 1][j] == WALL || matrix[i - 1][j] == CONCRETE) &&
					(matrix[i + 1][j] == WALL || matrix[i + 1][j] == CONCRETE) &&
					(matrix[i][j + 1] == WALL || matrix[i][j + 1] == CONCRETE) &&
					(matrix[i][j - 1] == WALL || matrix[i][j - 1] == CONCRETE)) {
					matrix[i][j] = CONCRETE;
				}

			}
		}
	}
}

bool MapGenerator::isPositionAPath(Point p) {
	if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height) {
		return false;
	}
	if (matrix[p.x][p.y] == Cell_Type::PATH) {
		return true;
	}
	return false;
}
Point MapGenerator::getFirstPathCellTopLeft() {
	Point p(0, 0);
	int i = 0;
	while (matrix[p.x][p.y]==WALL || matrix[p.x][p.y] == CONCRETE) {
		if (i % 2 == 0) {
			p.x += 1;
		}
		else {
			p.y += 1;
		}
		if (p.x >= width || p.y >= height) {
			return Point(-1, -1);
		}
		i++;
	}
	return p;

}



MapGenerator::~MapGenerator()
{
	for (size_t i = 0; i < width; i++)
	{
		delete matrix[i];
	}
	delete matrix;
}

int** MapGenerator::openMatrix(int** matrix, int multiplier) {
	int** newMatrix;
	newMatrix = new int*[this->width];
	for (size_t i = 0; i < this->width; i++)
	{
		newMatrix[i] = new int[this->height];
		for (size_t j = 0; j < this->height ;  j++)
		{
			int test = 0;
			newMatrix[i][j] = matrix[i][j];
			if (matrix[i][j] == 0) {
				if (i != 0) {
					if (matrix[i - 1][j] == 0) {
						test++;
					}
				}
				else {
					test++;
				}
				if (i != this->width - 1) {
					if (matrix[i + 1][j] == 0) {
						test++;
					}
				}
				else {
					test++;
				}
				if (j != 0) {
					if (matrix[i][j - 1] == 0) {
						test++;
					}
				}
				else {
					test++;
				}
				if (j != this->height - 1) {
					if (matrix[i][j + 1] == 0) {
						test++;
					}
				}
				else {
					test++;
				}
				if (test<multiplier) {
					newMatrix[i][j] = 1;
				}
			}

		}
	}
	return newMatrix;
}

//augmente le nombre de 0;
int** MapGenerator::closeMatrix(int** matrix, int multiplier) {
	int** newMatrix;
	newMatrix = new int*[width];
	for (size_t i = 0; i < width; i++)
	{
		newMatrix[i] = new int[height];
		for (size_t j = 0; j < height; j++)
		{
			int test = 0;
			newMatrix[i][j] = matrix[i][j];
			if (matrix[i][j] >0) {
				if (i != 0) {
					if (matrix[i - 1][j] >0) {
						test++;
					}
				}
				else {
					test++;
				}
				if (i != width - 1) {
					if (matrix[i + 1][j] >0) {
						test++;
					}
				}
				else {
					test++;
				}
				if (j != 0) {
					if (matrix[i][j - 1] >0) {
						test++;
					}
				}
				else {
					test++;
				}
				if (j != height - 1) {
					if (matrix[i][j + 1] >0) {
						test++;
					}
				}
				else {
					test++;
				}
				if (test<multiplier) {
					newMatrix[i][j] = 0;
				}
			}

		}
	}
	return newMatrix;
}

Point MapGenerator::getWorldPosition(Point mapPosition) {
	return Point((mapPosition.x*blocWidth) + blocWidth / 2, (mapPosition.y*blocHeight) + blocHeight / 2);
}

Point MapGenerator::getMapPosition(Point worldPosition) {
	return Point((worldPosition.x/blocWidth), (worldPosition.y/blocHeight));
}
