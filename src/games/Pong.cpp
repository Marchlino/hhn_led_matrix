#include "Pong.hpp"
#include "../network/Player.hpp"
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#include "logger.h"


Pong::Pong() {

    this->_ball.x = 0.5;
    this->_ball.y = 0.5;
    srand(time(0));
    this->_ball_direction.x = 0.01;
    this->_ball_direction.y = 0.01;
    rotation( &this->_ball_direction, (float) (rand()%360) );
    this->_ball_radius = 0.02;

}


int Pong::_getPlayerIndex(Player* p) {

    for (int i = 0; i < this->_players.size(); ++i) {

        if (this->_players.at(i)->getID() == p->getID()) {

            return i;

        }

    }

    return -1;

}

bool Pong::addPlayer(Player* p) {

    if ( this->_players.size() >= 2 ) return false;
    if ( _getPlayerIndex(p) != -1 ) return false;

    this->_players.push_back(p);
    this->_paddles.push_back(Vec2(0.1 + (0.75 * (this->_players.size() - 1)), 0.4));
    this->_scores.push_back(0);
    return true;

}

bool Pong::removePlayer(Player* p) {

    int index = _getPlayerIndex(p);
    if (index == -1) return false;

    this->_players.erase(this->_players.begin() + index);
    this->_paddles.erase(this->_paddles.begin() + index);
    this->_scores.erase(this->_scores.begin() + index);
    return true;

}

bool Pong::updatePlayer(Player* p, std::vector<Value> v) {

    int index = _getPlayerIndex(p);

    this->_paddles[index].y = v[0].number.value()/256.;

    return true;

}

bool Pong::isRunning() {

    return true;

}

bool Pong::render(Image* img) {

    img->setDrawingColor(rgb(255, 255, 255));

    for ( Vec2 v: this->_paddles )
        img->drawRect(v.x, v.y, 0.05, 0.2);

    img->drawCircle( this->_ball.x - this->_ball_radius, this->_ball.y - this->_ball_radius, this->_ball_radius );

    img->setDrawingColor(rgb(20, 148, 14));

    for ( int i = 0; i < this->_players.size(); i++ )
        img->drawText( this->_players[i]->getName() + ": " + std::to_string(this->_scores[i]) , 0.05 + (0.5 * i), 0.05, 0.4, 0.1);

    return true;

}

bool Pong::update() {

    this->_ball = add( this->_ball, this->_ball_direction );
    this->_ball_direction = mult( this->_ball_direction, 1.005 );
    if ( this->_ball_direction.y > 0.1 ) {
        this->_ball_direction.y = 0.1;
    }
    if ( this->_ball_direction.x > 0.1 ) {
        this->_ball_direction.x = 0.1;
    }

    for ( Vec2 v: this->_paddles ) {
        if ( (this->_ball.y + this->_ball_direction.y) >= v.y && (this->_ball.y + this->_ball_direction.y) <= v.y + 0.2 && (this->_ball.x + this->_ball_direction.x) >= v.x && (this->_ball.x + this->_ball_direction.x) <= v.x + 0.05 ) {

            this->_ball_direction.y = -this->_ball_direction.y;
            this->_ball_direction.x = -this->_ball_direction.x;

            this->_ball.y = v.y + std::abs( (this->_ball.y + this->_ball_direction.y) - v.y );
            return true;

        }
    }

    if ( (this->_ball.y + this->_ball_direction.y) - this->_ball_radius <= 0 ) {

        this->_ball_direction.y = -this->_ball_direction.y;

        this->_ball.y = this->_ball_radius;
        return true;

    } else if ( (this->_ball.y + this->_ball_direction.y) + this->_ball_radius >= 1. ) {

        this->_ball_direction.y = -this->_ball_direction.y;

        this->_ball.y = 1. - this->_ball_radius;
        return true;

    }

    if ( this->_ball.x - this->_ball_radius <= 0 || this->_ball.x + this->_ball_radius >= 1. ) {

        for ( Vec2 v: this->_paddles ) {

            v.x = 0;
            v.y = 0.1 + (0.8 * this->_players.size() - 1);

        }

        this->_ball.x = 0.5;
        this->_ball.y = 0.5;

        this->_ball_direction.x = 0.01;
        this->_ball_direction.y = 0.01;
        srand(time(0));
        rotation( &this->_ball_direction, (float) (rand()%360) );

        if ( this->_ball.x - this->_ball_radius <= 0 ) {

            if ( this->_scores.size() > 0)
                this->_scores[0]++;

        } else {

            if ( this->_scores.size() > 1)
                this->_scores[1]++;

        }

    }

    return true;

}

bool Pong::reset() {

    this->_players.clear();
    this->_scores.clear();
    this->_paddles.clear();
    this->_ball.x = 0.5;
    this->_ball.y = 0.5;
    this->_ball_direction.x = 0.01;
    this->_ball_direction.y = 0.01;
    srand(time(0));
    rotation( &this->_ball_direction, (float) (rand()%360) );
    return true;

}


int Pong::getControllerType() {

    return 1;

}


std::string Pong::getName() {

    return "Pong";

}
