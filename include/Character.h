//
// Created by Elena Faillace on 07/07/2018.
//

#ifndef BLACKROSE_CHARACTER_H
#define BLACKROSE_CHARACTER_H


#include <SFML/System.hpp>
#include "Inventory.h"
#include "Weapon.h"
#include "Shield.h"
#include "Entity.h"

class Character: public Entity {
public:
    Character();
    virtual void move();
    virtual void fight();
    //basic interaction with the world, it will be associated with a key
    virtual bool use();
    virtual void die();

protected:
    int hp;
    int resistance;
    int speed;
    sf::Vector2f position;
    Inventory inventory;
};


#endif //BLACKROSE_CHARACTER_H
