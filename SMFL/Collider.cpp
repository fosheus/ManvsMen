#include "Collider.h"



Collider::Collider()
{
}

bool Collider::checkCollision(Collider collider)
{
	
	sf::RectangleShape rects[] = { this->getMinBounds() ,collider.getMinBounds() };
	for (size_t i = 0; i < 2; i++)
	{
		for (int i1 = 0; i1 < (int)rects[i].getPointCount(); i1++)
		{
			int i2 = (i1 + 1) % rects[i].getPointCount();
			sf::Vector2f* iPoints = Utils::getPoints(rects[i]);
			sf::Vector2f p1 = iPoints[i1];
			sf::Vector2f p2 = iPoints[i2];

			sf::Vector2f normal = sf::Vector2f(p2.y - p1.y, p1.x - p2.x);
			double minA = LONG_MAX, maxA = LONG_MIN;
			sf::Vector2f* aPoints = Utils::getPoints(rects[0]);
			for (size_t j = 0; j < rects[0].getPointCount(); j++)
			{
				sf::Vector2f p = aPoints[j];
				double projected = normal.x * p.x + normal.y * p.y;
				if (projected < minA)
					minA = projected;
				if (projected > maxA)
					maxA = projected;
			}



			double minB = LONG_MAX, maxB = LONG_MIN;
			sf::Vector2f* bPoints = Utils::getPoints(rects[1]);
			for (size_t j = 0; j < rects[1].getPointCount(); j++)
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


Collider::~Collider()
{
}
