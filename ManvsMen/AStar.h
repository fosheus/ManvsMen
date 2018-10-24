#pragma once
#include <vector>
#include "Point.h"

class AStar
{

public :
	struct Node {
		Point coord;
		int hCost;
		int gCost;
		Node* parent=NULL;
		int getfCost() {
			return hCost + gCost;
		}
	};
private :
	
	Node* root;
	Point start;
	Point end;


	int getDistance(Point pointA, Point pointB);
	std::vector<AStar::Node> getNeighbours(Node node, int width, int height);
	std::vector<Point> retracePath(Node node);
	
	

public :
	AStar();
	AStar(Point start, Point dest);
	void setStartPoint(Point start);
	Point getStartPoint();
	void setDestPoint(Point end);
	Point getDestPoint();

	std::vector<Point> pathFinding(int **map, int width, int height);
	~AStar();

	
};

