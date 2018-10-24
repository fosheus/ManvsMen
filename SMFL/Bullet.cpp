#include "Bullet.h"
#include "Utils.h"
#include <math.h>

Bullet::Bullet(float x, float y,float angle,int damages,int range)  {
	
	setPosition(x, y);
	setRotation(angle);
	this->speed = SPEED;
	this->damages = damages;
	this->range = range;
	lifeSpan = range;

}



void Bullet::move(float multiplier) {

	if (lifeSpan > 0) {
		float xAdded, yAdded;
		xAdded = cos(Utils::degrees_to_radians(-getRotation()))*speed*multiplier;
		yAdded = sin(Utils::degrees_to_radians(-getRotation()))*speed*multiplier;
		sf::Vector2f pos = getPosition();
		setPosition(pos.x + xAdded, pos.y + yAdded);
		lifeSpan -= std::sqrt(pow(xAdded, 2) + pow(yAdded, 2));
	}

}



Bullet::~Bullet()
{
}
