#include "SelectWeaponState.h"
#include "DEFINITIONS.h"
#include "GameState.h"
#include "MainMenuState.h"

#define MAX_SIZE_X 200
#define MAX_SIZE_Y 30

SelectWeaponState::SelectWeaponState(GameDataRef data) : _data(data)
{
}

SelectWeaponState::~SelectWeaponState()
{
	
}

void SelectWeaponState::Init()
{
	float windowX = SCREEN_WIDTH;
	float windowY = SCREEN_HEIGHT;
	float characterX;
	float characterY;
	float currentX;
	float currentY;

	selectedWeaponIndex = 0;
	animation = new Animation(&this->_data->assets.GetImage(CHARACTER_SPRITESHEET), 
		sf::Vector2u(CHARACTER_SPRITESHEET_COLUMN, CHARACTER_SPRITESHEET_ROW), CHARACTER_SPRITESHEET_SWITCH_TIME);
	_background.setTexture(_data->assets.GetImage(SELECT_WEAPON_BACKGROUND));
	_selectRight.setTexture(_data->assets.GetImage(SELECT_WEAPON_RIGHT_ARROW));
	_selectLeft.setTexture(_data->assets.GetImage(SELECT_WEAPON_LEFT_ARROW));
	_returnButton.setTexture(_data->assets.GetImage(SELECT_WEAPON_RETURN_BUTTON));
	_playButton.setTexture(_data->assets.GetImage(SELECT_WEAPON_PLAY_BUTTON));

	animation->update(0, 0);
	this->_selectedWeapon.setTexture(this->_data->assets.GetImage(CHARACTER_SPRITESHEET));
	this->_selectedWeapon.setTextureRect(this->animation->uvRect);

	characterX = _selectedWeapon.getTexture()->getSize().x/CHARACTER_SPRITESHEET_COLUMN;
	characterY = _selectedWeapon.getTexture()->getSize().y/CHARACTER_SPRITESHEET_ROW;
	currentX = _selectLeft.getTexture()->getSize().x;
	currentY = _selectLeft.getTexture()->getSize().y;

	_selectLeft.setPosition(windowX / 2 - currentX-characterX/2 - 50,windowY / 4 - (currentY/2));

	currentX = _selectRight.getTexture()->getSize().x;
	currentY = _selectRight.getTexture()->getSize().y;
	_selectRight.setPosition(windowX / 2 +characterX/2 + 50, windowY / 4 - (currentY / 2));
	
	_selectedWeapon.setPosition(windowX/2-characterX/2,windowY/4-characterY/2);

	currentX = _playButton.getTexture()->getSize().x;
	currentY = _playButton.getTexture()->getSize().y;
	_playButton.setPosition(windowX-currentX-50, windowY-currentY-20);

	currentX = _returnButton.getTexture()->getSize().x;
	currentY = _returnButton.getTexture()->getSize().y;
	_returnButton.setPosition(50,windowY-currentY-20);

	_weaponDamage.setPosition(windowX / 2 - MAX_SIZE_X / 2, windowY * 5 / 12);
	_weaponDamageMax.setPosition(_weaponDamage.getPosition());
	_weaponDamage.setFillColor(sf::Color::Red);
	_weaponDamageMax.setFillColor(sf::Color::Transparent);
	_weaponDamageMax.setOutlineColor(sf::Color::Red);
	_weaponDamageMax.setOutlineThickness(5);
	_weaponDamageMax.setSize(sf::Vector2f(MAX_SIZE_X,MAX_SIZE_Y));

	_weaponRange.setPosition(windowX / 2 - MAX_SIZE_X / 2, windowY * 6 / 12);
	_weaponRangeMax.setPosition(_weaponRange.getPosition());
	_weaponRange.setFillColor(sf::Color::Blue);
	_weaponRangeMax.setFillColor(sf::Color::Transparent);
	_weaponRangeMax.setOutlineColor(sf::Color::Blue);
	_weaponRangeMax.setOutlineThickness(5);
	_weaponRangeMax.setSize(sf::Vector2f(MAX_SIZE_X, MAX_SIZE_Y));

	_weaponShootSpeed.setPosition(windowX / 2 - MAX_SIZE_X / 2, windowY * 7 / 12);
	_weaponShootSpeedMax.setPosition(_weaponShootSpeed.getPosition());
	_weaponShootSpeed.setFillColor(sf::Color::Yellow);
	_weaponShootSpeedMax.setFillColor(sf::Color::Transparent);
	_weaponShootSpeedMax.setOutlineColor(sf::Color::Yellow);
	_weaponShootSpeedMax.setOutlineThickness(5);
	_weaponShootSpeedMax.setSize(sf::Vector2f(MAX_SIZE_X, MAX_SIZE_Y));


	damage.setFont(_data->assets.GetFont(ARIAL_FONT));
	damage.setPosition(sf::Vector2f(_weaponDamage.getPosition().x - 200, _weaponDamage.getPosition().y));
	damage.setString("Damages");
	damage.setFillColor(sf::Color::White);
	range.setPosition(sf::Vector2f(_weaponRange.getPosition().x - 200, _weaponRange.getPosition().y));
	range.setFont(_data->assets.GetFont(ARIAL_FONT));
	range.setString("Range");
	range.setFillColor(sf::Color::White);
	shoot.setPosition(sf::Vector2f(_weaponShootSpeed.getPosition().x - 200, _weaponShootSpeed.getPosition().y));
	shoot.setFont(_data->assets.GetFont(ARIAL_FONT));
	shoot.setString("Rearm time");
	shoot.setFillColor(sf::Color::White);
	magazine.setPosition(sf::Vector2f(shoot.getPosition().x, windowY * 8 / 12));
	magazine.setFont(_data->assets.GetFont(ARIAL_FONT));
	magazine.setString("Magazine : ");
	magazine.setFillColor(sf::Color::White);

	weaponName.setFont(_data->assets.GetFont(ARIAL_FONT));
	weaponName.setFillColor(sf::Color::White);




	this->_background.setPosition(this->_data->window.getSize().x / 2 - this->_background.getGlobalBounds().width / 2, this->_data->window.getSize().y / 2 - this->_background.getGlobalBounds().height / 2);
}

