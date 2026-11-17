#pragma once
#include "Game.hpp"
#include "../utils/geometry.hpp"


typedef struct {

    Player* p;
    int score;
    Vec2 border_1;
    Vec2 border_2;
    Vec2 paddle;
    Vec2 normal;

} Pong_player;


typedef struct {

    Vec2 position;
    Vec2 direction;
    float radius;
    Pong_player* touched_by;

} Pong_ball;


typedef struct {

    Vec2 position;
    float radius;

} Wave;


class Pong: public Game {

private:

    std::vector<Pong_player*> _players;
    Pong_ball* _ball;
    Wave _wave;


    int _getPlayerIndex(Player* );
    bool _recalcPlayers();

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
