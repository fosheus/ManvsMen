#include "Character.h"
#include "WeaponEntity.h"
#include "MedikitEntity.h"


void Character::init() {
	health = CHARACTER_MAX_HEALTH;
	maxHealth = CHARACTER_MAX_HEALTH;
	maxProtection = CHARACTER_MAX_PROTECTION;
	protection = CHARACTER_DEFAULT_PROTECTION;
	bulletShape.setFillColor(sf::Color::Red);
	bulletShape.setSize(sf::Vector2f(BULLETSHAPE_WIDTH, BULLETSHAPE_HEIGHT));
	healProcess = 0;
	killCount = 0;
	shooting = false;
	animation = new Animation(&this->_data->assets.GetImage(CHARACTER_SPRITESHEET), sf::Vector2u(CHARACTER_SPRITESHEET_COLUMN, CHARACTER_SPRITESHEET_ROW), CHARACTER_SPRITESHEET_SWITCH_TIME);
	
	animation->update(0, 0);
	this->characterSprite.setTexture(this->_data->assets.GetImage(CHARACTER_SPRITESHEET));
	this->characterSprite.setTextureRect(this->animation->uvRect);
	this->characterSprite.setOrigin(TEXTURE_CENTER);
	this->characterSprite.setRotation(this->getRotation());
	this->characterSprite.setPosition(this->getPosition());
	this->characterSprite.setScale(TEXTURE_SCALE, TEXTURE_SCALE);
	this->characterSprite.setTextureRect(this->animation->uvRect);
	
}

Character::Character(GameDataRef data) : _data(data){
	x = 0;
	y = 0;
	angle = 0;
	speed = DEFAULT_SPEED;
	init();
}

Character::Character(GameDataRef data,float x,float y, float angle) :_data(data)
{
	this->x = x;
	this->y = y;
	this->angle = angle;
	this->speed = DEFAULT_SPEED;
	init();
}


Character::~Character()
{
}

/*MOUVEMENT*/
void Character::moveForward(float multiplier) {

	x += cos(Utils::degrees_to_radians(angle))*speed*multiplier;
	y -= sin(Utils::degrees_to_radians(angle))*speed*multiplier;
}

void Character::moveBackward(float multiplier) {
	x -= cos(Utils::degrees_to_radians(angle))*speed*multiplier;
	y += sin(Utils::degrees_to_radians(angle))*speed*multiplier;
}

void Character::moveLeft(float multiplier) {
	x -= cos(Utils::degrees_to_radians(angle + 90))*speed*multiplier;
	y += sin(Utils::degrees_to_radians(angle + 90))*speed*multiplier;
}


void Character::moveRight(float multiplier) {
	x -= cos(Utils::degrees_to_radians(angle - 90))*speed*multiplier;
	y += sin(Utils::degrees_to_radians(angle - 90))*speed*multiplier;
}

void Character::moveDown(float multiplier) {
	y += multiplier*speed;
}
void Character::moveUp(float multiplier) {
	y -= multiplier*speed;
}
void Character::moveWest(float multiplier) {
	x -= multiplier*speed;
}
void Character::moveEast(float multiplier) {
	x += multiplier*speed;
}
void Character::moveBack(float multiplier) {

	if (lastDirection & Direction::Forward) {
		moveBackward(multiplier);
		if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
			moveForward(multiplier);
		}
	}
	if (lastDirection & Direction::Backward) {
		moveForward(multiplier);
		if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
			moveBackward(multiplier);
		}
	}
	if (lastDirection & Direction::Right) {
		moveLeft(multiplier);
		if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
			moveRight(multiplier);
		}
	}
	if (lastDirection & Direction::Left) {
		moveRight(multiplier);
		if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
			moveLeft(multiplier);
		}
	}
	if (lastDirection & Direction::Up) {
		moveDown(multiplier);
		if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
			moveUp(multiplier);
		}
	}
	if (lastDirection & Direction::Down) {
		moveUp(multiplier);
		if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
			moveDown(multiplier);
		}
	}
	if (lastDirection & Direction::West) {
		moveEast(multiplier);
		if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
			moveWest(multiplier);
		}
	}
	if (lastDirection & Direction::East) {
		moveWest(multiplier);
		if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
			moveEast(multiplier);
		}
	}
}

/*BOUNDS MANAGEMENT*/
sf::RectangleShape Character::getMinBounds() {

	sf::Vector2f point = Utils::getGeneralCoordinateFromLocalPoint(TEXTURE_TOP_LEFT_OFFSET, getPosition(), this->angle);
	sf::RectangleShape rect;

	rect.setPosition(point.x, point.y);
	rect.setRotation(-this->angle);
	rect.setSize(TEXTURE_HITBOX_SIZE);
	return rect;
}


