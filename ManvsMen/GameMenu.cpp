#include "GameMenu.h"


GameMenu::GameMenu(){
	pixelFont.loadFromFile("font/pixel.ttf");
	quitText.setFont(pixelFont);
	resumeText.setFont(pixelFont);
	restartText.setFont(pixelFont);
	quitText.setString("QUIT");
	resumeText.setString("RESUME");
	restartText.setString("RESTART");
	texts.push_back(resumeText);
	texts.push_back(restartText);
	texts.push_back(quitText);
	selectedIndex = 0;
	background.setTexture(this->_data->assets.GetImage("images/menuBackground.jpg"));
}

GameMenu::Items GameMenu::getSelectedItem() {
	return (GameMenu::Items)selectedIndex;
}

void GameMenu::SelectItemUp() {
	selectedIndex--;
	if (selectedIndex < 0) {
		selectedIndex = NB_ITEMS - 1;
	}
}
void GameMenu::SelectItemDown() {
	selectedIndex++;
	if (selectedIndex >=NB_ITEMS) {
		selectedIndex =0;
	}
}

void GameMenu::Draw(sf::RenderWindow& window ) {

	for (size_t i = 0; i < texts.size(); i++)
	{
		texts[i].setFillColor(sf::Color::Black);
	}
	texts[selectedIndex].setFillColor(sf::Color::Green);
	background.setPosition(window.getSize().x/2- background.getTexture()->getSize().x/2, window.getSize().y / 2 - background.getTexture()->getSize().y / 2);
	window.draw(background);
	sf::Vector2f backPos = background.getPosition();
	for (size_t i = 0; i < texts.size(); i++)
	{
		texts[i].setPosition(backPos.x+100,backPos.y+(50*(i+2)));
		window.draw(texts[i]);
	}
}


GameMenu::~GameMenu()
{
}
