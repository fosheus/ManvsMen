#include "Player.h"


Player::Player(GameDataRef data, float x, float y, float angle) : Character(data,x, y, angle) {
}

void Player::updateWeapon(float multiplier)
{
	if (weapon) {
		weapon->update(multiplier,false);
	}
}

Player::Player(GameDataRef data) : Character(data, 0, 0, 0) {

}


void Player::update(float multiplier) {
	
	if (isDead()) {
		return;
	}
	if (health < maxHealth) {
		healProcess += multiplier;
		if (healProcess >= HEAL_PROCESS_TIMEOUT) {
			heal(1);
			healProcess = 0;
		}
	}
	/*Keyboard_management*/
	{
		bool Up = 0, Right = 0, Down = 0, Left = 0;
		bool Shoot = 0, Reload = 0;
		bool Run = 0;
		pickWeapon = 0;
		this->aiming = 0;

		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) Up = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) Left = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) Down = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) Right = 1;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) Shoot = 1;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) this->aiming = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) Reload = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) Run = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && holdPickWeapon == false) {
			pickWeapon = 1; holdPickWeapon = true;
		}
		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E)) holdPickWeapon = false;*/
		if (_data->keys.testInputDown("Up")) Up = 1;
		if (_data->keys.testInputDown("Left")) Left = 1;
		if (_data->keys.testInputDown("Down")) Down = 1;
		if (_data->keys.testInputDown("Right")) Right = 1;
		if (_data->keys.testInputDown("Shoot")) Shoot = 1;
		if (_data->keys.testInputDown("Aim")) this->aiming = 1;
		if (_data->keys.testInputDown("Reload")) Reload = 1;
		if (_data->keys.testInputDown("Run")) Run = 1;
		if (_data->keys.testInputDown("PickUp") && holdPickWeapon == false) {
			pickWeapon = 1; holdPickWeapon = true;
		}
		else if (!_data->keys.testInputDown("PickUp")) holdPickWeapon = false;
		

		
		if (this->aiming) {
			speed = DEFAULT_SPEED* 0.4;
		}
		else if (Run) {
			speed = DEFAULT_SPEED* 1.5;
		} 
		else {
			speed = DEFAULT_SPEED;
		}

		if (weapon) {
			weapon->update(multiplier,aiming);
			if (weapon->noAmmo()) {
				delete weapon;
				weapon = new GunWeapon(_data, -1);
			}
		}

		if (Up || Down || Right || Left) {
			if (Run)
			{
				updateTexture(multiplier*2);
			}
			else {
				updateTexture(multiplier);
			}
		}
		lastDirection = Direction::None;

		if (Up && Right) {
			move(Direction::Up, (float)(multiplier / sqrt(2)));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::Down, (float)(multiplier / sqrt(2)));
			}
			move(Direction::East, (float)(multiplier / sqrt(2)));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::West, (float)(multiplier / sqrt(2)));
			}
		}
		else if (Up && Left) {
			move(Direction::Up, (float)(multiplier / sqrt(2)));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::Down, (float)(multiplier / sqrt(2)));
			}
			move(Direction::West, (float)(multiplier / sqrt(2)));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::East, (float)(multiplier / sqrt(2)));
			}
		}
		else if (Down && Right) {
			move(Direction::Down, (float)(multiplier / sqrt(2)));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::Up, (float)(multiplier / sqrt(2)));
			}
			move(Direction::East, (float)(multiplier / sqrt(2)));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::West, (float)(multiplier / sqrt(2)));
			}
		}
		else if (Down && Left) {
			move(Direction::Down, (float)(multiplier / sqrt(2)));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::Up, (float)(multiplier / sqrt(2)));
			}
			move(Direction::West, (float)(multiplier / sqrt(2)));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::East, (float)(multiplier / sqrt(2)));
			}
		}
		else if (Up && Down) {
			//nothing
		}
		else if (Left && Right) {
			//nothing
		}
		else if (Up) {
			move(Direction::Up, (float)(multiplier));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::Down, (float)(multiplier));
			}
		}
		else if (Down) {
			move(Direction::Down, (float)(multiplier));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::Up, (float)(multiplier));
			}
		}
		else if (Left) {
			move(Direction::West, (float)(multiplier));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::East, (float)(multiplier));
			}
		}
		else if (Right) {
			move(Direction::East, (float)(multiplier));
			if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
				move(Direction::West, (float)(multiplier));
			}
		}
		if (weapon) {
			if (Shoot) {
				shoot();
			}

			if (Reload && !holdReload) {
				holdReload = true;
				weapon->reload();
			}
			else {
				holdReload = false;
			}
		}
	}
	
	this->testBoundsAndRePosition(this->_data->map.getMinWorldBounds(), this->_data->map.getMaxWorldBounds());
	if (weapon) {
		weapon->updateAndKillBulletsOutOfBounds(multiplier, this->_data->map.getMinWorldBounds(), this->_data->map.getMaxWorldBounds());
	}

}


void Player::move(Direction direction, float multiplier) {

	lastDirection = Direction(lastDirection | direction);

	switch (direction)
	{
	case Direction::Forward:
		moveForward(multiplier);
		break;
	case Direction::Backward:
		moveBackward(multiplier);
		break;
	case Direction::Right:
		moveRight(multiplier);
		break;
	case Direction::Left:
		moveLeft(multiplier);
		break;
	case Direction::Up:
		moveUp(multiplier);
		break;
	case Direction::Down:
		moveDown(multiplier);
		break;
	case Direction::West:
		moveWest(multiplier);
		break;
	case Direction::East:
		moveEast(multiplier);
		break;
	case Direction::Back:
		moveBack(multiplier);
	default:
		break;
	}
	
	if (!this->_data->map.isEntityOnPathCell(this->getPosition())) {
		switch (direction)
		{
		case Direction::Forward:
			moveBackward(multiplier);
			break;
		case Direction::Backward:
			moveForward(multiplier);
			break;
		case Direction::Right:
			moveLeft(multiplier);
			break;
		case Direction::Left:
			moveRight(multiplier);
			break;
		case Direction::Up:
			moveDown(multiplier);
			break;
		case Direction::Down:
			moveUp(multiplier);
			break;
		case Direction::West:
			moveEast(multiplier);
			break;
		case Direction::East:
			moveWest(multiplier);
			break;
		case Direction::Back:
			moveBack(multiplier);
		default:
			break;
		}
	}
	
}

float Player::getReloadingProgress()
{
	if (weapon) {
		return weapon->getReloadingProgress();
	}
	return -1.0f;

}



void Player::draw(sf::RenderWindow &window) {
	characterSprite.setPosition(this->getPosition());
	characterSprite.setRotation(-this->angle);
	window.draw(characterSprite);
	if (weapon != NULL) {
		std::vector<Bullet> bullets = getBullets();
		for (size_t i = 0; i < bullets.size(); i++)
		{
			bulletShape.setRotation(-bullets[i].getRotation());
			bulletShape.setPosition(bullets[i].getPosition());
			window.draw(bulletShape);
		}
	}
	
}


void Player::shoot() {
	if (weapon != NULL) {
		if (weapon->canShoot()) {
			sf::Vector2f bulletOrigin = Utils::getGeneralCoordinateFromLocalPoint(TEXTURE_WEAPON_POINT_OFFSET, getPosition(), this->angle);

			weapon->shoot(bulletOrigin.x, bulletOrigin.y, this->angle);

		}
	}
}


std::string Player::toString() {
	return Character::toString() + " "+std::to_string(getHealth());
}


Player::~Player()
{
}