void SelectWeaponState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			this->_data->window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed || event.type==sf::Event::KeyPressed) {

			if (this->_data->inputs.IsSpriteClicked(_selectLeft, sf::Mouse::Left, _data->window) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				selectedWeaponIndex = (selectedWeaponIndex - 1);
				if (selectedWeaponIndex < 0) {
					selectedWeaponIndex = CHARACTER_SPRITESHEET_ROW - 1;
				}
			}
			if (this->_data->inputs.IsSpriteClicked(_selectRight, sf::Mouse::Left, _data->window) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				selectedWeaponIndex = (selectedWeaponIndex + 1) % CHARACTER_SPRITESHEET_ROW;
			}
			if (this->_data->inputs.IsSpriteClicked(_returnButton, sf::Mouse::Left, _data->window) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
				this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)));
			}
			if (this->_data->inputs.IsSpriteClicked(_playButton, sf::Mouse::Left, _data->window) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				this->_data->machine.AddState(StateRef(new GameState(this->_data, weaponTypeFromSelectedIndex(selectedWeaponIndex))));
			}
		}
	}
}

void SelectWeaponState::Update(float dt)
{
	float finalmultiplier = dt*multiplier;
	animation->update(selectedWeaponIndex, finalmultiplier);
	_selectedWeapon.setTextureRect(animation->uvRect);
	std::string s;
	

	switch (selectedWeaponIndex) {
	case 0:
		_weaponDamage.setSize(sf::Vector2f(MAX_SIZE_X*GUN_DAMAGES/WEAPON_MAX_DAMAGES, MAX_SIZE_Y));
		_weaponRange.setSize(sf::Vector2f(MAX_SIZE_X*GUN_RANGE / WEAPON_MAX_RANGE, MAX_SIZE_Y));
		_weaponShootSpeed.setSize(sf::Vector2f(MAX_SIZE_X*GUN_REARMTIME / WEAPON_MAX_REARMTIME, MAX_SIZE_Y));
		magazine.setString("Magazine : \t" + to_string(GUN_MAGAZINE));
		s = "Gun";
		break;
	case 1:
		_weaponDamage.setSize(sf::Vector2f(MAX_SIZE_X*RIFLE_DAMAGES / WEAPON_MAX_DAMAGES, MAX_SIZE_Y));
		_weaponRange.setSize(sf::Vector2f(MAX_SIZE_X*RIFLE_RANGE / WEAPON_MAX_RANGE, MAX_SIZE_Y));
		_weaponShootSpeed.setSize(sf::Vector2f(MAX_SIZE_X*RIFLE_REARMTIME / WEAPON_MAX_REARMTIME, MAX_SIZE_Y));
		magazine.setString("Magazine : \t" + to_string(RIFLE_MAGAZINE));
		s = "Rifle";
		break;
	case 2:
		_weaponDamage.setSize(sf::Vector2f(MAX_SIZE_X*SHOTGUN_DAMAGES / WEAPON_MAX_DAMAGES, MAX_SIZE_Y));
		_weaponRange.setSize(sf::Vector2f(MAX_SIZE_X*SHOTGUN_RANGE / WEAPON_MAX_RANGE, MAX_SIZE_Y));
		_weaponShootSpeed.setSize(sf::Vector2f(MAX_SIZE_X*SHOTGUN_REARMTIME / WEAPON_MAX_REARMTIME, MAX_SIZE_Y));
		magazine.setString("Magazine : \t" + to_string(SHOTGUN_MAGAZINE));
		s = "Shotgun";
		break;
	case 3:
		_weaponDamage.setSize(sf::Vector2f(MAX_SIZE_X*SNIPER_DAMAGES / WEAPON_MAX_DAMAGES, MAX_SIZE_Y));
		_weaponRange.setSize(sf::Vector2f(MAX_SIZE_X*SNIPER_RANGE / WEAPON_MAX_RANGE, MAX_SIZE_Y));
		_weaponShootSpeed.setSize(sf::Vector2f(MAX_SIZE_X*SNIPER_REARMTIME / WEAPON_MAX_REARMTIME, MAX_SIZE_Y));
		magazine.setString("Magazine : \t" + to_string(SNIPER_MAGAZINE));
		s = "Sniper";
		break;
	case 4:
		_weaponDamage.setSize(sf::Vector2f(MAX_SIZE_X*DESERT_EAGLE_DAMAGES / WEAPON_MAX_DAMAGES, MAX_SIZE_Y));
		_weaponRange.setSize(sf::Vector2f(MAX_SIZE_X*DESERT_EAGLE_RANGE / WEAPON_MAX_RANGE, MAX_SIZE_Y));
		_weaponShootSpeed.setSize(sf::Vector2f(MAX_SIZE_X*DESERT_EAGLE_REARMTIME / WEAPON_MAX_REARMTIME, MAX_SIZE_Y));
		magazine.setString("Magazine : \t" + to_string(DESERT_EAGLE_MAGAZINE));
		s = "Desert Eagle";
	default:
		break;
	}
	weaponName.setString(s);
	float weaponNameSize = weaponName.findCharacterPos(s.size()- 1).x - weaponName.findCharacterPos(0).x;
	weaponName.setPosition(sf::Vector2f((float)SCREEN_WIDTH/2 - weaponNameSize / 2, ((float)SCREEN_HEIGHT)/12.0f));

}

