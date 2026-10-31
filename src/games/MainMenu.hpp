#pragma once
#include <cstdint>
#include <vector>

#include "Game.hpp"
#include "../utils/geometry.hpp"


/**
 * @class MainMenu
 * @brief Program Main menu, manage and lauch all the other games.
 * @param std::vector<Game*> List of games.
 */
class MainMenu: public Game {

    private:

        std::vector<Player*> _players;
        std::vector<int> _next_game_vote;
        std::vector<Game*> _games;
        std::vector<int> _voted_for;
        int _current_game;
        uint_fast32_t _iterations = 0;
        std::vector<Vec3*> _cube;
        Camera* _camera;

        /**
         * @fn _getPlayerIndex
         * @brief Return the index of the given Player objet in the players vector by matching IDs.
         * @param Player* Player to look for.
         */
        int _getPlayerIndex( Player* );

        /**
         * @fn _mostVotedGame
         * @brief Return the index of the most voted game in voted_for.
         */
        int _mostVotedGame();

    public:

        MainMenu( std::vector<Game*> games );
        bool addPlayer( Player* ) override;
        bool removePlayer( Player* ) override;
        bool updatePlayer( Player* , std::vector<int> ) override;
        bool isRunning() override;
        bool render( Image* i) override;
        bool update() override;
        bool reset() override;

};
