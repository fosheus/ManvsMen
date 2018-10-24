#include "Utils.h"
#include <math.h>

#include <sstream>
#include <iomanip>
#include <iostream>

#define M_PI 3.14159265358979323846

float Utils::degrees_to_radians(float angle) {
	return (float)(angle*  M_PI / 180);
}

float Utils::radians_to_degrees(float angle) {
	return (float)(angle * 180 / M_PI);
}

std::string Utils::floatToStringWPrecision(float number, int precision) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision) << number;
	return stream.str();
}

int Utils::round_int(double r) {
	return (r > 0.0f) ? ((int)(r + 0.5f)) : ((int)(r - 0.5f));
}

float Utils::distance(sf::Vector2f point1, sf::Vector2f point2) {
	return sqrt(pow(point1.x-point2.x,2)+pow(point1.y-point2.y,2));
}

bool Utils::rotatedCollisionDetection(sf::RectangleShape a, sf::RectangleShape b)
{
	sf::RectangleShape rects[] = { a,b };
	for (size_t i = 0; i < 2; i++)
	{
		for (int i1 = 0; i1 < (int)rects[i].getPointCount(); i1++)
		{
			int i2 = (i1 + 1) % rects[i].getPointCount();
			sf::Vector2f* iPoints = getPoints(rects[i]);
			sf::Vector2f p1 = iPoints[i1];
			sf::Vector2f p2 = iPoints[i2];

			sf::Vector2f normal = sf::Vector2f(p2.y - p1.y, p1.x - p2.x);

			double minA = LONG_MAX, maxA = LONG_MIN;
			sf::Vector2f* aPoints = getPoints(a);
			for (size_t j = 0; j < a.getPointCount(); j++)
			{
				sf::Vector2f p = aPoints[j];
				double projected = normal.x * p.x + normal.y * p.y;
				if (projected < minA)
					minA = projected;
				if (projected > maxA)
					maxA = projected;
			}



			double minB = LONG_MAX, maxB = LONG_MIN;
			sf::Vector2f* bPoints = getPoints(b);
			for (size_t j = 0; j < b.getPointCount(); j++)
			{
				sf::Vector2f p = bPoints[j];
				double projected = normal.x * p.x + normal.y * p.y;
				if (projected < minB)
					minB = projected;
				if (projected > maxB)
					maxB = projected;
			}
			delete[] iPoints;
			delete[] aPoints;
			delete[] bPoints;

			if (maxA < minB || maxB < minA)
				return false;
		}
	}
	return true;
}

bool Utils::circleCollisionDetection(sf::Vector2f pos1,sf::Vector2f pos2,float radius1,float radius2) {

	float distance = Utils::distance(pos1, pos2);
	if (distance <= radius1 + radius2) {
		return true;
	}
	return false;

}

sf::Vector2f* Utils::getPoints(sf::RectangleShape rect) {

	sf::Vector2f p1;
	sf::Vector2f p2;
	sf::Vector2f p3;
	sf::Vector2f p4;

	float angleRadian = Utils::degrees_to_radians(-rect.getRotation());
	float angleRadianm90 = Utils::degrees_to_radians(-rect.getRotation() -90);

	p1.x = rect.getPosition().x;
	p1.y = rect.getPosition().y;

	p2.x = p1.x + rect.getSize().x*cos(angleRadian);
	p2.y = p1.y - rect.getSize().x*sin(angleRadian);

	p3.x = p2.x + rect.getSize().y*cos(angleRadianm90);
	p3.y = p2.y - rect.getSize().y*sin(angleRadianm90);

	p4.x = p1.x + rect.getSize().y*cos(angleRadianm90);
	p4.y = p1.y - rect.getSize().y*sin(angleRadianm90);

	return new sf::Vector2f[4]{p1, p2, p3, p4};
}


sf::Vector2f Utils::getGeneralCoordinateFromLocalPoint(sf::Vector2f localOffset, sf::Vector2f generalOrigin, float angle) {
	float x, y;
	

	x = generalOrigin.x + localOffset.x*cos(Utils::degrees_to_radians(-angle));
	y = generalOrigin.y + localOffset.x*sin(Utils::degrees_to_radians(-angle));

	x = x + localOffset.y*cos(Utils::degrees_to_radians(-(angle - 90)));
	y = y + localOffset.y*sin(Utils::degrees_to_radians(-(angle - 90)));
	return sf::Vector2f(x, y);
}
sf::Vector2f Utils::getVector2fFromPoint(Point p) {
	return sf::Vector2f((float)p.x, (float)p.y);
}
Point Utils::getPointFromVector2f(sf::Vector2f v) {
	return Point((int)v.x, (int)v.y);

}

