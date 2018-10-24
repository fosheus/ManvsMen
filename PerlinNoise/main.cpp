#include "stdlib.h"
#include <SFML/Graphics.hpp>
#include "PerlinNoise.h"

#define WIN_SIZE 800.0f
#define ARRAY_SIZE 512.0f
#define MAX_OCTALES 10
int nOutputWidth = ARRAY_SIZE;
int nOutputHeight = ARRAY_SIZE;
float *fNoiseSeed2D = nullptr;
float *fPerlinNoise2D = nullptr;

int nOctaveCount = 1;
float fScalingBias = 2.0f;
int nMode = 1;

int main() {

	if (ARRAY_SIZE < pow(2, MAX_OCTALES - 1)) {
		printf("array size must be >= 2^(MAX_OCTALES-1)");
		exit(1);
	}

	nOutputWidth = ARRAY_SIZE;
	nOutputHeight = ARRAY_SIZE;

	fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
	fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];
	for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;


	sf::RenderWindow window(sf::VideoMode(WIN_SIZE, WIN_SIZE), "SFML works!");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();


			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					nOctaveCount++;
				}
				if (event.key.code == sf::Keyboard::LShift) {
					nOctaveCount--;
				}
				if (event.key.code == sf::Keyboard::A) {
					fScalingBias += 0.2f;
				}
				if (event.key.code == sf::Keyboard::Q) {
					fScalingBias -= 0.2f;
				}
				if (event.key.code == sf::Keyboard::Z) {
					for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
				}
			}


		}
		if (fScalingBias < 0.2f)
			fScalingBias = 0.2f;

		if (nOctaveCount == MAX_OCTALES+1)
			nOctaveCount = 1;
		if (nOctaveCount == 0) {
			nOctaveCount = MAX_OCTALES;
		}
		PerlinNoise::PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScalingBias, fPerlinNoise2D);


		window.clear();
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(WIN_SIZE/ARRAY_SIZE, WIN_SIZE / ARRAY_SIZE));
		for (int x = 0; x < nOutputWidth; x++)
		{
			for (int y = 0; y < nOutputHeight; y++)
			{
				rect.setPosition(x*rect.getSize().x, y*rect.getSize().y);
				short bg_col, fg_col;
				wchar_t sym;
				int pixel_bw = (int)(fPerlinNoise2D[y * nOutputWidth + x] * 12.0f);
				switch (pixel_bw)
				{
				case 0:rect.setFillColor(sf::Color(0, 0, 80));  break;

				case 1:rect.setFillColor(sf::Color(0, 0, 120));  break;
				case 2: rect.setFillColor(sf::Color(0, 0, 160));  break;
				case 3: rect.setFillColor(sf::Color(0, 0, 210));  break;
				case 4: rect.setFillColor(sf::Color(0, 0, 255));  break;

				case 5: rect.setFillColor(sf::Color(0, 60, 180));  break;
				case 6: rect.setFillColor(sf::Color(0, 120, 120));  break;
				case 7: rect.setFillColor(sf::Color(0, 180, 60));  break;
				case 8: rect.setFillColor(sf::Color(0, 255, 0));  break;

				case 9:  rect.setFillColor(sf::Color(60, 180, 0)); break;
				case 10: rect.setFillColor(sf::Color(80, 150, 0));  break;
				case 11: rect.setFillColor(sf::Color(100, 120, 10));  break;
				case 12: rect.setFillColor(sf::Color(30, 20, 5));  break;
				
				default :
					rect.setFillColor(sf::Color::Black);
					break;
				}

				window.draw(rect);
			}
		}
		window.display();

	}

}