#include "GameState.h"
#include "DEFINITIONS.h"
#include "MainMenuState.h"
#include "GameoverState.h"
#include "PauseMenuState.h"


GameState::GameState(GameDataRef data, Weapon::WeaponType playerWeapon) :_data(data),map(data),player(data)
{
	this->_data->map.generateMap(MAP_WIDTH, MAP_HEIGHT, BLOCKSIZE);
	this->playerInitialWeaponType = playerWeapon;
}

GameState::~GameState()
{
}

void GameState::Init()
{
	mainView.setSize(sf::Vector2f(MAIN_VIEW_WIDTH, MAIN_VIEW_HEIGHT));
	this->_data->window.setView(mainView);
	
	killCountDisplay.setFont(this->_data->assets.GetFont(ARIAL_FONT));
	enemyNumberDisplay.setFont(this->_data->assets.GetFont(ARIAL_FONT));
	textFps.setFont(this->_data->assets.GetFont(ARIAL_FONT));
	ammoInfo.setFont(this->_data->assets.GetFont(ARIAL_FONT));
	healthStatus.setFont(this->_data->assets.GetFont(ARIAL_FONT));

	healthStatus.setScale(sf::Vector2f(0.75f, 0.75f));
	healthStatus.setPosition(200, 200);
	textFps.setPosition(0, 50);
	ammoInfo.setScale(sf::Vector2f(0.75f, 0.75f));

	healthBar.setSize(sf::Vector2f(100, 10));
	healthBar.setFillColor(sf::Color::Green);
	protectionBar.setSize(sf::Vector2f(100, 10));
	protectionBar.setFillColor(sf::Color::Blue);

	reloadingBar.setSize(sf::Vector2f(100, 10));
	reloadingBar.setFillColor(sf::Color(0, 0, 0, 200));

	killCountDisplay.setPosition(0, 0);
	enemyNumberDisplay.setPosition(0, 40);


	focus = true;
	map.load(sf::Vector2u(BLOCKSIZE, BLOCKSIZE), this->_data->map.getMatrix(), MAP_WIDTH, MAP_HEIGHT);

	srand((unsigned int)time(NULL));
	sf::Vector2f playerPos = sf::Vector2f(this->_data->map.getFirstPathCellTopLeft());
	player = Player(this->_data,playerPos.x,playerPos.y,0);
	player.changeWeapon(Weapon::weaponFromWeaponType(this->_data,playerInitialWeaponType));

	enemies.clear();
	for (size_t i = 0; i < NUMBER_ENEMIES; i++) {
		spawnEnemy();
	}
	for (size_t i = 0; i < 3; i++)
	{
		Weapon*w = NULL;
		int wt = rand() % ((int)(Weapon::WeaponType::Count) - 1);
		switch ( wt) {
			case (int)Weapon::WeaponType::Rifle: 
				w = new RifleWeapon(this->_data, (rand() % (RIFLE_MAGAZINE*3- RIFLE_MAGAZINE) + RIFLE_MAGAZINE));
				spawnWeapon(this->_data->map.getRandomPathPoint(), w);
				break;
			case (int)Weapon::WeaponType::Shotgun:
				w = new ShotgunWeapon(this->_data, (rand() % (SHOTGUN_MAGAZINE*3- SHOTGUN_MAGAZINE) + SHOTGUN_MAGAZINE));
				spawnWeapon(this->_data->map.getRandomPathPoint(), w);
				break;
			case (int)Weapon::WeaponType::Sniper:
				w = new SniperWeapon(this->_data, (rand() % (SNIPER_MAGAZINE*3- SNIPER_MAGAZINE) + SNIPER_MAGAZINE));
				spawnWeapon(this->_data->map.getRandomPathPoint(), w);
				break;
			case (int)Weapon::WeaponType::DesertEagle:
				w = new DesertEagleWeapon(this->_data, (rand() % (DESERT_EAGLE_MAGAZINE*3 - DESERT_EAGLE_MAGAZINE) + DESERT_EAGLE_MAGAZINE));
				spawnWeapon(this->_data->map.getRandomPathPoint(), w);
				break;
		}
		
	}
	for (size_t i = 0; i < 3; i++)
	{
		entities.push_back(new MedikitEntity(this->_data,this->_data->map.getRandomPathPoint()));
	}
	for (size_t i = 0; i < 3; i++)
	{
		int protectionBonus = rand() % (60 - 20) + 10;
		entities.push_back(new KevlarEntity(this->_data, this->_data->map.getRandomPathPoint(),protectionBonus));
	}

	Running = true;
	displayMenu = false;
}

