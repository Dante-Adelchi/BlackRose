//

// Created by Elena Faillace on 07/07/2018.
//

#include "../include/World.h"


World::World(std::shared_ptr <sf::RenderWindow> window, const TextureHolder &textures): window(window), textures
        (textures), player(new PlayerCharacter(PlayerCharacter::SubType::blondHero, textures,
                window->getSize())), rangedWeapon(new RangedWeapon(textures,RangedWeapon::Type::energyShooter)),
                           map(new ProceduralMap(textures, Tile::BackGroundType::baseFloor)) {
    int x,y;
    do{
        x = generateRandom(24);
        y = generateRandom(24);
    } while (map->tileMap[x*25+y]->backGround != Tile::labFloor);
    player->rect.setPosition(x*32+16,y*32+16);

    createEnemies();
    createWeapons();

}


void World::createEnemies() {

    for(int i=0; i<5; i++) {
        std::shared_ptr<Enemy> enemy = enemyFactory.createEnemy(Enemy::SubType::robotRed, textures,
                window->getSize());
        int x,y;
        do{
            x = generateRandom(24);
            y = generateRandom(24);
        } while (map->tileMap[x*25+y]->backGround != Tile::labFloor);
        enemy->rect.setPosition(x*32+16,y*32+16);

        enemyArray.emplace_back(enemy);
    }
}

void World::createWeapons() {
    rangedWeapon->addStuff(50);
    int x,y;
    do{
        x = generateRandom(24);
        y = generateRandom(24);
    } while (map->tileMap[x*25+y]->backGround != Tile::labFloor);
    rangedWeapon->setPosition(sf::Vector2f(x*32+16,y*32+16));

    rangedWeapon->update();
    collectableObject.emplace_back(rangedWeapon);
}

void World::update(sf::Time dt) {

    player->update(dt);
    updateEnemies();
    updateProjectiles();
    updateObjects();
    checkCollision();
    updateTextureDisplayed();

}

void World::updateTextureDisplayed() {
    int counter = 0;
    for ( auto iter = textureDisplayArray.begin(); iter != textureDisplayArray.end(); iter++ ) {
        textureDisplayArray[ counter ]->update();
        counter++;
    }
}

void World::updateObjects() {
    if(!collectableObject.empty()) {
        int counter = 0;
        int deleted = -1;
        for ( auto iter = collectableObject.begin(); iter != collectableObject.end(); iter++ ) {
            collectableObject[ counter ]->update();

            if ( collectableObject[ counter ]->counterLifeTime < 0 ) {
                deleted = counter;
            }
            counter++;
        }
        if(deleted>=0)
            collectableObject.erase(collectableObject.begin() + deleted);
    }
}

void World::checkCollision() {

    collisionPlayerProjectilesOnObjects();
    collisionPlayerEnemy();
    //Wall
    collisionWithMap();
}

void World::collisionWithMap() {
    int counterMap = 0;
    //iter on tiles
    for(auto iterMap = map->tileMap.begin(); iterMap != map->tileMap.end(); iterMap++) {

        //if tile is a wall
        if(map->tileMap[counterMap]->backGround == Tile::BackGroundType::metalWall) {
            int counterEnemy = 0;
            //iter on enemies
            for(auto iterEnemy = enemyArray.begin(); iterEnemy != enemyArray.end(); iterEnemy++) {

                //if enemy touches a wall
                std::shared_ptr<Enemy> enemy = enemyArray[counterEnemy];
                if(enemy->rect.getGlobalBounds().intersects(map->tileMap[counterMap]->rect.getGlobalBounds())) {

                    //enemy bounce on walls
                    if(enemy->isMovingUp) {
                        enemy->changeDirection();
                        enemy->setPosition(0,1);
                    } else if(enemy->isMovingDown) {
                        enemy->changeDirection();
                        enemy->setPosition(0,-1);
                    } else if(enemy->isMovingLeft) {
                        enemy->changeDirection();
                        enemy->setPosition(1, 0);
                    } else if(enemy->isMovingRight) {
                        enemy->changeDirection();
                        enemy->setPosition(-1, 0);
                    }
                }
                counterEnemy++;
            }
            //check player
            if(player->rect.getGlobalBounds().intersects(map->tileMap[counterMap]->rect.getGlobalBounds())) {

                if(player->isMovingUp && player->isMovingLeft) {
                    player->setPosition(2,2);
                } else if(player->isMovingUp && player->isMovingRight) {
                    player->setPosition(-2,2);
                } else if(player->isMovingDown && player->isMovingLeft) {
                    player->setPosition(2,-2);
                } else if(player->isMovingDown && player->isMovingRight) {
                    player->setPosition(-2,-2);
                } else if(player->isMovingUp) {
                    player->setPosition(0,2);
                } else if(player->isMovingDown) {
                    player->setPosition(0,-2);
                } else if(player->isMovingLeft) {
                    player->setPosition(2,0);
                } else if(player->isMovingRight) {
                    player->setPosition(-2,0);
                }
            }
            //check Projectiles
            if(!projectilePlayerArray.empty()) {
                int counterProjectiles = 0;
                //go through all projectiles
                for ( auto iterProjectile = projectilePlayerArray.begin(); iterProjectile != projectilePlayerArray.end(); iterProjectile++ ) {

                    if(projectilePlayerArray[counterProjectiles]->rect.getGlobalBounds().intersects(map->tileMap[counterMap]->rect.getGlobalBounds())) {
                        projectilePlayerArray[counterProjectiles]->active = false;
                    }
                    counterProjectiles++;
                }
            }
            //TODO itera anche sui proiettili nemici
        }
        counterMap++;
    }
}

