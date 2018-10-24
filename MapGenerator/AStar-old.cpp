#include "AStar.h"
#include <algorithm>

/*
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
			if (map[p1.x][p1.y] != 0) {
				return true;
			}
		}
	}
	return false;
}

int AStar::getDistance(Point pointA, Point pointB) {
	int dstX = std::abs(pointA.x - pointA.x);
	int dstY = std::abs(pointA.y - pointA.y);

	if (dstX > dstY)
		return 14 * dstY + 10 * (dstX - dstY);
	return 14 * dstX + 10 * (dstY - dstX);
}


std::vector<Point> AStar::pathFinding(int** map, int width, int height) {

	std::vector<Point> points;
	if (start.x < width && start.y < height && end.x < width && end.y < height) {
		std::vector<Node> openList;
		std::vector<Node> closeList;
		Node* root = new Node();
		Point neighbors[8];
		Node* current = root;
		root->coord = start;
		root->gCost = 0;
		openList.push_back(*current);
		bool found = false;

		while (!openList.empty()) {

			if (current->coord == end) {
				found = true;
				break;
			}
			for (int i = 0; i < openList.size(); i++) {
				if (openList[i] == *current) {
					openList.erase(openList.begin() + i);
				}
			}

			closeList.push_back(*current);
			//get neighbors points
			neighbors[0] = Point(current->coord.x - 1, current->coord.y - 1);
			neighbors[1] = Point(current->coord.x, current->coord.y - 1);
			neighbors[2] = Point(current->coord.x + 1, current->coord.y - 1);
			neighbors[3] = Point(current->coord.x - 1, current->coord.y);
			neighbors[4] = Point(current->coord.x + 1, current->coord.y);
			neighbors[5] = Point(current->coord.x - 1, current->coord.y + 1);
			neighbors[6] = Point(current->coord.x, current->coord.y + 1);
			neighbors[7] = Point(current->coord.x + 1, current->coord.y + 1);


			int i = 0;
			for (i = 0; i < 8; i++) {
				bool addNode = true;
				if (pointInBoundsAndNotWall(neighbors[i], map, width, height)) {

					for (int k = 0; k < closeList.size(); k++) {
						if (closeList[k].coord == neighbors[i]) {
							addNode = false;
							break;
						}
					}
					if (addNode) {
						int newCostToNeighbor = current->gCost + getDistance(current->coord, neighbors[i]);
						for (int j = 0; j < openList.size(); j++) {
							if (openList[j].coord == neighbors[i]) {
								if (openList[j].gCost < newCostToNeighbor) {
									openList[j].gCost = newCostToNeighbor;
									openList[j].hCost = getDistance(openList[j].coord, end);
									openList[j].F = openList[j].hCost + openList[j].gCost;
									openList[j].parent = current;
								}
								addNode = false;
								break;
							}
						}
						if (addNode) {
							Node n;
							n.coord = neighbors[i];
							n.parent = current;
							n.hCost = getDistance(neighbors[i], end); //(std::abs(end.x - neighbors[i].x) + std::abs(end.y - neighbors[i].y)) * 10;
							/*if (neighbors[i].x == current->coord.x || neighbors[i].y == current->coord.y) {
								n.cost = current->cost + 10;
							}
							else {
								n.cost = current->cost + 14;
							}
							n.gCost = newCostToNeighbor;
							n.F = n.hCost + n.gCost;

							openList.push_back(n);
						}
					}
				}
			}
			int minCost = INT_MAX;
			for (int i = 0; i < openList.size(); i++) {
				if (openList[i].F < minCost) {
					current = new Node(openList[i]);
					minCost = current->F;
				}


			}
		}
		
		while (current->parent) {
			points.push_back(current->coord);
			current = current->parent;
		}
		points.push_back(current->coord);
		std::reverse(points.begin(), points.end());
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
*/