#include "MainMenu.hpp"
#include "logger.h"


MainMenu::MainMenu( std::vector<Game*> games ) {

    this->_current_game = -1;
    this->_games = games;

    this->_cube = {
        new Vec3(0.5, 0.5, 0.5), new Vec3(0.5, 0.5, -0.5), new Vec3(0.5, -0.5, 0.5), new Vec3(0.5, -0.5, -0.5),
        new Vec3(-0.5, 0.5, 0.5), new Vec3(-0.5, 0.5, -0.5), new Vec3(-0.5, -0.5, 0.5), new Vec3(-0.5, -0.5, -0.5)
    };

    this->_camera = new Camera();

}


bool MainMenu::addPlayer( Player* p ) {

    // The player is already here ?
    if ( _getPlayerIndex( p ) != -1 ) return false;

    this->_players.push_back( p );
    this->_voted_for.push_back( -1 );

    // Also add the player to the running game...
    if ( this->_current_game >= 0 ) {

        this->_games.at( this->_current_game )->addPlayer( p );

    }

    return true;

}


int MainMenu::_getPlayerIndex( Player* p ) {

    for ( int i = 0; i < this->_players.size(); ++i ) {

        // Found the player...
        if ( this->_players.at( i )->getID() == p->getID() ) {

            return i;

        }

    }

    // Player not in game
    return -1;

}


bool MainMenu::removePlayer( Player* p ) {

    int player_index = _getPlayerIndex( p );

    // Player not in game
    if ( player_index == -1 ) return false;

    this->_players.erase( this->_players.begin() + player_index );
    this->_voted_for.erase( this->_voted_for.begin() + player_index );

    // Also remove the player from the running game...
    if ( this->_current_game >= 0 ) {

        this->_games.at( this->_current_game )->removePlayer( p );

    }

    return false;

}


bool MainMenu::updatePlayer( Player* p, std::vector<int> v) {

    int player_index = _getPlayerIndex( p );

    if ( player_index == -1 ) return false;

    // We need at least 2 parameters
    if ( v.size() >= 2 ) {

        switch (v[0]) {

            // Vote
            case 0:
                if ( v[1] < this->_games.size() ) {

                    // Mark the vote for game n in the voted for array
                    this->_voted_for[player_index] = v[1];
                    return true;

                }

            // Pass to current game
            case 1:
                if ( this->_current_game >= 0 )
                    return this->_games[this->_current_game]->updatePlayer( p, v );

        }

    }

    return false;

}


int MainMenu::_mostVotedGame() {

    std::vector<int> game_score( this->_games.size() + 1 );

    for ( int v: this->_voted_for ) {

        // Not voted or neutral state
        if ( v < 0 ) {

            game_score[0] += 1;

        } else {

            game_score[v + 1] += 1;

        }

    }

    int max = 0;
    for ( int i = 0; i < game_score.size(); ++i ) {

        if ( game_score[max] < game_score[i] ) {

            max = i;

        }

    }

    return max - 1;

}


bool MainMenu::update() {

    int next_game = _mostVotedGame();

    // We change of game...
    if ( next_game != this->_current_game ) {

        // if the current game is a game and not the main menu (main menu = -1, neutral vote);
        if ( this->_current_game >= 0 )
            this->_games[this->_current_game]->reset();

        this->_current_game = next_game;

        for ( Player* p: this->_players )
            this->_games[this->_current_game]->addPlayer( p );

    }

    // update the running game
    if ( this->_current_game >= 0 )
        this->_games[this->_current_game]->update();

    return true;

}


bool MainMenu::isRunning() {

    return true;

}


bool MainMenu::render( Image* i ) {

    if ( this->_iterations == 0 ) {
        initCamera( i, this->_camera, 30 );
    }

    i->clear();
    this->_iterations += 1;

    if ( this->_current_game >= 0 )
        this->_games[this->_current_game]->render( i );
    else {
        for ( Vec3* v: this->_cube ) {

             // Vertex relative to cube's local origin (0,0,0)
            Vec3* local_vertex = v;

            // Apply rotations in local space
            rotationX( v, 2. );
            rotationY( v, 5. );
            rotationZ( v, 1. );

            // Transform to world space
            Vec3 world_vertex = Vec3(
                local_vertex->x + 0,
                local_vertex->y + 0,
                local_vertex->z + 10
            );

            Vec2 p = translate2D( &world_vertex, this->_camera );

            i->setDrawingColor( rgb( 40, 70, 61 ) );
            i->drawText( "LED Matrix", p.x + 0.4, p.y + 0.4, 0.5, 0.1 );
            i->setDrawingColor( rgb( 173, 90, 178 ) );
            i->drawCircle( p.x + 0.5, p.y + 0.5, 0.01 );

        }
    }

    return true;

}


bool MainMenu::reset() {

    this->_players.clear();
    this->_next_game_vote.clear();
    this->_voted_for.clear();
    this->_current_game = -1;

    return true;

}
