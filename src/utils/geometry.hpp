#pragma once

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


/**
 * @fn rotation
 * @brief Rotate a Vec2 by degree.
 * @param Vec2* The vector to rotate.
 * @param float The amount in degree to rotate the vector by.
 */
void rotation( Vec2* v, float degree );


/**
 * @fn rotationX
 * @brief Rotate a Vec3 on the X axis by degree.
 * @param Vec3* The vector to rotate.
 * @param float The amount in degree to rotate the vector by.
 */
void rotationX( Vec3* v, float degree );


/**
 * @fn rotationY
 * @brief Rotate a Vec3 on the Y axis by degree.
 * @param Vec3* The vector to rotate.
 * @param float The amount in degree to rotate the vector by.
 */
void rotationY( Vec3* v, float degree );


/**
 * @fn rotationZ
 * @brief Rotate a Vec3 on the Z axis by degree.
 * @param Vec3* The vector to rotate.
 * @param float The amount in degree to rotate the vector by.
 */
void rotationZ( Vec3* v, float degree );


/**
 * @fn initCamera
 * @brief Initalize the Camera object to fit a Image size and a FoV in degree.
 * @param Image* The image we want the Camera to fit to.
 * @param Camera* The Camera object we want to modify.
 * @param float The Field of View in degree of the Camera.
 */
void initCamera( Image* i, Camera* c, float fov );


/**
 * @fn translate2D
 * @brief Translate a Vec3 to a 2D plane using a Camera object.
 * @param Vec3* Vector3 we want to transform.
 * @param Camera* The Camera object we view the 2D plan from.
 */
Vec2 translate2D( Vec3* v, Camera* c );
