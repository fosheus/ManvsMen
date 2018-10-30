#include "TileMap.h"


TileMap::TileMap(GameDataRef data) :_data(data)
{
}

TileMap::~TileMap()
{
}

bool TileMap::load(sf::Vector2u tileSize, int ** matrix, unsigned int width, unsigned int height,sf::Vector2f referencePos)
{
	m_tileset = this->_data->assets.GetImage("images/tileSetMap.png");
	
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width*height * 4);

	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			int tileNumber = matrix[i][j];
			int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
			int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

			sf::Vertex* quad = &m_vertices[(i + j*width) * 4];

			quad[0].position = sf::Vector2f((float)i*tileSize.x, (float)j*tileSize.y);
			quad[1].position = sf::Vector2f((float)(i + 1)*tileSize.x, (float)j*tileSize.y);
			quad[2].position = sf::Vector2f((float)(i + 1)*tileSize.x, (float)(j + 1)*tileSize.y);
			quad[3].position = sf::Vector2f((float)i*tileSize.x, (float)(j + 1)*tileSize.y);

			quad[0].texCoords = sf::Vector2f((float)tu*tileSize.x, (float)tv*tileSize.y);
			quad[1].texCoords = sf::Vector2f((float)(tu + 1)*tileSize.x, (float)tv*tileSize.y);
			quad[2].texCoords = sf::Vector2f((float)(tu + 1)*tileSize.x, (float)(tv + 1)*tileSize.y);
			quad[3].texCoords = sf::Vector2f((float)tu*tileSize.x, (float)(tv + 1)*tileSize.y);

			quad[0].color = sf::Color(64, 64, 64);
			quad[1].color = sf::Color(64, 64, 64);
			quad[2].color = sf::Color(64, 64, 64);
			quad[3].color = sf::Color(64, 64, 64);

			for (int i = 0; i < 4; i++) {
				if (Utils::distance(referencePos, quad[i].position) < PLAYERVIEWFIELDRADIUS / 2.0f) {
					//ray casting
					sf::Vector2f point = referencePos;
					float deltaX = referencePos.x - quad[i].position.x;
					float deltaY = referencePos.y - quad[i].position.y;
					float speed = 1;
					float angle = atan2(deltaY, deltaX);
					while (this->_data->map.isEntityInMapBounds(point)) {
						if (Utils::distance(point, quad[i].position) < speed) {
							quad[i].color = sf::Color::White;
							break;
						}
						point.x = point.x - cos(angle)*speed;
						point.y = point.y - sin(angle)*speed;
						if (!this->_data->map.isEntityOnPathCell(point)) {
							quad[i].color = sf::Color(64, 64, 64);
							break;
						}
					}
				}
				else {
					quad[i].color = sf::Color(64, 64, 64);
				}
			}

			

		}
		
	}
	return true;
}

void TileMap:: draw(sf::RenderTarget& target, sf::RenderStates states) const {

	states.transform *= getTransform();

	states.texture = &m_tileset;

	target.draw(m_vertices, states);
}