void World::collisionPlayerEnemy() {
    //Enemies and Player
    int counterEnemy = 0;
    for ( auto iter = enemyArray.begin(); iter != enemyArray.end(); iter++ ) {
        if ( player->rect.getGlobalBounds().intersects(enemyArray[counterEnemy]->rect.getGlobalBounds())) {
            player->display();
            player->hp -= enemyArray[counterEnemy]->getAttackDamage();
        }
        counterEnemy++;
    }
}

void World::collisionPlayerProjectilesOnObjects() {
    if(!projectilePlayerArray.empty() && !enemyArray.empty()) {
        int counterProjectiles = 0;
        //go through all projectiles
        for ( auto iterProjectile = projectilePlayerArray.begin(); iterProjectile != projectilePlayerArray.end(); iterProjectile++ ) {

            int counterEnemy = 0;
            //go through all enemies
            for ( auto iterEnemy = enemyArray.begin(); iterEnemy != enemyArray.end(); iterEnemy++ ) {
                if ( projectilePlayerArray[ counterProjectiles ]->rect.getGlobalBounds().
                        intersects(enemyArray[ counterEnemy ]->rect.getGlobalBounds())) {

                    std::cout << "Collision!" << std::endl;
                    enemyArray[counterEnemy]->display();
                    enemyArray[counterEnemy]->hp -= projectilePlayerArray[counterProjectiles]->attackDamage;
                    projectilePlayerArray[counterProjectiles]->active = false;
                }
                counterEnemy++;
            }
            counterProjectiles++;
        }
    }
}

void World::updateEnemies() {
    if(!enemyArray.empty()) {
        int counter = 0;
        int deleted = -1;
        for ( auto iter = enemyArray.begin(); iter != enemyArray.end(); iter++ ) {
            enemyArray[ counter ]->update();

            //if it use his weapon it adds it to the enemyProjectiles
            auto shooter = std::dynamic_pointer_cast<RobotShooter>(enemyArray[counter]);
            if( shooter != nullptr && shooter->attackAvailable) {
                projectileEnemyArray.emplace_back(std::make_shared<Projectile>(textures));
                projectileEnemyArray.back()->setPosition(shooter->rect.getPosition(),shooter->direction);
                projectileEnemyArray.back()->range = shooter->weapon->range;
                //perché never used ??
                shooter->attackAvailable = false;
            }

            if ( !enemyArray[ counter ]->active ) {
                deleted = counter;
            }
            counter++;
        }
        if(deleted>=0)
            enemyArray.erase(enemyArray.begin() + deleted);
    }
}

void World::updateProjectiles() {
    //TODO itera anche sui projectiles nemici
    if(!projectilePlayerArray.empty()) {
        int counter = 0;
        int deleted = -1;
        for ( auto iter = projectilePlayerArray.begin(); iter != projectilePlayerArray.end(); iter++ ) {
            projectilePlayerArray[ counter ]->update();

            if ( !projectilePlayerArray[ counter ]->active ) {
                deleted = counter;
            }
            counter++;
        }
        if(deleted>=0)
            projectilePlayerArray.erase(projectilePlayerArray.begin() + deleted);
    }
    if(!projectileEnemyArray.empty()) {
        int counter = 0;
        int deleted = -1;
        for ( auto iter = projectileEnemyArray.begin(); iter != projectileEnemyArray.end(); iter++ ) {
            projectileEnemyArray[ counter ]->update();

            if ( !projectileEnemyArray[ counter ]->active ) {
                deleted = counter;
            }
            counter++;
        }
        if(deleted>=0)
            projectileEnemyArray.erase(projectileEnemyArray.begin() + deleted);
    }
}

