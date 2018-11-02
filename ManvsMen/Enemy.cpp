#include "Enemy.h"

Enemy::Enemy(GameDataRef data) : Character(data,0,0,0),m_thread(&Enemy::findPath,this), 
	thread_findPathAway(&Enemy::findPathAwayFromPlayer,this) {
	targetPoint = sf::Vector2f(0, 0);
	targetPointFound = false;
	targetSeen = false;
	targetPlayer = NULL;
	pathFound = false;
	escapeSearchFail = 0;
	shootTimeout = 0;
	this->viewFieldRadius = ENEMY_VIEW_FIELD_RADIUS;
}

Enemy::Enemy(GameDataRef data,float x, float y, float angle,float viewFieldRadius) : 
	Character(data,x, y, angle), m_thread(&Enemy::findPath, this) , 
	thread_findPathAway(&Enemy::findPathAwayFromPlayer, this)
{
	targetPoint = sf::Vector2f(x, y);
	targetPointFound = false;
	targetSeen = false;
	targetPlayer = NULL;
	pathFound = false;
	this->health = 100;
	this->maxHealth = 100;
	escapeSearchFail = 0;
	

}

float Enemy::getViewFieldRadius()
{
	if (weapon) {
		if (weapon->getShootRange() > this->viewFieldRadius) {
			return weapon->getShootRange();
		}
	}
	return viewFieldRadius;
}

void Enemy::findRandomTargetPoint(sf::Vector2f posMin, sf::Vector2f posMax) {
	float x = (rand() / (double)RAND_MAX)*(posMax.x - posMin.x) + posMin.x;
	float y= (rand() / (double)RAND_MAX)*(posMax.y - posMin.y) + posMin.y;
	targetPointFound = true;
	this->targetPoint = sf::Vector2f(x, y);
	lastBoxTargetPoint[0] = posMin;
	lastBoxTargetPoint[1] = posMax;

}

bool Enemy::targetInViewField(sf::Vector2f target)
{
	float distance = Utils::distance(target, this->getPosition());
	if (distance < getViewFieldRadius()) {

		sf::Vector2f p1 = target;
		sf::Vector2f p2 = this->getPosition();
		sf::Vector2f p3 = target;

		float deltaX = p1.x - p2.x;
		float deltaY = p1.y - p2.y;
		float speed = 2;
		float angle = atan2(deltaY, deltaX);
		while (this->_data->map.isEntityInMapBounds(p3)) {
			if (Utils::distance(p3, p2) < speed) {
				return true;
			}
			p3.x = p3.x - cos(angle)*speed;
			p3.y = p3.y - sin(angle)*speed;
			if (!this->_data->map.isEntityOnPathCell(p3)) {
				return false;
			}
		}
	}
	return false;

}

bool Enemy::targetInViewFieldRadius() {
	if (this->targetPlayer) {
		float distance = Utils::distance(this->getPosition(), this->targetPlayer->getPosition());
		if (distance < this->viewFieldRadius) {
			return true;
		}
	}
	return false;
}

bool Enemy::targetInViewFieldPeriphery() {
	if (this->targetPlayer) {
		float distance = Utils::distance(this->getPosition(), this->targetPlayer->getPosition());
		if (distance > this->viewFieldRadius && distance < this->viewFieldRadius*1.20f) {
			return true;
		}
	}
	return false;
}

void Enemy::move(Direction direction, float multiplier) {

	if (health > 0) {
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
		case Direction::Target :
			moveToTargetPoint(multiplier);
			break;
		default:
			break;
		}
	}
}



void Enemy::update(float multiplier) {
	

	if (isDead()) {
		return;
	}
	if (health < 100) {
		healProcess += multiplier;
		if (healProcess >= HEAL_PROCESS_TIMEOUT) {
			heal(1);
			healProcess = 0;
		}
	}
	if (weapon) {
		weapon->update(multiplier,aiming);
	}
	if (shootTimeout > 0) {
		shootTimeout -= (int)multiplier;
	}
	updatePosition(multiplier);
	this->testBoundsAndRePosition(this->_data->map.getMinWorldBounds(), this->_data->map.getMaxWorldBounds());

	if (weapon) {
		weapon->updateAndKillBulletsOutOfBounds(multiplier, this->_data->map.getMinWorldBounds(), this->_data->map.getMaxWorldBounds());
	}
}

void Enemy::moveToTargetPoint(float multiplier) {
	if (moving) {
		float deltaX = targetPoint.x - x;
		float deltaY = targetPoint.y - y;
		angle = -Utils::radians_to_degrees(atan2(deltaY, deltaX));
		if (escapeFound) { 
			speed = DEFAULT_SPEED*1.5;
			updateTexture(multiplier*1.5);
		}
		else {
			speed = DEFAULT_SPEED;
			updateTexture(multiplier);
		}
		moveForward(multiplier);
		if (Utils::distance(this->getPosition(), targetPoint) < speed) {
			if (!seenPositionStack.empty()) {
				targetPoint = seenPositionStack.top();
				seenPositionStack.pop();
			}
			else if (pathFound && pathPoints.size() != 0) {
				targetPoint = pathPoints[pathPoints.size() - 1];
				pathPoints.pop_back();
			}
			else {
				escapeFound = false;
				pathFound = false;
				escapeSearchFail = 0;
				m_thread.launch();
			}
		}
	}
}

