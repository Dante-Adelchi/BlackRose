//// Created by Elena Faillace on 25/06/2018.//#include "../include/Entity.h"void Entity::setVelocity(sf::Vector2f velocity) {    mVelocity = velocity;}void Entity::setVelocity(float velX, float velY) {    mVelocity.x = velX;    mVelocity.y = velY;}sf::Vector2f Entity::getVelocity() const {    return mVelocity;}