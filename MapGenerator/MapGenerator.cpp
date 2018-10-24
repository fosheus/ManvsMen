#include "MapGenerator.h"



MapGenerator::MapGenerator(size_t width, size_t height, size_t blocWidth, size_t blocHeight)
{
	
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

		fillAVL();
		int val=0;
		int maxIndex=0;
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
		vert.setSize(sf::Vector2f((float)blocWidth, (float)blocHeight));
		vert.setFillColor(sf::Color::Green);
		bleu.setSize(sf::Vector2f((float)blocWidth, (float)blocHeight));
		bleu.setFillColor(sf::Color::Blue);
	}

}
Point MapGenerator::getRandomPathPoint() {
	bool pointCorrect = false;
	Point point;
	while (!pointCorrect) {
		int x = rand() % width;
		int y = rand() % height;
		if (matrix[x][y] > 0) {
			point.x = x;
			point.y = y;
			return point;
		}
	}
	return Point(-1, -1);
}

void MapGenerator::fillAVL() {

	int **visitedMatrix;
	visitedMatrix= new int*[width];
	for (size_t i = 0; i < width; i++)
	{
		visitedMatrix[i] = new int[height];
		for (size_t j = 0; j < height; j++)
		{
			visitedMatrix[i][j] = 0;
		}
	}

	
	int currentTree=-1;
	bool cellFound = false;
	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			if (!visitedMatrix[i][j]) {
				if (isSafeAndPath(i, j)) {
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
		for each (Point var in getNeighbours(Point(x, y)))
		{
			addAdjToSameTree(var.x, var.y, visitedMatrix, currentTree);
		}
	}
}
std::vector<Point> MapGenerator::getNeighbours(Point point) {
	std::vector<Point> tmpNeighbours;
	std::vector<Point> neighbours;

	tmpNeighbours.push_back(Point(0,-1));
	tmpNeighbours.push_back(Point(-1, 0));
	tmpNeighbours.push_back(Point(1, 0));
	tmpNeighbours.push_back(Point(0, 1));
	for (size_t i = 0; i < tmpNeighbours.size(); i++)
	{
		int checkX = point.x + tmpNeighbours[i].x;
		int checkY = point.y + tmpNeighbours[i].y;

		if (isSafeAndPath(checkX, checkY)) {
			neighbours.push_back(Point(checkX, checkY));
		}
	}
	return neighbours;
}


bool MapGenerator::isSafeAndPath(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return false;
	}
	if (matrix[x][y] == WALL) {
		return false;
	}
	return true;
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

void MapGenerator::draw(sf::RenderWindow &window) {
	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			if (matrix[i][j] == 0) {
				vert.setPosition(i * blocWidth, j * blocHeight);
				window.draw(vert);
			}
			else {
				bleu.setPosition(i * blocWidth, j * blocHeight);
				window.draw(bleu);
			}
		}
	}
}
