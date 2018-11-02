#pragma once
#include "SFML\Graphics.hpp"
#include "Game.h"
#include "DEFINITIONS.h"

class TileMap : public sf::Drawable,public sf::Transformable
{
private:
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	GameDataRef _data;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	TileMap(GameDataRef data);
	~TileMap();
	bool load(sf::Vector2u tileSize, int ** matrix, unsigned int width, unsigned int height);
};

