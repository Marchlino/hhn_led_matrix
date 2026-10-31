#pragma once
#include "Game.hpp"

class Pong: public Game {

private:

    std::vector<Player*> players;
    std::vector<int> scores;
    float player_angle;
    float min_angle;

    int getPlayerIndex(Player* );

public:

    Pong() = default;
    bool addPlayer(Player* ) override;
    bool removePlayer(Player* )override;
    bool updatePlayer(Player* , std::vector<int> ) override;
    bool isRunning() override;
    bool render(Image* i) override;
    bool update() override;
    bool reset() override;

};
