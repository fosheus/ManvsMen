#pragma once
#include <SFML\Graphics.hpp>
#include "Character.h"

class Collider
{
public:
	Collider();
	bool checkCollision(Collider collider);
	sf::RectangleShape getMinBounds() { return body->getMinBounds(); }
	~Collider();


private :
	Character* body;
};

