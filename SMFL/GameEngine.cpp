#include "GameEngine.h"


GameEngine GameEngine::m_instance = GameEngine();


GameEngine::GameEngine() : mapManager(MAPSIZE_X,MAPSIZE_Y,BLOCKSIZE)
{

	map.load(sf::Vector2u(BLOCKSIZE, BLOCKSIZE), mapManager.getMatrix(), MAPSIZE_X,MAPSIZE_Y);

	window.create(sf::VideoMode(1280, 720), "Man vs Men ");
	mainView.setSize(sf::Vector2f(1280, 720));
	window.setView(mainView);
	mainView.zoom(1.5f);

	font.loadFromFile(ARIAL_FONT);
	text.setFont(font);
	text2.setFont(font);
	text2.setPosition(650, 0);
	textFps.setFont(font);
	textFps.setPosition(0, 50);
	ammoInfo.setFont(font);
	ammoInfo.setScale(sf::Vector2f(0.75f, 0.75f));

	healthBar.setSize(sf::Vector2f(100, 10));
	healthBar.setFillColor(sf::Color::Green);

	gameover.setTexture(imageManager.get_image("images/gameover.png"));

	focus = true;



}

GameEngine & GameEngine::getInstance()
{
	return m_instance;
}

void GameEngine::spawnEnemy() {
	Enemy* enemy;
	enemies.push_back(new Enemy());
	enemy = enemies[enemies.size() - 1];
	enemy->setHealth(100);
	enemy->setPosition(mapManager.getRandomPathPointInOrOutRange(500,false,player.getPosition()));
	enemy->changeWeapon(new Weapon(Weapon::WeaponType::Rifle, 100));
	enemy->startMoving();
	enemy->setViewFieldRadius(VIEWFIELDRADIUS);
	enemy->setTargetPlayer(&player);
	enemy->setTargetPoint(enemy->getPosition());
}



void GameEngine::InitGame() {

	srand((unsigned int)time(NULL));
	player = Player();
	player.setPosition(sf::Vector2f(mapManager.getFirstPathCellTopLeft()));
	player.changeWeapon(new Weapon(Weapon::WeaponType::Shotgun, 120));

	enemies.clear();
	for (size_t i = 0; i < NUMBER_ENEMIES; i++) {
		spawnEnemy();
	}
	for (size_t i = 0; i < 3; i++)
	{

		spawnWeapon(mapManager.getRandomPathPoint(),Weapon(Weapon::WeaponType::Rifle,50));
	}
	for (size_t i = 0; i < 3; i++)
	{
		entities.push_back(new MedikitEntity(mapManager.getRandomPathPoint()));
	}

	Running = true;
	displayGameOver = false;
	displayMenu = false;
}

void GameEngine::InputManagement() {
	if (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
		
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::LostFocus)
				focus = false;
			if (event.type == sf::Event::GainedFocus)
				focus = true;
		}
		if (focus) {
			bool Escape = 0, Pause = 0;
			bool Zoom = 0, Dezoom = 0;
			if (Keyboard::isKeyPressed(Keyboard::P)) Pause = 1;
			if (Keyboard::isKeyPressed(Keyboard::Num4)) Zoom = 1;
			if (Keyboard::isKeyPressed(Keyboard::Num2)) Dezoom = 1;

			if (Escape) {
				window.close();
				Running = false;
			}
			if (Zoom) {
				if (zoomhold == false) {
					mainView.zoom(2.0f);
					zoomhold = true;
				}
			}
			else if (Dezoom) {
				if (zoomhold == false) {
					mainView.zoom(0.5f);
					zoomhold = true;
				}
			}
			else {
				zoomhold = false;
			}
			if (managerMenu()) {
				switch (menu.getSelectedItem()) {
				case GameMenu::Items::Resume:
					displayMenu = false;
					uphold = false;
					downhold = false;
					enterhold = false;
					break;
				case GameMenu::Items::Restart:
					InitGame();
					uphold = false;
					downhold = false;
					enterhold = false;
					break;
				case GameMenu::Items::Quit:
					window.close();
					Running = false;
					break;
				}
			}

		}
		
	}
}

void GameEngine::spawnWeapon(sf::Vector2f position, Weapon& weapon)
{
	entities.push_back(new WeaponEntity(position, weapon));

}

void GameEngine::spawnEnemy(int health, Weapon::WeaponType weaponType, int ammo) {
	spawnEnemy();
	Enemy* enemy;
	enemy = enemies[enemies.size() - 1];
	enemy->setHealth(health);
	enemy->changeWeapon(new Weapon(weaponType, ammo));

}

