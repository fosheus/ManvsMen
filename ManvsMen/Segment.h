#pragma once
#include <SFML/Graphics.hpp>
class Segment
{
public:
	Segment();
	Segment(sf::Vector2f a, sf::Vector2f b) : a(a),b(b){}
	~Segment();
	sf::Vector2f a;
	sf::Vector2f b;
	
};