void Enemy::findPathAwayFromPlayer() {
	srand(time(NULL));
	threadRunning = true;
	std:vector<sf::Vector2f> safePoints;
	if (targetPlayer)
	{
		safePoints = this->_data->map.getSafePoints(this->getPosition(), targetPlayer->getPosition());
		
		if (safePoints.size() > 0) {
			this->pathPoints = this->_data->map.getPathFromToAvoidingPlayer(this->getPosition(), safePoints[rand() % safePoints.size()], targetPlayer->getPosition(), this->viewFieldRadius);
		}
			if (pathPoints.size() > 0) {
				escapeFound = true;
				escapeSearchFail = 0;
			}
			else {
				escapeSearchFail++;
		}
	}
	safePoints.clear();
	threadRunning = false;
}

void Enemy::findPath() {
	srand(time(NULL));
	sf::Clock clock;
	Point p;
	pathPoints = this->_data->map.getPathFromTo(this->getPosition(), this->_data->map.getRandomPathPoint());
	if (pathPoints.size() != 0) {
		pathFound = true;
	}
}



void Enemy::updatePosition(float multiplier) {

	float deltaX = targetPlayer->getX() - x;
	float deltaY = targetPlayer->getY() - y;
	float angle = -Utils::radians_to_degrees(atan2(deltaY, deltaX));

	aiming = false;

	if (targetPlayer)
	{
		float distance = Utils::distance(targetPlayer->getPosition(), this->getPosition());
		setRotation(angle);
		if (targetInViewField(targetPlayer->getPosition())) {
			if (weapon && weapon->getAmmo() != 0 && getPercentHealth() > 0.4f*targetPlayer->getPercentHealth()) {
				if (targetSeen == false) {
					shootTimeout = ENEMY_SHOOT_TIMEOUT;
					targetSeen = true;
					seenPositionStack.push(getPosition());
				}

				if (!seenPositionStack.empty()) {
					if (seenPositionStack.top() != targetPlayer->getPosition()) {
						seenPositionStack.push(targetPlayer->getPosition());
						targetPoint = seenPositionStack.top();
					}
				}
				else {
					seenPositionStack.push(targetPlayer->getPosition());
				}
				if (weapon->getShootRange() >= distance*0.8) {
					moveForward(multiplier);
					shoot();
				}
				else {
					shooting = false;
					moveForward(multiplier);
				}
			}
			else {
				if (!escapeFound && !threadRunning && escapeSearchFail < ENEMY_ESCAPEFAILTIMEOUT) {
					thread_findPathAway.launch();
					targetSeen = false;
				}
				else if (!escapeFound && escapeSearchFail >= ENEMY_ESCAPEFAILTIMEOUT) {
					if (targetSeen == false) {
						shootTimeout = ENEMY_SHOOT_TIMEOUT;
						targetSeen = true;
					}
					if (!seenPositionStack.empty()) {
						if (seenPositionStack.top() != targetPlayer->getPosition()) {
							seenPositionStack.push(targetPlayer->getPosition());
							targetPoint = seenPositionStack.top();

						}
					}
					else {
						seenPositionStack.push(targetPlayer->getPosition());
					}

					if (weapon->getShootRange() >= distance*0.8) {
						shoot();
					}
					else {
						shooting = false;
						moveForward(multiplier);
					}
				}
				else {
					moveToTargetPoint(multiplier);
				}
			}
		}
		else {
			targetSeen = false;
			shootTimeout = 0;
			moveToTargetPoint(multiplier);
		}
	}
	else {

		moveToTargetPoint(multiplier);

	}
}
void Enemy::updateWeapon(float multiplier)
{
	if (weapon) {
		weapon->update(multiplier,false);
	}
}
void Enemy::setTargetPlayer(Player* targetPlayer) {
	this->targetPlayer = targetPlayer;
}

void Enemy::setTargetPoint(sf::Vector2f targetPoint) {
	this->targetPoint = targetPoint;
}
Player* Enemy::getTargetPlayer() {
	return targetPlayer;
}

void Enemy::draw(sf::RenderWindow& window) {
	if (isDead() ) {
		return;
	}
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

void Enemy::shoot() {
	aiming = true;
	if (weapon->getWeaponType() == Weapon::WeaponType::Sniper) {
		SniperWeapon* sniper  = (SniperWeapon*)weapon;
		if (sniper->getCurrentPrecisionAngle() / (float)SNIPER_PRECISION_ANGLE > 0.3f) {
			return;
		}
	}
	if (shootTimeout > 0)
		return;
	if (weapon != NULL) {
		if (weapon->canShoot()) {
			sf::Vector2f bulletOrigin = Utils::getGeneralCoordinateFromLocalPoint(TEXTURE_WEAPON_POINT_OFFSET, getPosition(), this->angle);
			weapon->shoot(bulletOrigin.x, bulletOrigin.y, this->angle);
		}
	}
}



Enemy::~Enemy()
{
}