void GameState::HandleInput()
{
	sf::Event event;
	
	while (this->_data->window.pollEvent(event))
	{
		
		if (event.type == sf::Event::Closed) {
			this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
		}
		if (event.type == sf::Event::Resized) {
			mainView.setSize(this->_data->window.getSize().x, this->_data->window.getSize().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			sf::Vector2u windowSize= _data->window.getSize();
			sf::Texture tex;
			tex.create(windowSize.x,windowSize.y);
			tex.update(_data->window);
			this->_data->machine.AddState(StateRef(new PauseMenuState(this->_data,tex.copyToImage() )), false);
		}
		

	}
}

void GameState::Update(float dt)
{

	float finalMultiplier = dt*multiplier;
	if (!displayMenu) {

		if (!player.isDead()) {
			playerUpdate(finalMultiplier);
			playerUpdateZoom(finalMultiplier);
			playerCollideEntities(finalMultiplier);
			playerBulletsCollisionEnemies(finalMultiplier);
			enemiesUpdate(finalMultiplier);
			entitiesUpdate(finalMultiplier);
			collisionManagement(finalMultiplier);

			
		}
		else {
			sf::Vector2u windowSize = _data->window.getSize();
			sf::Texture tex;
			tex.create(windowSize.x, windowSize.y);
			tex.update(_data->window);
			this->_data->machine.AddState(StateRef(new GameoverState(this->_data, tex.copyToImage())),true);
		}

	}

}

void GameState::Draw(float dt)
{

	float reloadingProgress = player.getReloadingProgress();
	//CLEAR AND SET WINDOW//
	mainView.setCenter(player.getPosition());
	this->_data->window.clear();
	this->_data->window.setView(mainView);

	
	killCountDisplay.setString("Kill Count : "+to_string(player.getKillCount()));
	enemyNumberDisplay.setString("Enemy Number : " + to_string(enemies.size()));

	//DRAW MAP//
	this->_data->window.draw(map);

	//DRAW ENTITIES//
	for (size_t i = 0; i < entities.size(); i++)
	{
		this->_data->window.draw(*entities[i]);
	}
	//DRAW PLAYER//
	player.draw(this->_data->window);


	//DRAW ENEMIES//
	healthBar.setFillColor(sf::Color::Red);
	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->targetInViewField(player.getPosition())) {
			enemies[i]->getSprite().setColor(sf::Color::Green);
		}
		else {
			enemies[i]->getSprite().setColor(sf::Color::Red);
		}
		enemies[i]->draw(this->_data->window);

		//DRAW HEALTHBAR//
		float pHealth = enemies[i]->getPercentHealth();
		healthBar.setSize(sf::Vector2f(HEALTHMAXSIZE.x*pHealth, HEALTHMAXSIZE.y));
		healthBar.setPosition(sf::Vector2f(enemies[i]->getPosition().x - healthBar.getSize().x / 2, enemies[i]->getPosition().y - 20));
		this->_data->window.draw(healthBar);

	}
	healthBar.setFillColor(sf::Color::Green);

	if (player.getProtection()) {
		healthBar.setSize(sf::Vector2f(HEALTHMAXSIZE.x*player.getPercentHealth(), HEALTHMAXSIZE.y/2));
		protectionBar.setSize(sf::Vector2f(HEALTHMAXSIZE.x*player.getPercentProtection(),HEALTHMAXSIZE.y/2));

		healthBar.setPosition(sf::Vector2f(player.getPosition().x - healthBar.getSize().x / 2 , player.getPosition().y - 50));
		protectionBar.setPosition(sf::Vector2f(player.getPosition().x  - protectionBar.getSize().x / 2, player.getPosition().y - 45));
	}
	else {
		healthBar.setSize(sf::Vector2f(HEALTHMAXSIZE.x*player.getPercentHealth(), HEALTHMAXSIZE.y));
		healthBar.setPosition(sf::Vector2f(player.getPosition().x - healthBar.getSize().x / 2, player.getPosition().y - 50));
	}

	if (reloadingProgress>=0) {
		reloadingBar.setSize(sf::Vector2f(HEALTHMAXSIZE.x*reloadingProgress, HEALTHMAXSIZE.y));
		reloadingBar.setPosition(sf::Vector2f(player.getPosition().x - HEALTHMAXSIZE.x/2, player.getPosition().y - 65));
	} 
	if (player.getWeapon()) {
		if (player.getWeapon()->getWeaponType() == Weapon::WeaponType::Sniper) {

			SniperWeapon* sniper = (SniperWeapon*)player.getWeapon();
			float precision = sniper->getCurrentPrecisionAngle();
			sf::Vertex line1[] = {
				sf::Vertex(player.getPosition()),
			sf::Vertex(sf::Vector2f(player.getPosition().x + SNIPER_RANGE*cos(Utils::degrees_to_radians(-player.getRotation() - precision)),
				player.getPosition().y + SNIPER_RANGE*sin(Utils::degrees_to_radians(-player.getRotation() - precision))))
			};
			sf::Vertex line2[] = {
				sf::Vertex(sf::Vector2f(player.getPosition())),
			sf::Vertex(sf::Vector2f(player.getPosition().x + SNIPER_RANGE*cos(Utils::degrees_to_radians(-player.getRotation() + precision)),
				player.getPosition().y + SNIPER_RANGE*sin(Utils::degrees_to_radians(-player.getRotation() + precision))))
			};
			
			float nextWidth = MAIN_VIEW_WIDTH*zoomStatus / 3.3333f + MAIN_VIEW_WIDTH;
			float nextHeight = MAIN_VIEW_HEIGHT*zoomStatus / 3.3333f + MAIN_VIEW_HEIGHT;
			mainView.setSize(sf::Vector2f(nextWidth, nextHeight));
			this->_data->window.draw(line1, 2, sf::Lines);
			this->_data->window.draw(line2, 2, sf::Lines);
		}
	}





	ammoInfo.setString(player.getWeaponInfo());
	float ammoInfoTextSize = ammoInfo.findCharacterPos(player.getWeaponInfo().size() - 1).x - ammoInfo.findCharacterPos(0).x;
	ammoInfo.setPosition(sf::Vector2f(player.getPosition().x - ammoInfoTextSize / 2, player.getPosition().y - 30));

	this->_data->window.draw(healthBar);
	this->_data->window.draw(ammoInfo);
	if (player.getProtection()) {
		this->_data->window.draw(protectionBar);
	}
	if (reloadingProgress>=0) {
		this->_data->window.draw(reloadingBar);
	}
	

	//DRAW GAME INFOS//
	this->_data->window.setView(this->_data->window.getDefaultView());

	this->_data->window.draw(killCountDisplay);
	this->_data->window.draw(enemyNumberDisplay);
	this->_data->window.display();
}