void Character::updateTexture(float multiplier) {
	if (weapon) {
		switch (weapon->getWeaponType())
		{
		case Weapon::WeaponType::Rifle:
			animation->update(1, multiplier);
			break;
		case Weapon::WeaponType::Shotgun:
			animation->update(2, multiplier);
			break;
		case Weapon::WeaponType::Gun:
			animation->update(0, multiplier);
			break;
		case Weapon::WeaponType::Sniper:
			animation->update(3,multiplier);
			break;
		case Weapon::WeaponType::DesertEagle:
			animation->update(4, multiplier);
			break;
		default:
			animation->update(0, multiplier);
			break;
		}
	}
	else {
		animation->update(0, multiplier);
	}
	this->characterSprite.setTextureRect(animation->uvRect);

}

/*HEALTH*/

bool Character::hurt(int damages) {

	int oldProtection = 0;
	int damagesTakenProtection = 0;
	int damagestakenBody = 0;
	if (health <= 0) {
		return false;
	}
	if (protection) {
		damagesTakenProtection = (float)(damages*0.6f);
		damagestakenBody = damages - damagesTakenProtection;

		oldProtection = protection;
		if (protection >= (damages*0.6f)) {
			protection -= (damages*0.6f);
			this->health -= damagestakenBody;
		}
		else {
			int delta = (int)(damages*0.6f) - protection;
			protection = 0;
			health -= delta + damagestakenBody;

		}

	}
	else {
		this->health -= damages;
		if (health < 0) {
			health = 0;
		}
	}
	return true;

}

bool Character::heal(int points) {
	if (health >= maxHealth) {
		health = maxHealth;
		return false;
	}
	this->health += points;
	if (health > maxHealth) {
		health = maxHealth;
	}
	return true;
}

void Character::testBoundsAndRePosition(sf::Vector2f minWorldBounds, sf::Vector2f maxWorldBounds) {
	if (x < minWorldBounds.x) x = minWorldBounds.x;
	if (x > maxWorldBounds.x) x = maxWorldBounds.x;
	if (y < minWorldBounds.y) y = minWorldBounds.y;
	if (y > maxWorldBounds.y) y = maxWorldBounds.y;
}

void Character::setPosition(float x, float y) {
	this->x = x;
	this->y = y;
}

void Character::setRotation(float angle) {
	this->angle = angle;
}

void Character::setProtection(int protection)
{
	if (protection > 0) {
		this -> protection = protection;
	}
	if (this->protection > CHARACTER_MAX_PROTECTION) {
		this->protection = CHARACTER_MAX_PROTECTION;
	}
}
void Character::addProtection(int protection) {
	if (protection > 0) {
		this->protection += protection;
	}
	if (this->protection > CHARACTER_MAX_PROTECTION) {
		this->protection = CHARACTER_MAX_PROTECTION;
	}
}



std::string Character::toString() {

	std::string x = Utils::floatToStringWPrecision(this->x, 2);
	std::string y = Utils::floatToStringWPrecision(this->y, 2);
	std::string angle = Utils::floatToStringWPrecision(this->angle, 2);
	std::string speed = Utils::floatToStringWPrecision(this->speed, 2);
	return "[" + x + ";" + y + "]" + " a=" + angle + " v=" + speed;
}

bool Character::outOfBounds(sf::Vector2f minWorldBounds, sf::Vector2f maxWorldBounds) {
	if (this->x < minWorldBounds.x || this->x > maxWorldBounds.x) {
		return true;
	}
	if (this->y <minWorldBounds.y || this->y > maxWorldBounds.y) {
		return true;
	}
	return false;
}

void Character::hurtCharacter(Character * character,int damages)
{
	character->hurt(damages);
	if (character->isDead()) {
		this->killCount++;
	}
}


/*DRAW*/



/*WEAPON*/
std::vector<Bullet>& Character::getBullets() {
	if (weapon) {
		return weapon->getBullets();
	} 
	std::vector<Bullet> empty;
	return empty;
}
size_t Character::getBulletNumber() {
	if (weapon) {
		return weapon->getBullets().size();
	} return 0;
}

int Character::getMagazine() {
	if (weapon) {
		return weapon->getMagazine();
	}
	return 0;
}
int Character::getAmmo() {
	if (weapon) {
		return weapon->getAmmo();
	}
	return 0;
}

void Character::move(float angle,float multiplier)
{
	this->x += cos(angle)*speed*multiplier;
	this->y += sin(angle)*speed*multiplier;
}

std::string Character::getWeaponInfo() {
	if (weapon) {
		return weapon->weaponInfos();
	}
	else {
		return "No Weapon";
	}
}



void Character::changeWeapon(Weapon* weapon) {

	this->weapon = weapon;

	if (weapon) {
		switch (weapon->getWeaponType())
		{
		case Weapon::WeaponType::Rifle:
			animation->resetAnimation(1);
			break;
		case Weapon::WeaponType::Shotgun:
			animation->resetAnimation(2);
			break;
		case Weapon::WeaponType::Gun:
			animation->resetAnimation(0);
			break;
		case Weapon::WeaponType::Sniper:
			animation->resetAnimation(3);
			break;
		case Weapon::WeaponType::DesertEagle:
			animation->resetAnimation(4);
			break;
		default:
			animation->resetAnimation(0);
			break;
		}
	}
	else {
		animation->resetAnimation(0);
	}
	this->characterSprite.setTextureRect(animation->uvRect);
}

