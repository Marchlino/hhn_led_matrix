#include "MainMenu.hpp"
#include "logger.h"
#include <cstdint>
#include <string>

#include "../network/Player.hpp"


MainMenu::MainMenu( std::vector<Game*> games ) {

    this->_current_game = -1;
    this->_games = games;
    this->_cube = {
        new Vec3(0.5, 0.5, 0.5), new Vec3(0.5, 0.5, -0.5), new Vec3(0.5, -0.5, 0.5), new Vec3(0.5, -0.5, -0.5),
        new Vec3(-0.5, 0.5, 0.5), new Vec3(-0.5, 0.5, -0.5), new Vec3(-0.5, -0.5, 0.5), new Vec3(-0.5, -0.5, -0.5)
    };

    this->_camera = new Camera();

}


MainMenu::~MainMenu() {

    for ( Vec3* v: this->_cube )
        delete v;
    delete this->_camera;

}


bool MainMenu::addPlayer( Player* p ) {

    std::lock_guard<std::mutex> lock( this->_mutex );

    // The player is already here ?
    if ( _getPlayerIndex( p ) != -1 ) return false;

    this->_players.push_back( p );
    this->_voted_for.push_back( -1 );

    // Also add the player to the running game...
    if ( this->_current_game >= 0 ) {

        this->_games.at( this->_current_game )->addPlayer( p );

    }

    logger( 1, ( "Added player with ID: " + p->getID() ).c_str() );
    this->_state_version++;

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

    std::lock_guard<std::mutex> lock( this->_mutex );

    int player_index = _getPlayerIndex( p );

    // Player not in game
    if ( player_index == -1 ) return false;

    this->_players.erase( this->_players.begin() + player_index );
    this->_voted_for.erase( this->_voted_for.begin() + player_index );

    // Also remove the player from the running game...
    if ( this->_current_game >= 0 ) {

        this->_games.at( this->_current_game )->removePlayer( p );

    }

    logger( 1, ( "Removed player with ID: " + p->getID() ).c_str() );
    this->_state_version++;

    return true;

}