bool GameEngine::managerMenu() {
	/*MENU*/
	bool Escape = 0;
	bool Up = 0, Down = 0, Enter = 0;
	if (Keyboard::isKeyPressed(Keyboard::Escape)) Escape = 1;
	if (Keyboard::isKeyPressed(Keyboard::Up)) Up = 1;
	if (Keyboard::isKeyPressed(Keyboard::Down)) Down = 1;
	if (Keyboard::isKeyPressed(Keyboard::Return)) Enter = 1;
	

	if (displayMenu) {
		if (Up && !uphold) {
			menu.SelectItemUp();
			uphold = true;
		}
		else if (!Up) {
			uphold = false;
		}
		if (Down && !downhold) {
			menu.SelectItemDown();
			downhold = true;
		}
		else if (!Down){
			downhold = false;
		}
		displayMenu = true;
		if (Enter && !enterhold) {
			enterhold = true;
			return true;
		}
	}
	if (Escape) {
		displayMenu = true;
	}
	return false;
}
bool GameEngine::IsRunning() {
	return Running;
}
void GameEngine::Update() {

	dt = clock.restart().asSeconds();
	float finalMultiplier = dt*multiplier;

	if (!displayMenu) {

		if (!player.isDead()) {
			playerUpdate(finalMultiplier);
			playerCollideEntities(finalMultiplier);
			playerBulletsCollisionEnemies(finalMultiplier);
			enemiesUpdate(finalMultiplier);
			entitiesUpdate(finalMultiplier);
		}
		else {
			displayGameOver = true;
		}
		
	}
}
void GameEngine::playerBulletsCollisionEnemies(float multiplier) {
	int k = 0;
	std::vector<int> indexBulletToErase;
	for each (Bullet var in player.getBullets())
	{

		sf::RectangleShape r;
		r.setPosition(var.getPosition());
		r.setRotation(-var.getRotation());
		r.setSize(sf::Vector2f(BULLETSHAPE_WIDTH, BULLETSHAPE_HEIGHT));
		if (!mapManager.isEntityOnPathCell(var.getPosition())) {
			indexBulletToErase.push_back(k);
		}
		else {
			for (size_t j = 0; j < enemies.size(); j++)
			{
				sf::RectangleShape enemyMinBounds = enemies[j]->getMinBounds();

				if (Utils::rotatedCollisionDetection(r, enemyMinBounds)) {
					enemies[j]->hurt(var.getDamages());
					if (!(std::find(indexBulletToErase.begin(), indexBulletToErase.end(), k) != indexBulletToErase.end())) {
						indexBulletToErase.push_back(k);
					}

				}
			}
		}

		k++;
	}
	for each (int index in indexBulletToErase) {
		if (index < player.getBullets().size()) {
			player.getBullets().erase(player.getBullets().begin() + index);
		}
	}
	std::vector<int> indexEnemyToErase;
	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->isDead()) {
			indexEnemyToErase.push_back(i);
		}
	}

	for (size_t i = 0; i < indexEnemyToErase.size(); i++)
	{
		enemies.erase(enemies.begin() + indexEnemyToErase[i]);
		spawnEnemy();
	}
}

void GameEngine::playerCollideEntities(float mulitplier)
{
	sf::RectangleShape playerMinBounds = player.getMinBounds();

	vector<int> indexToDelete;
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (Utils::rotatedCollisionDetection(playerMinBounds, entities[i]->getMinBounds()))
		{
			if (entities[i]->getPickTimeout() == 0) {
				Entity *e = player.pickAndDropEntity(entities[i]);
				if (entities[i] == e) {
					//nothing
				}
				else if (e != NULL) {
					delete entities[i];
					if (WeaponEntity* we = dynamic_cast<WeaponEntity*>(e)) {
						entities[i] = new WeaponEntity(*we);
					}
					else if (MedikitEntity* me = dynamic_cast<MedikitEntity*>(e)) {
						entities[i] = new MedikitEntity(*me);
					}
				}
				else {
					indexToDelete.push_back(i);
				}
			}

		}
	}

	for (size_t i = 0; i < indexToDelete.size(); i++)
	{
		delete entities[indexToDelete[i]];
		entities.erase(entities.begin() + indexToDelete[i]);
	}
}

void GameEngine::entitiesUpdate(float multiplier)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->update(multiplier);
	}
}

void GameEngine::playerUpdate(float multiplier) {
	sf::Vector2f v = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	float deltaX = v.x - window.getView().getCenter().x;
	float deltaY = v.y - window.getView().getCenter().y;

	float angleMouse = Utils::radians_to_degrees(atan2(deltaY, deltaX));

	player.setRotation(-angleMouse);
	player.update(multiplier);
	if (!mapManager.isEntityOnPathCell(player.getPosition())) {
		player.move(Character::Direction::Back, multiplier);
	}
}

