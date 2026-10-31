#include "Pong.hpp"

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

    this->players.push_back(p);
    float new_angle = 360. / this->players.size();
    return true;

}

bool Pong::removePlayer(Player* ) {

    return true;

}

bool Pong::updatePlayer(Player* , std::vector<int> ) {

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

    return true;

}
