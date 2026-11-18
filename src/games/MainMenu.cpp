#include "MainMenu.hpp"
#include "logger.h"
#include <cstdint>
#include <string>

#include "../network/Player.hpp"


MainMenu::MainMenu( std::vector<Game*> games ) {

    this->_current_game = -1;
    this->_games = games;

    this->_camera = new Camera();

    this->_spaceship = new MainMenu_Spaceship();
    this->_spaceship->mesh = fast_obj_read("assets/MainMenu/spaceship.obj");
    if (!this->_spaceship->mesh) {
        logger(3, "Could not load assets/MainMenu/spaceship.obj");
    }
    this->_spaceship->position = Vec3(
        6,
        6,
        8
    );
    this->_spaceship->rotation.z = 180;

}


MainMenu::~MainMenu() {

    delete this->_spaceship;
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

    this->_state_version++;

    return true;

}


bool MainMenu::updatePlayer( Player* p, std::vector<Value> v) {

    std::lock_guard<std::mutex> lock( this->_mutex );

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
    else {

        this->_spaceship->rotation.y += 1.;

        fastObjMesh* m   = _spaceship->mesh;
        const Vec3   pos = _spaceship->position;
        const Vec3   rot = _spaceship->rotation;

        Vec3 ax(1, 0, 0), ay(0, 1, 0), az(0, 0, 1);
        for (Vec3* v : { &ax, &ay, &az }) {
            rotationX(v, rot.x);
            rotationY(v, rot.y);
            rotationZ(v, rot.z);
        }

        _screen.resize(m->position_count);
        _world.resize(m->position_count);
        for (unsigned i = 1; i < m->position_count; ++i) {
            const float* p = m->positions + 3 * i;
            Vec3 w(ax.x * p[0] + ay.x * p[1] + az.x * p[2] + pos.x,
                   ax.y * p[0] + ay.y * p[1] + az.y * p[2] + pos.y,
                   ax.z * p[0] + ay.z * p[1] + az.z * p[2] + pos.z);
            _world[i]  = w;
            _screen[i] = translate2D(&w, _camera);
        }
        _tris.clear();

        unsigned base = 0;
        for (unsigned f = 0; f < m->face_count; ++f) {
            unsigned n = m->face_vertices[f];

            uint8_t r = 255, g = 255, b = 255;
            if (m->material_count > 0) {
                unsigned mi = m->face_materials[f];
                if (mi < m->material_count) {
                    const fastObjMaterial& mat = m->materials[mi];
                    r = (uint8_t)(mat.Kd[0] * 255.0f);
                    g = (uint8_t)(mat.Kd[1] * 255.0f);
                    b = (uint8_t)(mat.Kd[2] * 255.0f);
                }
            }


            unsigned ia = m->indices[base].p;
            for (unsigned k = 1; k + 1 < n; ++k) {
                unsigned ib = m->indices[base + k].p;
                unsigned ic = m->indices[base + k + 1].p;

                const Vec2& sa = _screen[ia];
                const Vec2& sb = _screen[ib];
                const Vec2& sc = _screen[ic];

                float area = (sb.x - sa.x) * (sc.y - sa.y) - (sb.y - sa.y) * (sc.x - sa.x);

                if (area >= 0.0f) continue;

                const Vec3& A = _world[ia];
                const Vec3& B = _world[ib];
                const Vec3& C = _world[ic];

                float e1x = B.x - A.x, e1y = B.y - A.y, e1z = B.z - A.z;
                float e2x = C.x - A.x, e2y = C.y - A.y, e2z = C.z - A.z;

                float nx = e1y * e2z - e1z * e2y;
                float ny = e1z * e2x - e1x * e2z;
                float nz = e1x * e2y - e1y * e2x;
                float len = std::sqrt(nx * nx + ny * ny + nz * nz);
                if (len > 0.0f) { nx /= len; ny /= len; nz /= len; }

                const float lx = 0.0f, ly = -1.0f, lz = 0.0f;
                float diffuse = std::max(0.0f, nx * lx + ny * ly + nz * lz);
                float light   = 0.4f + 0.8f * diffuse;

                uint8_t R = (uint8_t)(r * light);
                uint8_t G = (uint8_t)(g * light);
                uint8_t B8 = (uint8_t)(b * light);

                float cx = (A.x + B.x + C.x) / 3.0f;
                float cy = (A.y + B.y + C.y) / 3.0f;
                float cz = (A.z + B.z + C.z) / 3.0f;
                float depth = cx * cx + cy * cy + cz * cz;

                _tris.push_back({ depth, _screen[ia], _screen[ib], _screen[ic], R, G, B8 });
            }
            base += n;
        }
        std::sort(_tris.begin(), _tris.end(),
                  [](const MainMenu_tri& x, const MainMenu_tri& y) { return x.depth > y.depth; });

    }

    return true;

}


bool MainMenu::isRunning() {

    return true;

}


bool MainMenu::render( Image* img ) {

    std::lock_guard<std::mutex> lock( this->_mutex );

    if ( this->_iterations == 0 ) {
        initCamera( img, this->_camera, 60 );
    }

    img->clear();
    this->_iterations += 1;

    if ( this->_current_game >= 0 )
        this->_games[this->_current_game]->render( img );

    else {

        for (const MainMenu_tri& t : _tris) {
            img->setDrawingColor(rgb(t.r, t.g, t.b8));
            img->drawTriangle(t.a.x, t.a.y, t.b.x, t.b.y, t.c.x, t.c.y);
        }

        img->setDrawingColor(rgb(22, 54, 232));
        img->drawText( "Heilbronn University", 0.01, 0.01, 0.98, 0.1 );
        img->setDrawingColor(rgb(255, 255, 255));
        img->drawText( "LED Matrix", 0.01, 0.11, 0.98, 0.1 );
        std::string nb_players = std::to_string(this->_players.size());
        img->setDrawingColor(rgb(219, 226, 26));
        img->drawText( nb_players, 0.01, 0.55, 0.48, 0.4 );
        img->setDrawingColor(rgb(255, 255, 255));
        img->drawText( "players", 0.51, 0.80, 0.48, 0.1 );

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