void GameEngine::enemiesUpdate(float multiplier) {
	sf::RectangleShape playerMinBounds = player.getMinBounds();
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->update(multiplier);
		sf::RectangleShape enemyMinBounds = enemies[i]->getMinBounds();
		if (Utils::circleCollisionDetection(player.getPosition(), enemies[i]->getPosition(),CHARACTER_RADIUS,CHARACTER_RADIUS)) {
			player.move(Character::Direction::Back,multiplier);
		}
		for (size_t j = 0; j < enemies[i]->getBullets().size(); j++)
		{
			Bullet bullet = enemies[i]->getBullets()[j];
			sf::RectangleShape r;
			r.setPosition(bullet.getPosition());
			r.setRotation(-bullet.getRotation());
			r.setSize(sf::Vector2f(BULLETSHAPE_WIDTH, BULLETSHAPE_HEIGHT));
			if (Utils::rotatedCollisionDetection(r, playerMinBounds)) {
				player.hurt(bullet.getDamages());
				enemies[i]->getBullets().erase(enemies[i]->getBullets().begin() + j);
			}
		}
	}
}

void GameEngine::Draw()
{
	//CLEAR AND SET WINDOW//
	mainView.setCenter(player.getPosition());
	window.clear();
	window.setView(mainView);

	count++;
	if (count == 100) {
		text.setString(std::to_string(1 / dt));
		count = 0;
	}
	sf::Text healthStatus;
	healthStatus.setFont(font);
	healthStatus.setScale(sf::Vector2f(0.75f, 0.75f));

	
	//DRAW MAP//
	window.draw(map);

	for (size_t i = 0; i < entities.size(); i++)
	{
		window.draw(*entities[i]);
	}
	player.draw(window);
	healthBar.setFillColor(Color::Red);
	

	for (size_t i = 0; i < enemies.size();i++)
	{
		if (enemies[i]->targetInViewField(player.getPosition())) {
			enemies[i]->getSprite().setColor(sf::Color::Green);
		}
		else {
			enemies[i]->getSprite().setColor(sf::Color::Red);
		}
		enemies[i]->draw(window);

		//DRAW HEALTHBAR AND WEAPON INFOS//
		float pHealth = enemies[i]->getPourcentHealth();
		healthBar.setSize(sf::Vector2f(HEALTHMAXSIZE.x*pHealth,HEALTHMAXSIZE.y));
		healthBar.setPosition(sf::Vector2f(enemies[i]->getPosition().x - healthBar.getSize().x / 2, enemies[i]->getPosition().y - 20));
		string s = std::to_string(enemies[i]->getPourcentHealth())+ " " + std::to_string(enemies[i]->getHealth());
		healthStatus.setString(s);
		healthStatus.setPosition(sf::Vector2f(enemies[i]->getPosition().x - healthBar.getSize().x / 2, enemies[i]->getPosition().y - 40));
		window.draw(healthStatus);
		window.draw(healthBar);
	}
	healthBar.setFillColor(Color::Green);
	float ammoInfoTextSize = ammoInfo.findCharacterPos(player.getWeaponInfo().size() - 1).x - ammoInfo.findCharacterPos(0).x;
	ammoInfo.setString(player.getWeaponInfo());
	ammoInfo.setPosition(sf::Vector2f(player.getPosition().x-ammoInfoTextSize/2, player.getPosition().y - 30));
	healthBar.setSize(sf::Vector2f(HEALTHMAXSIZE.x*player.getPourcentHealth(), HEALTHMAXSIZE.y));
	healthBar.setPosition(sf::Vector2f(player.getPosition().x - healthBar.getSize().x/2, player.getPosition().y - 50));
	window.draw(ammoInfo);
	window.draw(healthBar);
	
	window.setView(window.getDefaultView());
	//DRAW GAME INFOS//
	window.draw(text);
	window.draw(text2);
	window.draw(textFps);
	
	//MANAGE GAMEOVER SCREEN//
	if (displayGameOver) {
		
		gameover.setPosition(window.getSize().x/2 - gameover.getTexture()->getSize().x / 2, window.getSize().y/2 - gameover.getTexture()->getSize().y / 2);
		window.draw(gameover);
	}

	//MANAGE MENU//
	if (displayMenu) {
		menu.Draw(window);
	}

	window.display();
	
}



GameEngine::~GameEngine()
{
}
