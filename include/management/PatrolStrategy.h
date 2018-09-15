//
// Created by Elena Faillace on 14/09/2018.
//

#ifndef BLACKROSE_PATROLSTRATEGY_H
#define BLACKROSE_PATROLSTRATEGY_H

#include "Strategy.h"
#include "../Random.h"

class PatrolStrategy : public Strategy {
public:
    enum Direction {
        up, down, left, right
    };

    PatrolStrategy(sf::Vector2u windowSize);

    //change direction every x frames
    void update(bool &isMovingUp, bool &isMovingDown, bool &isMovingLeft, bool &isMovingRight, sf::RectangleShape&
    rect) override;

    void animation(bool &isMovingUp, bool &isMovingDown, bool &isMovingLeft, bool &isMovingRight, bool &
    delayWalk, bool &delayMoreWalk, int &counterWalk) override;

    int changeDirectionTime, counterDirection = 0;
    Direction direction;

private:
    void changeDirection(bool &isMovingUp, bool &isMovingDown, bool &isMovingLeft, bool &isMovingRight);

    void setPositionMovement(bool& isMovingUp,bool& isMovingDown,bool& isMovingLeft,bool& isMovingRight, const
    sf::Vector2f &movement, sf::RectangleShape& rect);

    int speed;
    sf::Vector2u windowSize;


};


#endif //BLACKROSE_PATROLSTRATEGY_H