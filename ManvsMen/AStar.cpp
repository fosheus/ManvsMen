#include "AStar.h"
#include <algorithm>


AStar::AStar()
{
}

AStar::AStar(Point start, Point dest) {

	this->start = start;
	this->end = end;
}
bool operator==(const Point &p1, const Point &p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

bool operator<(const AStar::Node &n1, const AStar::Node &n2) {
	return n1.gCost < n2.gCost;
}
bool operator==(const AStar::Node &n1, const AStar::Node &n2) {
	return n1.coord == n2.coord;
}
bool pointInBoundsAndNotWall(Point p1, int** map, int width, int height) {
	if (p1.x >= 0 && p1.x < width) {
		if (p1.y >= 0 && p1.y < height) {
			if (map[p1.x][p1.y] == 1) {
				return true;
			}
		}
	}
	return false;
}

int AStar::getDistance(Point pointA, Point pointB) {
	int dstX = std::abs(pointA.x - pointB.x);
	int dstY = std::abs(pointA.y - pointB.y);

	if (dstX > dstY)
		return 14 * dstY + 10 * (dstX - dstY);
	return 14 * dstX + 10 * (dstY - dstX);
}
std::vector<AStar::Node> AStar::getNeighbours(Node node,int width, int height) {
	std::vector<Node> neighbours;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0)
				continue;

			int checkX = node.coord.x + x;
			int checkY = node.coord.y + y;

			if (checkX >= 0 && checkX < width && checkY >= 0 && checkY < height) {
				Node n;
				n.coord = Point(checkX, checkY);
				n.parent = NULL;
				neighbours.push_back(n);
			}
		}
	}

	return neighbours;
}

std::vector<Point> AStar::retracePath(AStar::Node node) {

	std::vector<Point> points;
	points.push_back(node.coord);
	Node* current = node.parent;

	while (current) {
		points.push_back(current->coord);
		current = current->parent;
	}
	//std::reverse(points.begin(), points.end());
	return points;
}


std::vector<Point> AStar::pathFinding(int** map, int width, int height) {

	std::vector<Point> points;
	if (!(start.x < width && start.y < height && end.x < width && end.y < height)) {
		return points;
	}
	std::vector<Node> openList;
	std::vector<Node> closeList;

	Node start;
	start.coord = this->start;
	start.gCost = 0;
	start.hCost = getDistance(this->start, end);
	start.parent = NULL;
	openList.push_back(start);
	bool found = false;
	Node node;

	while (!openList.empty()) {

		node = openList[0];
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (openList[i].getfCost() < node.getfCost() || openList[i].getfCost() == node.getfCost()) {
				if (openList[i].hCost < node.hCost) {
					node = openList[i];
				}
			}
		}

		if (node.coord == end) {
			points = retracePath(node);
			for (size_t i = 0; i < closeList.size(); i++)
			{
				delete closeList[i].parent;
			}
			for (size_t i = 0; i < openList.size(); i++)
			{
				delete openList[i].parent;
			}
			return points;
		}
		for (int i = 0; i < openList.size(); i++) {
			if (openList[i] == node) {
				openList.erase(openList.begin() + i);
			}
		}

		closeList.push_back(node);
		//get neighbours points
		std::vector<Node>neighbours = getNeighbours(node, width, height);

		int i = 0;
		for (i = 0; i < neighbours.size(); i++) {
			bool addNode = true;
			if (!pointInBoundsAndNotWall(neighbours[i].coord, map, width, height)) {
				continue;
			}

			for (int k = 0; k < closeList.size(); k++) {
				if (closeList[k] == neighbours[i]) {
					addNode = false;
					break;
				}
			}
			if (!addNode) {
				continue;
			}
			int newCostToNeighbor = node.gCost + getDistance(node.coord, neighbours[i].coord);
			int index = -1;
			for (int j = 0; j < openList.size(); j++) {
				if (openList[j] == neighbours[i]) {
					index = j;
					break;
				}
			}
			if (index != -1 && newCostToNeighbor < openList[index].gCost) {
				openList[index].gCost = newCostToNeighbor;
				openList[index].hCost = getDistance(openList[index].coord, end);
				delete openList[index].parent;
				openList[index].parent = new Node(node);
			}
			else if (index == -1) {
				Node n;
				n.coord = neighbours[i].coord;
				n.gCost = newCostToNeighbor;
				n.hCost = getDistance(neighbours[i].coord, end);
				n.parent = new Node(node);
				openList.push_back(n);
			}

		}
	}

	for (size_t i = 0; i < closeList.size(); i++)
	{
		delete closeList[i].parent;
	}
	return points;
}




void AStar::setStartPoint(Point start) {
	this->start = start;
}

Point AStar::getStartPoint() {
	return start;
}

void AStar::setDestPoint(Point end) {
	this->end = end;
}

Point AStar::getDestPoint() {
	return end;
}

AStar::~AStar()
{
}
