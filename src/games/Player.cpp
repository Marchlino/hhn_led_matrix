
#include "Player.hpp"


Player::Player(std::string name, std::string id) {

    this->_name = name;
    this->_id = id;

}

std::string Player::getID() {

    return this->_id;

}