bool MainMenu::updatePlayer( Player* p, std::vector<Value> v) {

    std::lock_guard<std::mutex> lock( this->_mutex );

    std::string request = "[ ";
    for ( const Value& l: v ) {
        if ( l.number )
            request += std::to_string( *l.number ) + ", ";
        else if ( l.string )
            request += "\"" + *l.string + "\", ";
    }
    request += " ]";
    logger( 1, ( "Received request: " + request + " from player with ID: " + p->getID() ).c_str() );

    int player_index = _getPlayerIndex( p );

    if ( player_index == -1 ) return false;

    // We need at least 2 parameters
    if ( v.size() >= 2 && v[0].number ) {

        switch (*v[0].number) {

            // Vote
            case 0:
                if ( v[1].number && *v[1].number < this->_games.size() ) {

                    // Mark the vote for game n in the voted for array
                    this->_voted_for[player_index] = *v[1].number;
                    this->_state_version++;
                    return true;

                }
                return false;

            // Pass to current game
            case 1:
                if ( this->_current_game >= 0 ) {
                    v.erase(v.begin());
                    return this->_games[this->_current_game]->updatePlayer( p, v );
                }
                return false;

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

    std::lock_guard<std::mutex> lock( this->_mutex );

    int next_game = _mostVotedGame();

    // We change of game...
    if ( next_game != this->_current_game ) {

        // if the current game is a game and not the main menu
        if ( this->_current_game >= 0 )
            this->_games[this->_current_game]->reset();

        this->_current_game = next_game;

        if ( this->_current_game >= 0 )
            for ( Player* p: this->_players )
                this->_games[this->_current_game]->addPlayer( p );

        this->_state_version++;

    }

    if ( this->_current_game >= 0 )
        this->_games[this->_current_game]->update();

    return true;

}


bool MainMenu::isRunning() {

    return true;

}


bool MainMenu::render( Image* img ) {

    std::lock_guard<std::mutex> lock( this->_mutex );

    if ( this->_iterations == 0 ) {
        initCamera( img, this->_camera, 30 );
    }

    img->clear();
    this->_iterations += 1;

    if ( this->_current_game >= 0 )
        this->_games[this->_current_game]->render( img );

    else {

        img->setDrawingColor( rgb( 227, 181, 219 ) );
        img->drawRect( 0., 0., 1., 1. );
        img->setDrawingColor( rgb( 36, 31, 35 ) );
        img->drawText( "F", 0.05, -0.1, 0.9, 0.9 );

        std::string text = "^Animii^";
        uint8_t ind = 0;

        const int CYCLE_DURATION = 360;
        const int PHASE_DURATION = 120;
        int phase = (this->_iterations % CYCLE_DURATION) / PHASE_DURATION;
        float phase_progress = (float)((this->_iterations % CYCLE_DURATION) % PHASE_DURATION) / PHASE_DURATION;

        float letter_width = 0.08;
        int num_letters = text.length();
        float total_width = num_letters * letter_width;
        float start_x_base = (1.0 - total_width) / 2.0;

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

            img->setDrawingColor( rgb( 173, 90, 178 ) );
            img->drawCircle( p.x + 0.7, p.y + 0.2, 0.02 );

            float start_x = start_x_base + ind * letter_width;
            float start_y = 0.8;

            float target_x = p.x + 0.7;
            float target_y = p.y + 0.6;

            float text_x = target_x;
            float text_y = target_y;

            if ( phase == 0 ) {
                text_x = start_x + (target_x - start_x) * phase_progress;
                text_y = start_y + (target_y - start_y) * phase_progress;
            }
            else if ( phase == 1 ) {
                text_x = target_x;
                text_y = target_y;
            }
            else if ( phase == 2 ) {
                text_x = target_x + (start_x - target_x) * phase_progress;
                text_y = target_y + (start_y - target_y) * phase_progress;
            }

            img->setDrawingColor( rgb( 66, 12, 53 ) );
            img->drawText( std::string(1, text.at(ind)), text_x, text_y, 0.1, 0.1 );
            ind++;
        }

        for ( int i = 0; i < this->_players.size(); i++ ) {

            img->drawText(this->_players[i]->getName(), 0.1, 0.1 * i, 0.9, 0.1);

        }

    }

    return true;

}



bool MainMenu::reset() {

    std::lock_guard<std::mutex> lock( this->_mutex );

    this->_players.clear();
    this->_next_game_vote.clear();
    this->_voted_for.clear();
    this->_current_game = -1;
    this->_state_version++;

    return true;

}


int MainMenu::getControllerType() {

    return 0;

}


std::string MainMenu::getName() {

    return "Main menu";

}


uint32_t MainMenu::getStateVersion() {

    return this->_state_version;

}


std::vector<std::string> MainMenu::getGameNames() {

    std::lock_guard<std::mutex> lock( this->_mutex );

    std::vector<std::string> names;
    for ( Game* g: this->_games )
        names.push_back( g->getName() );

    return names;

}


std::vector<int> MainMenu::getVoteCounts() {

    std::lock_guard<std::mutex> lock( this->_mutex );

    std::vector<int> counts( this->_games.size(), 0 );
    for ( int v: this->_voted_for )
        if ( v >= 0 && v < counts.size() )
            counts[v] += 1;

    return counts;

}


int MainMenu::getCurrentGame() {

    std::lock_guard<std::mutex> lock( this->_mutex );
    return this->_current_game;

}


int MainMenu::getCurrentControllerType() {

    std::lock_guard<std::mutex> lock( this->_mutex );

    if ( this->_current_game >= 0 )
        return this->_games[this->_current_game]->getControllerType();

    return this->getControllerType();

}
