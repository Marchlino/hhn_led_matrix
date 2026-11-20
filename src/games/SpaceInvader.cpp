#include <cstdlib>
#include <ctime>

#include "SpaceInvader.hpp"
#include "../network/Player.hpp"
#include "logger.h"

SpaceInvader::SpaceInvader() {

    this->_asteroids_mesh = new Object3D_Mesh( "assets/SpaceInvader/asteroid.obj" );
    this->_player_mesh = new Object3D_Mesh( "assets/SpaceInvader/spaceship.obj" );

    for ( int i = 0; i < 10; i++ ) {

        srand(i + time(0));
        this->_asteroids.push_back( new SpaceInvader_Asteroid() );
        this->_asteroids.back()->obj = Object3D( this->_asteroids_mesh );
        this->_asteroids.back()->obj.position.z = 10;
        this->_asteroids.back()->obj.position.x = (rand()%16);
        this->_asteroids.back()->obj.position.y = - (rand()%10);

    }

    this->_camera = new Camera();

}

SpaceInvader::~SpaceInvader() {

    for ( SpaceInvader_Asteroid* obj: this->_asteroids )
        delete obj;
    for ( SpaceInvader_Player* p: this->_players )
        delete p;
    delete this->_asteroids_mesh;
    delete this->_player_mesh;

}

int SpaceInvader::_getPlayerIndex(Player* p) {

    for (int i = 0; i < this->_players.size(); ++i) {

        if (this->_players[i]->player->getID() == p->getID()) {

            return i;

        }

    }

    return -1;

}

bool SpaceInvader::addPlayer(Player* p ) {

    SpaceInvader_Player* tmp = new SpaceInvader_Player();
    tmp->spaceship = Object3D( this->_player_mesh );
    tmp->spaceship.position = Vec3(10, 10, 10);
    tmp->player = p;

    this->_players.push_back(tmp);

    return true;

}

bool SpaceInvader::removePlayer(Player* p) {

    int index = _getPlayerIndex(p);
    this->_players.erase( this->_players.begin() + index );

    return true;

}
bool SpaceInvader::updatePlayer(Player* p, std::vector<Value> v) {

    logger(0, (std::to_string(v.size())).c_str() );

    return true;

}
bool SpaceInvader::isRunning() { return true; }

bool SpaceInvader::render(Image *i) {

    if ( this->_iterations == 0 ) {
        initCamera( i, this->_camera, 60 );
    }
    this->_iterations++;

    for ( SpaceInvader_Asteroid* obj: this->_asteroids )
        obj->obj.display( i );
    for ( SpaceInvader_Player* p: this->_players )
        p->spaceship.display( i );
    for ( Vec2 v: this->_bullets )
        i->drawCircle(v.x, v.y, 0.01);

    return true;

}

bool SpaceInvader::update() {

    for ( SpaceInvader_Asteroid* obj: this->_asteroids ) {

        obj->obj.position.y += 0.1;
        obj->obj.rotation.z += 1.;
        obj->obj.rotation.x += 2.;
        obj->obj.rotation.y += 4.;
        if ( obj->obj.position.y > 20 ) {

            srand(*(int*)(void*)obj + time(0));
            obj->obj.position.y = - (rand()%10);
            obj->obj.position.x = rand()%16;

        }
        obj->obj.calculateTriangles( this->_camera );

    }

    for ( SpaceInvader_Player* p: this->_players ) {

        this->_bullets.push_back( Vec2(p->spaceship.position.x, p->spaceship.position.y) );
        p->spaceship.calculateTriangles( this->_camera );

    }

    for ( int i = 0; i < this->_bullets.size(); i++ ) {

        this->_bullets[i].y -= 0.01;
        if ( this->_bullets[i].y < 0 ) {
            this->_bullets.erase( this->_bullets.begin() + i );
            i--;
        }

    }

    return true;

}

bool SpaceInvader::reset() {

    for ( SpaceInvader_Asteroid* s: this->_asteroids )
        s->obj.position = Vec3( 0, 0, 0 );
    for ( SpaceInvader_Player* p: this->_players )
        delete p;
    this->_players.clear();
    this->_bullets.clear();

    return true;

}

int SpaceInvader::getControllerType() {

    return 2;

}

std::string SpaceInvader::getName() {

    return "Space invader";

}
