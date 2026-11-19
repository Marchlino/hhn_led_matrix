#pragma once

#include "../display/Image.hpp"

/**
 * @class Vec2
 * @brief Represent a 2D vector with basic operations.
 */
class Vec2 {

    public :

        float x, y;

        Vec2() = default;
        Vec2(float x, float y): x(x), y(y) {};
        Vec2& operator+=(const Vec2 &t);
        Vec2& operator-=(const Vec2 &t);
        Vec2& operator*=(const Vec2 &t);
        Vec2& operator/=(const Vec2 &t);
        Vec2& operator+=(float t);
        Vec2& operator-=(float t);
        Vec2& operator*=(float t);
        Vec2& operator/=(float t);
        Vec2 operator+(const Vec2 &t) const;
        Vec2 operator-(const Vec2 &t) const;
        Vec2 operator*(const Vec2 &t) const;
        Vec2 operator/(const Vec2 &t) const;
        Vec2 operator+(float t) const;
        Vec2 operator-(float t) const;
        Vec2 operator*(float t) const;
        Vec2 operator/(float t) const;

};
Vec2 operator*(float a, Vec2 b);

/**
 * @class Vec3
 * @brief Represent a 2D vector with basic operations.
 */
class Vec3 {

    public:

        float x, y, z;
        Vec3() {}
        Vec3(float x, float y, float z): x(x), y(y), z(z) {}
        Vec3& operator+=(const Vec3 &t);
        Vec3& operator-=(const Vec3 &t);
        Vec3& operator*=(float t);
        Vec3& operator/=(float t);
        Vec3 operator+(const Vec3 &t) const;
        Vec3 operator-(const Vec3 &t) const;
        Vec3 operator*(float t) const;
        Vec3 operator/(float t) const;

};
Vec3 operator*(float a, Vec3 b);


/**
 * @struct Camera
 * @brief Save for each axis the focal point.
 */
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

/**
 * @fn dot
 * @brief Dot product between a and b
 * @param a First vector
 * @param b Second vector
 */
float dot(Vec2 a, Vec2 b);

/**
 * @fn dot
 * @brief Dot product between a and b
 * @param a First vector
 * @param b Second vector
 */
float dot(Vec3 a, Vec3 b);

/**
 * @fn norm
 * @brief Norm of vector a
 * @param a Vector
 */
float norm(Vec2 a);

/**
 * @fn abs
 * @brief Absolute of vector
 * @param a Vector
 */
double abs(Vec2 a);

/**
 * @fn angle
 * @brief Angle of vector a and b
 * @param a Vector
 * @param b Vector
 */
double angle(Vec2 a, Vec2 b);

/**
 * @fn cross
 * @brief Cross product of a and b
 * @param a Vector
 * @param b Vector
 */
Vec3 cross(Vec3 a, Vec3 b);

/**
 * @fn cross
 * @brief Cross product of a and b
 * @param a Vector
 * @param b Vector
 */
float cross(Vec2 a, Vec2 b);

/**
 * @fn intersect
 * @brief Intersection of line (Point 1, Direction 1) and (Point 2, Direction 2)
 * @param a1 Point of first line
 * @param d1 Direction of first line
 * @param a2 Point of second line
 * @param d2 Direction of second line
 */
Vec2 intersect(Vec2 a1, Vec2 d1, Vec2 a2, Vec2 d2);

/**
 * @fn distance
 * @brief Distance between point a and point b
 * @param a First point
 * @param b Second point
 */
float distance(Vec2 a, Vec2 b);

/**
 * @fn distance
 * @brief Distance between point a and point b
 * @param a First point
 * @param b Second point
 */
float distance(Vec3 a, Vec3 b);