void GameState::spawnEnemy() {
	Enemy* enemy = new Enemy(this->_data);
	enemy->setHealth(DEFAULT_HEATLH);
	enemy->setPosition(this->_data->map.getRandomPathPointInOrOutRange(ENEMY_SPAWN_RANGE, false, player.getPosition()));
	Weapon * w = NULL;
	switch (rand() % (int)Weapon::WeaponType::Count) {
	case (int)Weapon::WeaponType::Rifle:
		w=new RifleWeapon(this->_data, (rand() % (80 - 20) + 20));
		enemy->changeWeapon(w);
		break;
	case (int)Weapon::WeaponType::Shotgun:
		w = new ShotgunWeapon(this->_data, (rand() % (40 - 10) + 10));
		enemy->changeWeapon( w);
		break;
	case (int)Weapon::WeaponType::Gun:
		w = new GunWeapon(this->_data, -1);
		enemy->changeWeapon(w);
		break;
	case (int)Weapon::WeaponType::Sniper:
		w = new SniperWeapon(this->_data, (rand() % (36 - 12) + 12));
		enemy->changeWeapon(w);
		break;
	case (int)Weapon::WeaponType::DesertEagle:
		w = new DesertEagleWeapon(this->_data, (rand() % (36 - 12) + 12));
		enemy->changeWeapon(w);
		break;
	default :
		std::cout << "GameState.cpp ,SpawnEnemy Arme inconnue" << std::endl;
		break;
	}
	enemy->startMoving();
	enemy->setTargetPlayer(&player);
	enemy->setTargetPoint(enemy->getPosition());
	enemies.push_back(enemy);

}


