//// Created by Elena Faillace on 25/06/2018.//#ifndef BLACKROSE_ENTI_H#define BLACKROSE_ENTI_H#include <SFML/System.hpp>class Entity {public:    void setVelocity(sf::Vector2f velocity);    void setVelocity(float velX, float velY);    sf::Vector2f getVelocity() const;private:    sf::Vector2f mVelocity;};#endif //BLACKROSE_ENTITY_H