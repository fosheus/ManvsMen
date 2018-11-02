#include "GameState.h"
#include "DEFINITIONS.h"
#include "MainMenuState.h"
#include "GameoverState.h"
#include "PauseMenuState.h"
#include <set>
#include <algorithm>
#include "Vector2fComparator.h"


GameState::GameState(GameDataRef data, Weapon::WeaponType playerWeapon) :_data(data),map(data),player(data),fovThread(&GameState::fovOperator,this)
{
	this->_data->map.generateMap(MAP_WIDTH, MAP_HEIGHT, BLOCKSIZE);
	this->playerInitialWeaponType = playerWeapon;
}

GameState::~GameState()
{
	fovThreadStarted = false;
	fovThread.wait();
}

void GameState::Init()
{
	//set window and view settings
	mainView.setSize(sf::Vector2f(MAIN_VIEW_WIDTH, MAIN_VIEW_HEIGHT));
	this->_data->window.setView(mainView);
	
	//load map in the tilemap renderer
	map.load(sf::Vector2u(BLOCKSIZE, BLOCKSIZE), this->_data->map.getMatrix(), this->_data->map.getMatrixSize().x, this->_data->map.getMatrixSize().y);

	//load hud
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

	//thread fov
	fovThread.launch();
}

void GameState::HandleInput()
{
	sf::Event event;
	
	while (this->_data->window.pollEvent(event))
	{
		
		if (event.type == sf::Event::Closed) {
			fovThreadStarted = false;
			fovThread.wait();
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
			fovThreadStarted = false;
			fovThread.wait();
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
	
	//DRAW FOV//
	
	std::map<float, IntersectionPoint>::iterator it;
	float distance = 0.0f;
	float multiplier = 0.0f;
	mutex.lock();
	if (intersectionPoints.size()>0) {
		for (it = std::next(intersectionPoints.begin(), 1); it != intersectionPoints.end(); it++) {

			sf::VertexArray varray(sf::Triangles, 3);
			auto prev = std::prev(it, 1);
			varray[0].position = sf::Vector2f(prev->second.x, prev->second.y);
			distance = Utils::distance(varray[0].position, playerPositionForFOVRendering);
			multiplier = 1 - distance / (mainView.getSize().x / 2.0f);
			if (multiplier < 0) multiplier = 0;
			varray[0].color = sf::Color(255*multiplier, 255*multiplier, 128*multiplier, 50);
			varray[1].position = playerPositionForFOVRendering;
			varray[1].color = sf::Color(255 , 255 , 128 , 50);

			varray[2].position = sf::Vector2f(it->second.x, it->second.y);
			distance = Utils::distance(varray[2].position, playerPositionForFOVRendering);
			multiplier = 1 - distance / (mainView.getSize().x / 2.0f);
			if (multiplier < 0) multiplier = 0;
			varray[2].color = sf::Color(255 * multiplier, 255 * multiplier, 128 * multiplier, 50);
			_data->window.draw(varray);
		}
		sf::VertexArray lastTriangle(sf::Triangles, 3);

		lastTriangle[0].position = sf::Vector2f(intersectionPoints.begin()->second.x, intersectionPoints.begin()->second.y);
		distance = Utils::distance(lastTriangle[0].position, playerPositionForFOVRendering);
		multiplier = 1 - distance / (mainView.getSize().x / 2.0f);
		if (multiplier < 0) multiplier = 0;
		lastTriangle[0].color = sf::Color(255*multiplier, 255*multiplier, 128*multiplier, 50);

		lastTriangle[1].position = playerPositionForFOVRendering;
		lastTriangle[1].color = sf::Color(255, 255, 128, 50);

		lastTriangle[2].position = sf::Vector2f(std::prev(intersectionPoints.end(), 1)->second.x, std::prev(intersectionPoints.end(), 1)->second.y);
		distance = Utils::distance(lastTriangle[2].position, playerPositionForFOVRendering);
		multiplier = 1 - distance / (mainView.getSize().x / 2.0f);
		if (multiplier < 0) multiplier = 0;
		lastTriangle[2].color = sf::Color(255 * multiplier, 255 * multiplier, 128 * multiplier, 50);

		
		_data->window.draw(lastTriangle);

	}
	mutex.unlock();
	
	//DRAW ENTITIES//
	for (size_t i = 0; i < entities.size(); i++)
	{
		//check if in player field of view
		if (entityIsInPlayerFieldOfView(entities[i]->getPosition())) {
			this->_data->window.draw(*entities[i]);
		}
	}
	//DRAW PLAYER//
	player.draw(this->_data->window);


	//DRAW ENEMIES//
	healthBar.setFillColor(sf::Color::Red);
	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (entityIsInPlayerFieldOfView(enemies[i]->getPosition())) {
			enemies[i]->draw(this->_data->window);
			//DRAW HEALTHBAR//
			float pHealth = enemies[i]->getPercentHealth();
			healthBar.setSize(sf::Vector2f(HEALTHMAXSIZE.x*pHealth, HEALTHMAXSIZE.y));
			healthBar.setPosition(sf::Vector2f(enemies[i]->getPosition().x - healthBar.getSize().x / 2, enemies[i]->getPosition().y - 20));
			this->_data->window.draw(healthBar);
		}
	

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

void GameState::Pause()
{
	fovThreadStarted = false;
	fovThread.wait();
}

void GameState::Resume()
{
	fovThread.launch();
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

IntersectionPoint * GameState::getIntersection(Segment ray, Segment segment)
{
	float r_px = ray.a.x;
	float r_py = ray.a.y;
	float r_dx = ray.b.x - ray.a.x;
	float r_dy = ray.b.y - ray.a.y;

	float s_px = segment.a.x;
	float s_py = segment.a.y;
	float s_dx = segment.b.x - segment.a.x;
	float s_dy = segment.b.y - segment.a.y;

	float r_mag = sqrt(r_dx*r_dx + r_dy * r_dy);
	float s_mag = sqrt(s_dx*s_dx + s_dy * s_dy);

	if (r_dx / r_mag == s_dx / s_mag && r_dy / r_mag == s_dy / s_mag) {
		return NULL;
	}
	float T2 = (r_dx*(s_py - r_py) + r_dy * (r_px - s_px)) / (s_dx*r_dy - s_dy * r_dx);
	float T1 = (s_px + s_dx * T2 - r_px) / r_dx;

	if (T1 < 0)return NULL;
	if (T2 < 0 || T2>1) return NULL;

	return new IntersectionPoint(r_px + r_dx * T1, r_py + r_dy * T1, T1);

}

void GameState::fovOperator()
{
	fovThreadStarted = true;
	while (fovThreadStarted) {

		std::vector<Segment> segments;
		std::vector<sf::Vector2f> points;
		sf::Vector2f playerPositionAtThreadStart = player.getPosition();
		for (int i = 0; i < MAP_WIDTH; i++) {
			for (int j = 0; j < MAP_HEIGHT; j++) {

				float x = i * BLOCKSIZE;
				float y = j * BLOCKSIZE;
				if (!_data->map.isEntityOnPathCell(sf::Vector2f(x, y))) {
					if (x >= playerPositionAtThreadStart.x - mainView.getSize().x / 2.0f &&
						x <= playerPositionAtThreadStart.x + mainView.getSize().x / 2.0f &&
						y <= playerPositionAtThreadStart.y + mainView.getSize().y / 2.0f &&
						y >= playerPositionAtThreadStart.y - mainView.getSize().y / 2.0f)
					{
						points.push_back(sf::Vector2f(x, y));
						points.push_back(sf::Vector2f(x + BLOCKSIZE, y));
						points.push_back(sf::Vector2f(x, y + BLOCKSIZE));
						points.push_back(sf::Vector2f(x + BLOCKSIZE, y + BLOCKSIZE));

						segments.push_back(Segment(sf::Vector2f(x, y), sf::Vector2f(x + BLOCKSIZE, y)));
						segments.push_back(Segment(sf::Vector2f(x + BLOCKSIZE, y), sf::Vector2f(x + BLOCKSIZE, y + BLOCKSIZE)));
						segments.push_back(Segment(sf::Vector2f(x + BLOCKSIZE, y + BLOCKSIZE), sf::Vector2f(x, y + BLOCKSIZE)));
						segments.push_back(Segment(sf::Vector2f(x, y + BLOCKSIZE), sf::Vector2f(x, y)));
					}
				}

			}
		}

		//create view bounds segments
		float x_minus = playerPositionAtThreadStart.x - mainView.getSize().x / 2.0f >= 0 ? playerPositionAtThreadStart.x - mainView.getSize().x / 2.0f : 0;
		float y_minus = playerPositionAtThreadStart.y - mainView.getSize().y / 2.0f >= 0 ? playerPositionAtThreadStart.y - mainView.getSize().y / 2.0f : 0;
		float x_plus = playerPositionAtThreadStart.x + mainView.getSize().x / 2.0f  <= MAP_WIDTH*BLOCKSIZE ? playerPositionAtThreadStart.x + mainView.getSize().x / 2.0f : MAP_WIDTH * BLOCKSIZE;
		float y_plus = playerPositionAtThreadStart.y + mainView.getSize().y / 2.0f  <= MAP_HEIGHT*BLOCKSIZE ? playerPositionAtThreadStart.y + mainView.getSize().y / 2.0f : MAP_HEIGHT * BLOCKSIZE;
		sf::Vector2f topLeft(x_minus , y_minus);
		sf::Vector2f topRight(x_plus, y_minus);
		sf::Vector2f botLeft(x_minus, y_plus);
		sf::Vector2f botRight(x_plus, y_plus);

		points.push_back(topLeft);
		points.push_back(topRight);
		points.push_back(botLeft);
		points.push_back(botRight);

		segments.push_back(Segment(topLeft, topRight));
		segments.push_back(Segment(topRight, botRight));
		segments.push_back(Segment(botRight, botLeft));
		segments.push_back(Segment(botLeft, topLeft));


		std::set<sf::Vector2f, Vector2fComparator> uniquePoints;
		unsigned int size = points.size();
		for (unsigned i = 0; i < size; i++) {
			uniquePoints.insert(points[i]);
		}
		points.assign(uniquePoints.begin(), uniquePoints.end());

		std::vector<float> uniqueAngles;
		for (int i = 0; i < points.size(); i++) {
			float angle = atan2(points[i].y - playerPositionAtThreadStart.y, points[i].x - playerPositionAtThreadStart.x);
			uniqueAngles.push_back(angle - 0.001f);
			uniqueAngles.push_back(angle);
			uniqueAngles.push_back(angle + 0.001f);
		}

		std::sort(uniqueAngles.begin(), uniqueAngles.end());
		std::map<float, IntersectionPoint> intersects;
		for (int i = 0; i < uniqueAngles.size(); i++) {
			float dx = cos(uniqueAngles[i]);
			float dy = sin(uniqueAngles[i]);

			Segment ray(sf::Vector2f(playerPositionAtThreadStart.x, playerPositionAtThreadStart.y), sf::Vector2f(playerPositionAtThreadStart.x + dx, playerPositionAtThreadStart.y + dy));

			IntersectionPoint* closestIntersect = NULL;
			for (int j = 0; j < segments.size(); j++) {
				IntersectionPoint*intersect = getIntersection(ray, segments[j]);
				if (intersect == NULL) continue;
				if (closestIntersect == NULL || (intersect->coeff < closestIntersect->coeff)) {
					delete closestIntersect;
					closestIntersect = intersect;
				}
				else {
					delete intersect;
				}
			}
			intersects[uniqueAngles[i]] = *closestIntersect;
			delete closestIntersect;
		}

		std::map<float, IntersectionPoint>::iterator it;
		mutex.lock();
		intersectionPoints.clear();
		for (it = intersects.begin(); it != intersects.end();it++) {
			intersectionPoints.emplace(*it);
		}
		playerPositionForFOVRendering = playerPositionAtThreadStart;
		mutex.unlock();

	}
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

bool GameState::entityIsInPlayerFieldOfView(sf::Vector2f entity)
{
	float distance = Utils::distance(entity, player.getPosition());
	if (distance < mainView.getSize().x/2.0f) {

		sf::Vector2f p1 = entity;
		sf::Vector2f p2 = player.getPosition();
		sf::Vector2f p3 = entity;

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

