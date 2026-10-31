#pragma once
#include <vector>

#include "Player.hpp"
#include "../display/Image.hpp"


/**
 * @class Game
 * @brief Abstract class to define any object which manage players, need to react to player action and render something.
 */
class Game {

    public:

        /**
         * @fn removePlayer
         * @brief Function called to add a player to a running game.
         * @param Player* The player to add.
         */
        virtual bool addPlayer( Player* ) = 0;

        /**
         * @fn removePlayer
         * @brief Function called to remove a player to a running game.
         * @param Player* The player to remove.
         */
        virtual bool removePlayer( Player* ) = 0;

        /**
         * @fn updatePlayer
         * @brief To update player values when a request is received.
         * @param Player* The player who is making the request.
         * @param std::vector<int> The data sent by the player.
         */
        virtual bool updatePlayer( Player* , std::vector<int> ) = 0;

        /**
         * @fn isRunning
         * @brief Check if the game is still running.
         */
        virtual bool isRunning() = 0;

        /**
         * @fn render
         * @brief Render the current game state to the Image object.
         * @param Image* The image to draw on.
         */
        virtual bool render( Image* i) = 0;

        /**
         * @fn update
         * @brief Game loop to update game objects like the ball for Pong.
         */
        virtual bool update() = 0;

        /**
         * @fn reset
         * @brief Reset the game to it's default state'
         */
        virtual bool reset() = 0;

};
