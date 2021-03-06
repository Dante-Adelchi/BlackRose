//
// Created by Elena Faillace on 17/09/2018.
//

#ifndef BLACKROSE_SEEKSTRATEGY_H
#define BLACKROSE_SEEKSTRATEGY_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Strategy.h"

class SeekStrategy: public Strategy {
public:
    enum Direction {
        up, down, left, right
    };

    SeekStrategy(sf::Vector2u windowSize);

    void update(bool &isMovingUp, bool &isMovingDown, bool &isMovingLeft, bool &isMovingRight, sf::RectangleShape&
    rect) override;

    void animation(bool &isMovingUp, bool &isMovingDown, bool &isMovingLeft, bool &isMovingRight, bool &
    delayWalk, bool &delayMoreWalk, int &counterWalk) override;

    //int changeDirectionTime, counterDirection = 0;
    Direction direction;
    Type type;
    sf::Vector2f playerPosition;

private:
    //void changeDirection(bool &isMovingUp, bool &isMovingDown, bool &isMovingLeft, bool &isMovingRight);


    float speed;
    sf::Vector2u windowSize;

};


#endif //BLACKROSE_SEEKSTRATEGY_H
