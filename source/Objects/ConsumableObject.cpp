//// Created by Elena Faillace on 07/07/2018.//#include "../../include/Objects/ConsumableObject.h"int ConsumableObject::getQuantity() const {    return quantity;}void ConsumableObject::update() {    rect.setPosition(position);    sprite.setPosition(rect.getPosition());}