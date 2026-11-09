#include "Pong.hpp"
#include "../network/Player.hpp"

int Pong::getPlayerIndex(Player* p) {

    for (int i = 0; i < this->players.size(); ++i) {

        // Found the player...
        if (this->players.at(i)->getID() == p->getID()) {

            return i;

        }

    }

    // Player not in game
    return -1;

}

bool Pong::addPlayer(Player* p) {

    if (getPlayerIndex(p) != -1) return false;
    this->players.push_back(p);
    float new_angle = 360. / this->players.size();
    return true;

}

bool Pong::removePlayer(Player* p) {

    int index = getPlayerIndex(p);
    if (index == -1) return false;
    this->players.erase(this->players.begin() + index);
    return true;

}

bool Pong::updatePlayer(Player* , std::vector<Value> ) {

    return true;

}

bool Pong::isRunning() {

    return true;

}

bool Pong::render(Image* i) {

    return true;

}

bool Pong::update() {

    return true;

}

bool Pong::reset() {

    this->players.clear();
    this->scores.clear();
    return true;

}


int Pong::getControllerType() {

    return 1;

}


std::string Pong::getName() {

    return "Pong";

}
