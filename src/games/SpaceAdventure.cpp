#include <cstdlib>
#include <ctime>

#include "SpaceAdventure.hpp"
#include "../network/Player.hpp"
#include "logger.h"

SpaceAdventure::SpaceAdventure() {

    this->_asteroids_mesh = new Object3D_Mesh( "assets/SpaceAdventure/asteroid.obj" );
    this->_player_mesh = new Object3D_Mesh( "assets/SpaceAdventure/spaceship.obj" );

    for ( int i = 0; i < 10; i++ ) {

        srand(i + time(0));
        this->_asteroids.push_back( new SpaceAdventure_Asteroid() );
        this->_asteroids.back()->obj = Object3D( this->_asteroids_mesh );
        this->_asteroids.back()->obj.position = Vec3(0, 0, 20);
        this->_asteroids.back()->position.x = 1. / (rand()%11);
        this->_asteroids.back()->position.y = -1. / (rand()%11);

    }

    this->_camera = new Camera();

}

SpaceAdventure::~SpaceAdventure() {

    for ( SpaceAdventure_Asteroid* obj: this->_asteroids )
        delete obj;
    for ( SpaceAdventure_Player* p: this->_players )
        delete p;
    delete this->_asteroids_mesh;
    delete this->_player_mesh;

}

int SpaceAdventure::_getPlayerIndex(Player* p) {

    for (int i = 0; i < this->_players.size(); ++i) {

        if (this->_players[i]->player->getID() == p->getID()) {

            return i;

        }

    }

    return -1;

}

bool SpaceAdventure::addPlayer(Player* p ) {

    SpaceAdventure_Player* tmp = new SpaceAdventure_Player();
    tmp->spaceship = Object3D( this->_player_mesh );
    tmp->spaceship.position = Vec3(0, 0, 20);
    tmp->rotation = Vec3(90, 180, 90);
    tmp->player = p;

    this->_players.push_back(tmp);

    return true;

}

bool SpaceAdventure::removePlayer(Player* p) {

    int index = _getPlayerIndex(p);
    this->_players.erase( this->_players.begin() + index );

    return true;

}
bool SpaceAdventure::updatePlayer(Player* p, std::vector<Value> v) {

    int index = _getPlayerIndex(p);
    switch (v[0].number.value()) {

        case 0: this->_players[index]->direction.y -= 0.01; break;
        case 1: this->_players[index]->direction.x += 0.01; break;
        case 2: this->_players[index]->direction.y += 0.01; break;
        case 3: this->_players[index]->direction.x -= 0.01; break;

    }

    return true;

}
bool SpaceAdventure::isRunning() { return true; }

bool SpaceAdventure::render(Image *i) {

    if ( this->_iterations == 0 ) {
        initCamera( i, this->_camera, 60 );
    }
    this->_iterations++;

    for ( SpaceAdventure_Asteroid* obj: this->_asteroids ) {

        for (const Object3D_Tri& t : obj->obj.tris) {
            i->setDrawingColor(rgb(t.r, t.g, t.b8));
            i->drawTriangle(t.a.x + obj->position.x, t.a.y + obj->position.y, t.b.x + obj->position.x, t.b.y + obj->position.y, t.c.x + obj->position.x, t.c.y + obj->position.y);
        }

    }
    for ( SpaceAdventure_Player* p: this->_players ) {

        for (const Object3D_Tri& t : p->spaceship.tris) {
            i->setDrawingColor(rgb(t.r, t.g, t.b8));
            i->drawTriangle(t.a.x + p->position.x, t.a.y + p->position.y, t.b.x + p->position.x, t.b.y + p->position.y, t.c.x + p->position.x, t.c.y + p->position.y);
        }

    }
    for ( Vec2 v: this->_bullets )
        i->drawCircle(v.x, v.y, 0.01);

    return true;

}

bool SpaceAdventure::update() {

    for ( SpaceAdventure_Asteroid* obj: this->_asteroids ) {

        obj->position.y += 0.01;
        obj->obj.rotation.z += 1.;
        obj->obj.rotation.x += 2.;
        obj->obj.rotation.y += 4.;
        if ( obj->position.y > 1.1 ) {

            srand(*(int*)(void*)obj + time(0));
            obj->position.x = 1. / (rand()%11);
            obj->position.y = -1. / (rand()%11);

        }
        obj->obj.calculateTriangles( this->_camera );

    }

    for ( SpaceAdventure_Player* p: this->_players ) {

        //this->_bullets.push_back( Vec2(p->position.x, p->position.y) );
        p->position += p->direction;
        p->direction *= 0.9;
        if ( p->position.y > 0.9 )
            p->position.y = 0.9;
        else if ( p->position.y < 0.1 )
            p->position.y = 0.1;
        if ( p->position.x > 0.9 )
            p->position.x = 0.9;
        else if ( p->position.x < 0.1 )
            p->position.x = 0.1;
        p->spaceship.rotation = Vec3(
            p->rotation.x + (p->direction.x * 1800),
            p->rotation.y + (p->direction.y * 900),
            p->rotation.z
        );
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

bool SpaceAdventure::reset() {

    for ( SpaceAdventure_Asteroid* s: this->_asteroids ) {
        s->position.x = 1. / (rand()%11);
        s->position.y = -1. / (rand()%11);
    }
    for ( SpaceAdventure_Player* p: this->_players )
        delete p;
    this->_players.clear();
    this->_bullets.clear();

    return true;

}

int SpaceAdventure::getControllerType() {

    return 2;

}

std::string SpaceAdventure::getName() {

    return "Space Adventure";

}
