#pragma once
#include "Game.hpp"
#include "../utils/geometry.hpp"

/**
 * @struct Pong_player
 * @brief Save player data and cache often needed values
 */
typedef struct {

    Player* p;
    int score;
    Vec2 border_1;
    Vec2 border_2;
    Vec2 paddle;
    Vec2 normal;

} Pong_player;

/**
 * @struct Pong_ball
 * @brief Store ball values
 */
typedef struct {

    Vec2 position;
    Vec2 direction;
    float radius;
    Pong_player* touched_by;

} Pong_ball;

/**
 * @struct Wave
 * @brief Store the wave effect values
 */
typedef struct {

    Vec2 position;
    float radius;

} Wave;

/**
 * @class Pong
 * @brief Circular variant of the classical Pong game.
 */
class Pong: public Game {

private:

    std::vector<Pong_player*> _players;
    Pong_ball* _ball;
    Wave _wave;

    int _getPlayerIndex(Player* );

    /**
     * @fn _recalcPlayers
     * @brief Recalculate player postion within the circle
     */
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
