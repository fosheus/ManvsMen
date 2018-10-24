#include <SFML/Graphics.hpp>
#include "MapGenerator.h"
#include "AStar.h"
#include <iostream>

#define MAX_X 400
#define MAX_Y 400
#define BLOCSIZE 10

int main(int argc, char** argv)
{
	int max;
	int blocsize;
	srand((unsigned int)time(NULL));
	if (argc ==3) {
		max = atoi(argv[1]);
		blocsize = atoi(argv[2]);
		std::cout << "oui" << std::endl;
	}
	else {
		max = MAX_X;
		blocsize = BLOCSIZE;
	}
	sf::RenderWindow window(sf::VideoMode(max, max), "SFML works!");
	
	MapGenerator* map= new MapGenerator(max/blocsize,max/blocsize, blocsize, blocsize);
	AStar astar;
	astar.setStartPoint(map->getRandomPathPoint());
	astar.setDestPoint(map->getRandomPathPoint());
	sf::RectangleShape vert;
	sf::RectangleShape bleu;
	sf::RectangleShape rouge;
	sf::RectangleShape jaune;
	vert.setSize(sf::Vector2f((float)blocsize,(float) blocsize));
	vert.setFillColor(sf::Color::Green);
	bleu.setSize(sf::Vector2f((float)blocsize, (float)blocsize));
	bleu.setFillColor(sf::Color::Blue);
	rouge.setSize(sf::Vector2f((float)blocsize, (float)blocsize));
	rouge.setFillColor(sf::Color::Red);
	jaune.setSize(sf::Vector2f((float)blocsize,(float) blocsize));
	jaune.setFillColor(sf::Color::Yellow);

	astar.find(map->getMatrix(), map->getWidth(), map->getHeight());

	sf::Clock clock;
	bool holdSpace = false;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (holdSpace == false) {
				clock.restart();
				
				astar.setStartPoint(map->getRandomPathPoint());
				astar.setDestPoint(map->getRandomPathPoint());
				astar.find(map->getMatrix(), map->getWidth(), map->getHeight());
				holdSpace = true;
				std::cout << clock.getElapsedTime().asSeconds() << std::endl;
			}
		}
		else {
			holdSpace = false;
		}
		

		window.clear();
		map->draw(window);
		if (astar.getPathFound()) {
			std::vector<Point> points = astar.getPath();
			for (size_t i = 0; i < points.size(); i++)
			{
				rouge.setPosition((float)points[i].x*blocsize,(float) points[i].y*blocsize);
				window.draw(rouge);
			}
		}
		else {
			std::vector<AStar::Node> *openList = astar.getOpenList();
			std::vector<AStar::Node> *closeList = astar.getCloseList();
			for (size_t i = 0; i < openList->size(); i++)
			{
				jaune.setPosition((float)(*openList)[i].coord.x*blocsize,(float) (*openList)[i].coord.y*blocsize);
				window.draw(jaune);
			}
			for (size_t i = 0; i < closeList->size(); i++)
			{
				rouge.setPosition((float)(*closeList)[i].coord.x*blocsize, (float)(*closeList)[i].coord.y*blocsize);
				window.draw(rouge);
			}

		}
		

		window.display();
	}

	return 0;
}