void GameState::spawnWeapon(sf::Vector2f position, Weapon* weapon)
{
	entities.push_back(new WeaponEntity(this->_data,position, weapon));

}

void GameState::spawnBackback( Character * character)
{
	std::vector<Entity*> entities;
	int giveMedikit = rand() % 100;
	int giveProtection = rand() % 100;
	int protectionBonus = (rand() % 45) + 5;
	if (giveMedikit < ENEMY_PERCENT_DROP_MEDIKIT) {
		entities.push_back(new MedikitEntity(this->_data, character->getPosition()));
	}
	if (giveProtection < ENEMY_PERCENT_DROP_PROTECTION) {
		entities.push_back(new KevlarEntity(this->_data, character->getPosition(),protectionBonus));
	}
	entities.push_back(new WeaponEntity(this->_data, character->getPosition(), character->getWeapon()));
	
	Entity* backpack = new BackpackEntity(this->_data, character->getPosition(),entities);

	this->entities.push_back(backpack);
}


void GameState::playerUpdateZoom(float multiplier)
{	
	if (player.getWeapon()) {
		if (player.getWeapon()->getWeaponType() == Weapon::WeaponType::Sniper) {
			SniperWeapon* sniper = (SniperWeapon*)player.getWeapon();
			if (player.isAiming()) {
				zoomStatus = sniper->getAimingProgress();
			}
			else if (zoomStatus>0) {
				zoomStatus -= multiplier / 25;

				if (zoomStatus < 0) {
					zoomStatus = 0;
				}
			}
		}
	}
}


void GameState::playerBulletsCollisionEnemies(float multiplier) {
	int k = 0;
	std::vector<int> indexBulletToErase;
	for (Bullet var : player.getBullets())
	{

		sf::RectangleShape r;
		r.setPosition(var.getPosition());
		r.setRotation(-var.getRotation());
		r.setSize(sf::Vector2f(BULLETSHAPE_WIDTH, BULLETSHAPE_HEIGHT));
		if (!this->_data->map.isEntityOnPathCell(var.getPosition())) {
			indexBulletToErase.push_back(k);
		}
		else {
			for (size_t j = 0; j < enemies.size(); j++)
			{
				sf::RectangleShape enemyMinBounds = enemies[j]->getMinBounds();

				if (Utils::rotatedCollisionDetection(r, enemyMinBounds)) {
					player.hurtCharacter(enemies[j],var.getDamages());
					if (!(std::find(indexBulletToErase.begin(), indexBulletToErase.end(), k) != indexBulletToErase.end())) {
						indexBulletToErase.push_back(k);
					}

				}
			}
		}

		k++;
	}
	for (int index : indexBulletToErase) {
		if (index < player.getBullets().size()) {
			player.getBullets().erase(player.getBullets().begin() + index);
		}
	}
	std::vector<int> indexEnemyToErase;
	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->isDead()) {
			indexEnemyToErase.push_back(i);
			spawnBackback(enemies[i]);
		}
	}

	for (size_t i = 0; i < indexEnemyToErase.size(); i++)
	{
		enemies.erase(enemies.begin() + indexEnemyToErase[i]);
		spawnEnemy();
	}
}

void GameState::playerCollideEntities(float mulitplier)
{
	sf::RectangleShape playerMinBounds = player.getMinBounds();

	for (size_t i = 0; i < entities.size(); i++)
	{
		if (Utils::rotatedCollisionDetection(playerMinBounds, entities[i]->getMinBounds()))
		{
			if (entities[i]->getPickTimeout() == 0) {
				Entity *e = entities[i]->pickedBy(&player);
				if (entities[i] == e) {
					//nothing
				}
				else if (e != NULL) {
					delete entities[i];
					entities[i] = e;
				}
				else {
					entities.erase(entities.begin() + i);
				}
			}

		}
	}

}