Weapon::WeaponType SelectWeaponState::weaponTypeFromSelectedIndex(int selectedIndex) {
	
	switch (selectedIndex)
	{
	case 0:
		return Weapon::WeaponType::Gun;
		break;
	case 1:
		return Weapon::WeaponType::Rifle;
		break;
	case 2:
		return Weapon::WeaponType::Shotgun;
		break;
	case 3:
		return Weapon::WeaponType::Sniper;
		break;
	case 4:
		return Weapon::WeaponType::DesertEagle;
	default:
		return Weapon::WeaponType::Count;
		break;
	}
}

void SelectWeaponState::Draw(float dt)
{
	_data->window.clear();
	_data->window.draw(_background);
	_data->window.draw(_playButton);
	_data->window.draw(_selectedWeapon);
	_data->window.draw(_selectRight);
	_data->window.draw(_selectLeft);
	_data->window.draw(_returnButton);
	_data->window.draw(_weaponDamage);
	_data->window.draw(_weaponRange);
	_data->window.draw(_weaponShootSpeed);
	_data->window.draw(_weaponDamageMax);
	_data->window.draw(_weaponRangeMax);
	_data->window.draw(_weaponShootSpeedMax);
	_data->window.draw(damage);
	_data->window.draw(range);
	_data->window.draw(shoot);
	_data->window.draw(magazine);
	_data->window.draw(weaponName);



	_data->window.display();

}
