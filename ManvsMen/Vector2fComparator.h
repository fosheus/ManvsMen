#pragma once
#include <SFML/Graphics.hpp>
class Vector2fComparator
{
public:
	Vector2fComparator();
	bool operator()(const sf::Vector2f& point1, const sf::Vector2f& point2) const {
		if (point1.x < point2.x) return true;
		if (point1.x > point2.x)return false;
		if (point1.y < point2.y)return true;
		return false;
	}
	~Vector2fComparator();
};

