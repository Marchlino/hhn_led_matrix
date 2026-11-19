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


Vec2& Vec2::operator+=(const Vec2 &t) {
    x += t.x;
    y += t.y;
    return *this;
}
Vec2& Vec2::operator-=(const Vec2 &t) {
    x -= t.x;
    y -= t.y;
    return *this;
}
Vec2& Vec2::operator*=(const Vec2 &t) {
    x *= t.x;
    y *= t.y;
    return *this;
}
Vec2& Vec2::operator/=(const Vec2 &t) {
    x /= t.x;
    y /= t.y;
    return *this;
}
Vec2& Vec2::operator*=(float t) {
    x *= t;
    y *= t;
    return *this;
}
Vec2& Vec2::operator/=(float t) {
    x /= t;
    y /= t;
    return *this;
}
Vec2& Vec2::operator+=(float t) {
    x += t;
    y += t;
    return *this;
}
Vec2& Vec2::operator-=(float t) {
    x -= t;
    y -= t;
    return *this;
}
Vec2 Vec2::operator+(const Vec2 &t) const {
    return Vec2(*this) += t;
}
Vec2 Vec2::operator-(const Vec2 &t) const {
    return Vec2(*this) -= t;
}
Vec2 Vec2::operator*(const Vec2 &t) const {
    return Vec2(*this) *= t;
}
Vec2 Vec2::operator/(const Vec2 &t) const {
    return Vec2(*this) /= t;
}
Vec2 Vec2::operator*(float t) const {
    return Vec2(*this) *= t;
}
Vec2 Vec2::operator/(float t) const {
    return Vec2(*this) /= t;
}
Vec2 Vec2::operator+(float t) const {
    return Vec2(*this) += t;
}
Vec2 Vec2::operator-(float t) const {
    return Vec2(*this) -= t;
}

Vec3& Vec3::operator+=(const Vec3 &t) {
    x += t.x;
    y += t.y;
    z += t.z;
    return *this;
}
Vec3& Vec3::operator-=(const Vec3 &t) {
    x -= t.x;
    y -= t.y;
    z -= t.z;
    return *this;
}
Vec3& Vec3::operator*=(float t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
}
Vec3& Vec3::operator/=(float t) {
    x /= t;
    y /= t;
    z /= t;
    return *this;
}
Vec3 Vec3::operator+(const Vec3 &t) const {
    return Vec3(*this) += t;
}
Vec3 Vec3::operator-(const Vec3 &t) const {
    return Vec3(*this) -= t;
}
Vec3 Vec3::operator*(float t) const {
    return Vec3(*this) *= t;
}
Vec3 Vec3::operator/(float t) const {
    return Vec3(*this) /= t;
}

float dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}
float dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float norm(Vec2 a) {
    return dot(a, a);
}
double abs(Vec2 a) {
    return sqrt(norm(a));
}

double angle(Vec2 a, Vec2 b) {
    return acos(dot(a, b) / abs(a) / abs(b));
}

Vec3 cross(Vec3 a, Vec3 b) {
    return Vec3(a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x);
}

float cross(Vec2 a, Vec2 b) {
    return a.x * b.y - a.y * b.x;
}

Vec2 intersect(Vec2 a1, Vec2 d1, Vec2 a2, Vec2 d2) {
    return a1 + cross(a2 - a1, d2) / cross(d1, d2) * d1;
}

Vec2 operator*(float a, Vec2 b) {
    return b * a;
}

Vec3 operator*(float a, Vec3 b) {
    return b * a;
}

float distance(Vec2 a, Vec2 b) {
    return sqrt( pow( a.x - b.x, 2) + pow( a.y - b.y, 2) );
}
float distance(Vec3 a, Vec3 b) {
    return sqrt( pow( a.x - b.x, 2) + pow( a.y - b.y, 2) + pow( a.z - b.z, 2 ) );
}
