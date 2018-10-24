#pragma once
#include "SFML\Graphics.hpp"
#include "Point.h"
#include <string>
class Utils
{
public:
	static float degrees_to_radians(float angle);
	static float radians_to_degrees(float angle);
	static std::string floatToStringWPrecision(float number, int precision);
	static float distance(sf::Vector2f point1, sf::Vector2f point2);
	static bool rotatedCollisionDetection(sf::RectangleShape rect1, sf::RectangleShape rect2);
	static bool circleCollisionDetection(sf::Vector2f pos1, sf::Vector2f pos2, float radius1, float radius2);
	static sf::Vector2f* getPoints(sf::RectangleShape rect);
	static sf::Vector2f getGeneralCoordinateFromLocalPoint(sf::Vector2f localOffset, sf::Vector2f generalOrigin, float angle);
	static int round_int(double r);
	static sf::Vector2f getVector2fFromPoint(Point p);
	static Point getPointFromVector2f(sf::Vector2f v);
};

