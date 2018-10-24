#pragma once
#include "SFML\Graphics.hpp"

#define SPEED 60 

class Bullet :
	public sf::Transformable
{
private :
	float speed;
	int damages;
	int range;
	float lifeSpan;

public:
	Bullet(float x, float y, float angle, int damages,int range);

	int getDamages() { return damages; }
	void setDamages(int damages) { this->damages = damages; }
	bool isDead() { return lifeSpan<=0; }
	void move(float multiplier);
	~Bullet();

	
};