void World::draw() {
    window->setView(window->getDefaultView());
    drawMap();

    drawProjectiles();
    drawObjects();
    drawEnemies();
    drawPlayer();

    drawTextDisplayed();
}

void World::drawPlayer() {
    if(!player->dead) {
        window->draw(player->getSprite());
        if(player->barDisplayed) {
            window->draw(player->bar);
            window->draw(player->lifeBar);
        }
    }
}

void World::drawTextDisplayed() {
    if(!textureDisplayArray.empty()) {
        int counter = 0;
        for ( auto iter = textureDisplayArray.begin(); iter != textureDisplayArray.end(); iter++ ) {
                window->draw(textureDisplayArray[ counter ]->text);
            counter++;
        }
    }
}

void World::drawObjects() {
    if(!collectableObject.empty()) {
        int counter = 0;
        for ( auto iter = collectableObject.begin(); iter != collectableObject.end(); iter++ ) {
            if(!collectableObject[counter]->equipped)
                window->draw(collectableObject[ counter ]->getSprite());
            counter++;
        }
    }
}

void World::drawMap() {
    for(int i=0; i<25*25; i++) {
        window->draw(map->tileMap[i]->sprite);
    }
}

void World::drawEnemies() {
    if(!enemyArray.empty()) {
        int counter = 0;
        for ( auto iter = enemyArray.begin(); iter != enemyArray.end(); iter++ ) {
            window->draw(enemyArray[counter]->getSprite());
            if(enemyArray[counter]->barDisplayed) {
                window->draw(enemyArray[counter]->bar);
                window->draw(enemyArray[counter]->lifeBar);
            }
            counter++;
        }
    }
}

void World::handlePlayerInput(sf::Keyboard::Key key, bool isPressed, sf::Clock& shootingClock) {
    if (key == sf::Keyboard::W || key == sf::Keyboard::Up)
        player->isMovingUp = isPressed;
    else if (key == sf::Keyboard::S || key == sf::Keyboard::Down)
        player->isMovingDown = isPressed;
    else if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
        player->isMovingLeft = isPressed;
    else if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
        player->isMovingRight = isPressed;
    else if (key == sf::Keyboard::Q)
        checkCollection();
    else if (key == sf::Keyboard::Space && isPressed) {
        if(shootingClock.getElapsedTime().asSeconds() >= 0.2) {
            useWeapon();
            shootingClock.restart();
        }
    }
    else if (key == sf::Keyboard::Escape && isPressed)
        window->close();
}

void World::checkCollection() {
    if(!collectableObject.empty()) {
        int counterObject = 0;
        //go through all objects
        for ( auto iter = collectableObject.begin(); iter != collectableObject.end(); iter++ ) {
            if ( collectableObject[ counterObject ]->rect.getGlobalBounds().intersects(player->rect.getGlobalBounds())) {
                std::cout << "collecting object" << std::endl;
                player->interactWithObject(collectableObject[ counterObject ]);
            }
            counterObject++;
        }
    }
}

//gets the projectile back in the array of the world and sets the right position
void World::useWeapon() {
    if( player->useWeapon()) {
        projectilePlayerArray.emplace_back(std::make_shared<Projectile>(textures));
        projectilePlayerArray.back()->setPosition(player->rect.getPosition(), player->direction);
        projectilePlayerArray.back()->range = player->weapon->range;
    } else
        std::cout<<"non puo sparare "<<std::endl;
}

void World::drawProjectiles() {
    if(!projectilePlayerArray.empty()) {
        int counter = 0;
        for ( auto iter = projectilePlayerArray.begin(); iter != projectilePlayerArray.end(); iter++ ) {
            window->draw(projectilePlayerArray[ counter ]->getSprite());
            counter++;
        }
    }
    if(!projectileEnemyArray.empty()) {
        int counter = 0;
        for ( auto iter = projectileEnemyArray.begin(); iter != projectileEnemyArray.end(); iter++ ) {
            window->draw(projectileEnemyArray[ counter ]->getSprite());
            counter++;
        }
    }
}
