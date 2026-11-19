#pragma once
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>
#include "fast_obj.h"

#include "Game.hpp"
#include "../utils/geometry.hpp"

/**
 * @struct MainMenu_Spaceship
 * @brief The main menu 3D object. Was originally a spaceship.
 */
typedef struct {

    fastObjMesh* mesh;
    Vec3 rotation;
    Vec3 position;

} MainMenu_Spaceship;

/**
 * @struct MainMenu_tri
 * @brief Cache the calculated triangles from the mesh.
 */
typedef struct {
    float   depth;
    Vec2    a, b, c;
    uint8_t r, g, b8;
} MainMenu_tri;


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
        MainMenu_Spaceship* _spaceship;
        Camera* _camera;
        std::mutex _mutex;
        std::vector<Vec2> _screen;
        std::vector<Vec3> _world;
        std::vector<MainMenu_tri> _tris;

        // Bumped whenever something the web clients display changes
        // (players, votes, current game), so Network knows when to resend.
        std::atomic<uint32_t> _state_version{ 0 };

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
        ~MainMenu();
        bool addPlayer( Player* ) override;
        bool removePlayer( Player* ) override;
        bool updatePlayer( Player* , std::vector<Value> ) override;
        bool isRunning() override;
        bool render( Image* i) override;
        bool update() override;
        bool reset() override;
        int getControllerType() override;
        std::string getName() override;

        /**
         * @fn getStateVersion
         * @brief Return a counter incremented every time the menu state changes.
         */
        uint32_t getStateVersion();

        /**
         * @fn getGameNames
         * @brief Return the names of all registered games, in vote order.
         */
        std::vector<std::string> getGameNames();

        /**
         * @fn getVoteCounts
         * @brief Return the number of votes for each registered game.
         */
        std::vector<int> getVoteCounts();

        /**
         * @fn getCurrentGame
         * @brief Return the index of the running game, -1 for the main menu.
         */
        int getCurrentGame();

        /**
         * @fn getCurrentControllerType
         * @brief Return the controller type of the running game (the menu's when none runs).
         */
        int getCurrentControllerType();

};
