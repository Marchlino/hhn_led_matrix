#pragma once
#include <cstdint>
#include <vector>

#include "Game.hpp"
#include "../utils/geometry.hpp"


/**
 * @class MainMenu
 * @desc Program Main menu, manage and lauch all the other games.
 */
class MainMenu: public Game {

    private:

        std::vector<Player*> players;
        std::vector<int> next_game_vote;
        std::vector<Game*> games;
        std::vector<int> voted_for;
        int current_game;
        uint_fast32_t iterations = 0;
        std::vector<Vec3*> cube;
        Camera* c;


        int getPlayerIndex(Player* );
        int mostVotedGame();

    public:

        MainMenu(std::vector<Game*> games);
        bool addPlayer(Player* ) override;
        bool removePlayer(Player* ) override;
        bool updatePlayer(Player* , std::vector<int> ) override;
        bool isRunning() override;
        bool render(Image* i) override;
        bool update() override;
        bool reset() override;

};
