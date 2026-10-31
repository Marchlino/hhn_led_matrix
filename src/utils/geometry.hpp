#include "../display/Image.hpp"

typedef struct v2 {
    float x;
    float y;
    v2(float x, float y) {
        this->x = x;
        this->y = y;
    }
} Vec2;

typedef struct v3 {
    float x;
    float y;
    float z;
    v3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
} Vec3;

typedef struct v4 {
    float x;
    float y;
    float z;
    float d;
    v4(float x, float y, float z, float d) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->d = d;
    }
} Vec4;

typedef struct {
    float fov_angle_x;
    float fov_angle_y;
} Camera;

void rotation(Vec2* v, float degree);
void rotationX(Vec3* v, float degree);
void rotationY(Vec3* v, float degree);
void rotationZ(Vec3* v, float degree);

void initCamera(Image* i, Camera* c, float fov);
Vec2 translate2D(Vec3* v, Camera* c);
