//// Created by Elena Faillace on 07/07/2018.//#ifndef BLACKROSE_ENTITY_H#define BLACKROSE_ENTITY_H#include <SFML/Graphics.hpp>class Entity {public:    enum Direction {        up,        down,        left,        right    };    virtual const sf::Sprite &getSprite();    virtual ~Entity() = 0;    //for collision    sf::RectangleShape rect;    sf::Texture texture;    Direction direction;    sf::Sprite sprite;    sf::Text text;};#endif //BLACKROSE_ENTITY_H