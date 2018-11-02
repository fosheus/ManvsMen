#pragma once
#include <SFML/Graphics.hpp>
class IntersectionPoint
{
public:
	IntersectionPoint();
	IntersectionPoint(float x, float y, float coeff): x(x),y(y),coeff(coeff) {}
	~IntersectionPoint();
	float x;
	float y;
	float coeff;
};

