#pragma once
#include "Game.hpp"
#include "../utils/geometry.hpp"

class Pong: public Game {

private:

    std::vector<Player*> _players;
    std::vector<int> _scores;
    std::vector<Vec2> _paddles;
    Vec2 _ball;
    Vec2 _ball_direction;
    float _ball_radius;

    int _getPlayerIndex(Player* );

public:

    Pong();
    ~Pong() = default;
    bool addPlayer(Player* ) override;
    bool removePlayer(Player* )override;
    bool updatePlayer(Player* , std::vector<Value> ) override;
    bool isRunning() override;
    bool render(Image* i) override;
    bool update() override;
    bool reset() override;
    int getControllerType() override;
    std::string getName() override;

};
