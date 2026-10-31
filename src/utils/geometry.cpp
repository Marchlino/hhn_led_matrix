#include <cmath>

#include "geometry.hpp"


void rotation( Vec2* v, float degree ) {

    float rad = degree * ( M_PI / 180.0f );
    float vx = v->x;
    v->x = std::cos( rad ) * v->x - std::sin( rad ) * v->y;
    v->y = std::sin( rad ) * vx + std::cos( rad ) * v->y;

}


void rotationX( Vec3* v, float degree ) {

    float rad = degree * ( M_PI / 180.0f );
    float cos_a = std::cos( rad );
    float sin_a = std::sin( rad );

    float vy = v->y;
    v->y = cos_a * v->y - sin_a * v->z;
    v->z = sin_a * vy + cos_a * v->z;

}


void rotationY( Vec3* v, float degree ) {

    float rad = degree * ( M_PI / 180.0f );
    float cos_a = std::cos( rad );
    float sin_a = std::sin( rad );

    float vx = v->x;
    v->x = cos_a * v->x + sin_a * v->z;
    v->z = -sin_a * vx + cos_a * v->z;

}


void rotationZ( Vec3* v, float degree ) {

    float rad = degree * ( M_PI / 180.0f );
    float cos_a = std::cos( rad );
    float sin_a = std::sin( rad );

    float vx = v->x;
    v->x = cos_a * v->x - sin_a * v->y;
    v->y = sin_a * vx + cos_a * v->y;

}


void initCamera( Image* i, Camera* c, float fov ) {

    float aspect_ratio = i->weigthUnit() / i->heigthUnit();

    float fov_rad = fov * ( M_PI / 180.0f );
    if ( i->weigthUnit() > i->heigthUnit() ) {
        c->fov_angle_y = 1.0f / std::tan( fov_rad );
        c->fov_angle_x = c->fov_angle_y / aspect_ratio;
    } else {
        c->fov_angle_x = 1.0f / std::tan( fov_rad );
        c->fov_angle_y = c->fov_angle_x / aspect_ratio;
    }


}

Vec2 translate2D( Vec3* v, Camera* c ) {

    Vec2 result( -1., -1. );

    if ( v->z <= 0 ) {
        return result;
    }

    result.x = ( v->x * c->fov_angle_x ) / v->z;
    result.y = ( v->y * c->fov_angle_y ) / v->z;

    return result;

}