void GameState::entitiesUpdate(float multiplier)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->update(multiplier);
	}
}

void GameState::playerUpdate(float multiplier) {
	sf::Vector2f v = this->_data->window.mapPixelToCoords(sf::Mouse::getPosition(this->_data->window));
	float deltaX = v.x - this->_data->window.getView().getCenter().x;
	float deltaY = v.y - this->_data->window.getView().getCenter().y;
	float angleMouse = Utils::radians_to_degrees(atan2(deltaY, deltaX));

	player.setRotation(-angleMouse);
	player.update(multiplier);
	if (!this->_data->map.isEntityOnPathCell(player.getPosition())) {
		player.move(Character::Direction::Back, multiplier);
	}
	int spawnStatus = player.getKillCount() % 10;
	if (spawnStatus == 0 && enemySpawned == 0) {
		spawnEnemy();
		enemySpawned = 1;
	}
	if (spawnStatus != 0) {
		enemySpawned = 0;
	}
}

void GameState::enemiesUpdate(float multiplier) {
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->update(multiplier);
		
	}
}

void GameState::collisionManagement(float multiplier) {
	sf::RectangleShape playerMinBounds = player.getMinBounds();

	for (size_t i = 0; i < enemies.size(); i++) {
		sf::RectangleShape enemyMinBounds = enemies[i]->getMinBounds();
		if (Utils::circleCollisionDetection(player.getPosition(), enemies[i]->getPosition(), CHARACTER_RADIUS, CHARACTER_RADIUS)) {
			moveCharactersOpposite(&player, enemies[i],multiplier);
			if (!_data->map.isEntityOnPathCell(player.getPosition())) {
				//get cell position 
				moveCharacterAwayFromPosition(&player, _data->map.getMatrixPosToRealPos(player.getPosition()),multiplier);
			}
		}
		for (size_t j = 0; j < enemies[i]->getBullets().size(); j++)
		{
			Bullet bullet = enemies[i]->getBullets()[j];
			sf::RectangleShape r;
			r.setPosition(bullet.getPosition());
			r.setRotation(-bullet.getRotation());
			r.setSize(sf::Vector2f(BULLETSHAPE_WIDTH, BULLETSHAPE_HEIGHT));
			if (Utils::rotatedCollisionDetection(r, playerMinBounds)) {
				enemies[i]->hurtCharacter(&player, bullet.getDamages());
				enemies[i]->getBullets().erase(enemies[i]->getBullets().begin() + j);
			}
		}
	}
}

void GameState::moveCharactersOpposite(Character* character1, Character* character2, float multiplier) {

	// Distance between ball centers
	float fDistance =Utils::distance(character1->getPosition(),character2->getPosition()) ;

	// Calculate displacement required
	float fOverlap = 0.5f * (fDistance - CHARACTER_RADIUS - CHARACTER_RADIUS);

	// Displace Current Ball away from collision
	float c1x = character1->getX()- fOverlap * (character1->getX() - character2->getX()) / fDistance;
	float c1y = character1->getY() - fOverlap * (character1->getY() - character2->getY()) / fDistance;
	character1->setPosition(c1x, c1y);
	// Displace Target Ball away from collision
	float c2x = character2->getX() + fOverlap * (character1->getX() - character2->getX()) / fDistance;
	float c2y = character2->getY() + fOverlap * (character1->getY() - character2->getY()) / fDistance;
	character2->setPosition(c2x, c2y);


}

void GameState::moveCharacterAwayFromPosition(Character * character, sf::Vector2f position, float multiplier)
{
	// Distance between ball centers
	float fDistance = Utils::distance(character->getPosition(), position);

	// Calculate displacement required
	float fOverlap = 0.5f * (fDistance - CHARACTER_RADIUS - CHARACTER_RADIUS);

	// Displace Current Ball away from collision
	float c1x = character->getX() - fOverlap * (character->getX() - position.x) / fDistance;
	float c1y = character->getY() - fOverlap * (character->getY() - position.y) / fDistance;
	character->setPosition(c1x, c1y);
	
}

