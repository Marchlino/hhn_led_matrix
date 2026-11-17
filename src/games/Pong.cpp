#include "Pong.hpp"
#include "../network/Player.hpp"
#include <asm-generic/socket.h>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#include "logger.h"


Pong::Pong() {

    this->_ball = new Pong_ball();
    this->_ball->radius = 0.01;
    this->_ball->touched_by = nullptr;
    this->_ball->position.x = 0.5 - this->_ball->radius;
    this->_ball->position.y = 0.5 - this->_ball->radius;
    srand(time(0));
    this->_ball->direction = Vec2( 0.01, 0.01 );
    rotation( &this->_ball->direction, rand()%360 );
    this->_wave.radius = 2.;

}


int Pong::_getPlayerIndex(Player* p) {

    for (int i = 0; i < this->_players.size(); ++i) {

        if (this->_players.at(i)->p->getID() == p->getID()) {

            return i;

        }

    }

    return -1;

}

bool Pong::_recalcPlayers() {

    float share_step = 360. / this->_players.size();
    for ( int i = 0; i < this->_players.size(); i++ ) {

        float angle1 = share_step * i * (M_PI/180);
        float angle2 = share_step * ( i + 1 ) * (M_PI/180);

        Vec2 p1 = Vec2(0.1 + 0.4 * cos(angle1) + 0.4, 0.1 + 0.4 * sin(angle1) + 0.4);
        Vec2 p2 = Vec2(0.1 + 0.4 * cos(angle2) + 0.4, 0.1 + 0.4 * sin(angle2) + 0.4);
        Vec2 pmid = Vec2((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

        this->_players[i]->border_1 = p1;
        this->_players[i]->border_2 = p2;
        this->_players[i]->paddle = pmid;
        this->_players[i]->normal = mult( diff(pmid, .5), -1 );
        float n_s = sqrt( pow( this->_players[i]->normal.x, 2 ) + pow( this->_players[i]->normal.y, 2 ) );
        this->_players[i]->normal.x /= n_s;
        this->_players[i]->normal.y /= n_s;

    }

    return true;

}

bool Pong::addPlayer(Player* p) {

    Pong_player* new_p = new Pong_player();
    new_p->p = p;
    new_p->score = 0;
    this->_players.push_back(new_p);

    _recalcPlayers();

    return true;

}

bool Pong::removePlayer(Player* p) {

    int index = _getPlayerIndex(p);
    if ( this->_ball->touched_by == this->_players[0] )
         this->_ball->touched_by = nullptr;

    this->_players.erase( this->_players.begin() + index );

    return true;

}

bool Pong::updatePlayer(Player* p, std::vector<Value> v) {

    int index = _getPlayerIndex(p);
    Pong_player* player = this->_players[index];

    Vec2 movement = Vec2(
        v[0].number.value() * ( player->border_1.x - player->border_2.x ) / 256,
        v[0].number.value() * ( player->border_1.y - player->border_2.y ) / 256
    );

    player->paddle = add( movement, player->border_2 );

    return true;

}

bool Pong::isRunning() {

    return true;

}

bool Pong::render(Image* img) {

    img->setDrawingColor(rgb(255, 255, 255));

    for ( Pong_player* player: this->_players ) {

        if ( this->_ball->touched_by == player ) {
            img->setDrawingColor(rgb(223, 20, 40));
        } else {
            img->setDrawingColor(rgb(255, 255, 255));
        }

        Vec2 point = Vec2(
            (player->border_1.x - player->border_2.x) * 0.1,
            (player->border_1.y - player->border_2.y) * 0.1
        );

        img->drawTriangle(
            player->paddle.x + point.x, player->paddle.y + point.y,
            player->paddle.x + point.x + (player->normal.x * 0.01), player->paddle.y + point.y + (player->normal.y * 0.01),
            player->paddle.x - point.x, player->paddle.y - point.y
        );
        img->drawTriangle(
            player->paddle.x - point.x, player->paddle.y - point.y,
            player->paddle.x - point.x + (player->normal.x * 0.01), player->paddle.y - point.y + (player->normal.y * 0.01),
            player->paddle.x + point.x + (player->normal.x * 0.01), player->paddle.y + point.y + (player->normal.y * 0.01)
        );

        img->setDrawingColor(rgb(128, 128, 128));
        std::string score = std::to_string(player->score);
        img->drawText( score, player->paddle.x + player->normal.x * -0.1, player->paddle.y + player->normal.y * -0.1, 0.05 * score.size(), 0.05);

    }
    img->setDrawingColor(rgb(255, 255, 255));
    img->drawCircle(this->_ball->position.x, this->_ball->position.y, this->_ball->radius);

    if ( this->_wave.radius < 1 ) {
        img->setDrawingColor(rgb(255, 255, 255));
        img->drawCircle(this->_wave.position.x - this->_wave.radius, this->_wave.position.y - this->_wave.radius, this->_wave.radius);
        img->setDrawingColor(rgb(0, 0, 0));
        img->drawCircle(this->_wave.position.x + 0.05 - this->_wave.radius, this->_wave.position.y + 0.05 - this->_wave.radius, this->_wave.radius - 0.05);

    }

    return true;

}

bool Pong::update() {

    this->_wave.radius *= 1.2;

    if ( this->_wave.radius >= 1 ) {

    if ( this->_ball->position.x < 0 || this->_ball->position.x > 1 || this->_ball->position.y < 0 || this->_ball->position.y > 1 ) {

        this->_wave.position = this->_ball->position;
        this->_wave.radius = 0.01;

        if ( this->_ball->touched_by != nullptr )
            this->_ball->touched_by->score++;

        this->_ball->touched_by = nullptr;
        this->_ball->position.x = 0.5 - this->_ball->radius;
        this->_ball->position.y = 0.5 - this->_ball->radius;
        srand(time(0));
        this->_ball->direction = Vec2( 0.01, 0.01 );
        rotation( &this->_ball->direction, rand()%360 );
        _recalcPlayers();

        return true;

    }

    Vec2 ball_next = Vec2(
        this->_ball->position.x + this->_ball->direction.x,
        this->_ball->position.y + this->_ball->direction.y
    );

    for ( Pong_player* player: this->_players ) {

        Vec2 half = Vec2(
            (player->border_1.x - player->border_2.x) * 0.1,
                         (player->border_1.y - player->border_2.y) * 0.1
        );
        Vec2 a  = Vec2( player->paddle.x - half.x, player->paddle.y - half.y );
        Vec2 ab = Vec2( 2 * half.x, 2 * half.y );

        float len2 = ab.x * ab.x + ab.y * ab.y;
        float t = ( (ball_next.x - a.x) * ab.x + (ball_next.y - a.y) * ab.y ) / len2;
        t = std::max( 0.0f, std::min( 1.0f, t ) );
        Vec2 closest = Vec2( a.x + t * ab.x, a.y + t * ab.y );

        if ( distance( closest, ball_next ) >= this->_ball->radius )
            continue;

        float nlen = sqrt( player->normal.x * player->normal.x + player->normal.y * player->normal.y );
        Vec2 n = Vec2( player->normal.x / nlen, player->normal.y / nlen );
        float side = (ball_next.x - closest.x) * n.x + (ball_next.y - closest.y) * n.y;
        if ( side < 0 ) {
            n.x = -n.x;
            n.y = -n.y;
        }

        float dot = this->_ball->direction.x * n.x + this->_ball->direction.y * n.y;
        if ( dot >= 0 )
            continue;

        Vec2 reflected = Vec2(
            this->_ball->direction.x - 2 * dot * n.x,
            this->_ball->direction.y - 2 * dot * n.y
        );
        this->_ball->direction = mult( reflected, 1.01 );

        this->_ball->position = Vec2(
            closest.x + n.x * this->_ball->radius,
            closest.y + n.y * this->_ball->radius
        );
        this->_ball->touched_by = player;

        break;
    }

    this->_ball->position = add( this->_ball->position, this->_ball->direction );

    }

    return true;
}

bool Pong::reset() {

    for ( Pong_player* p: this->_players ) {
        delete p;
    }
    this->_players.clear();

    this->_wave.radius = 2.;
    this->_ball->touched_by = nullptr;
    this->_ball->position.x = 0.5 - this->_ball->radius;
    this->_ball->position.y = 0.5 - this->_ball->radius;
    srand(time(0));
    this->_ball->direction = Vec2( 0.01, 0.01 );
    rotation( &this->_ball->direction, rand()%360 );

    return true;

}


int Pong::getControllerType() {

    return 1;

}


std::string Pong::getName() {

    return "Pong";

}
