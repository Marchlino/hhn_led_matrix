#pragma once
#include <mutex>
#include <string>
#include <vector>
#include "crow/websocket.h"

#include "../games/Game.hpp"


/**
 * @class Player
 * @brief Represent a connected player.
 * @param std::string The name of the player
 * @param std::string The (unique) id of the player.
 */
class Player {

    private:

        std::string _name;
        std::string _id;
        Game* _game;
        crow::websocket::connection* _connection;
        std::mutex _name_mutex;

    public:

        Player( crow::websocket::connection* connection );

        /**
         * @fn getID
         * @brief Return the player ID.
         */
        std::string getID();

        /**
         * @fn getConnection
         * @brief Return the websocket connection of the player.
         */
        crow::websocket::connection* getConnection();

        /**
         * @fn setName
         * @brief Change the player name.
         */
        bool setName( std::string );

        /**
         * @fn getName
         * @brief Return the player name.
         */
        std::string getName();

        /**
         * @fn updateGame
         * @brief Pass player request to the current game.
         */
        bool updateGame( std::vector<Value> );

        /**
         * @fn setGame
         * @brief Set Player current game.
         */
        bool setGame( Game* g );

        /**
         * @fn exitGame
         * @brief Exit Player current game.
         */
        bool exitGame();

};
