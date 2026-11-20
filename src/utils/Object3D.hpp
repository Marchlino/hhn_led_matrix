#pragma once
#include "geometry.hpp"
#include "fast_obj.h"
#include <string>
#include "logger.h"

#include "../display/Image.hpp"

/**
 * @struct Object3D_Tri
 * @brief Cache the calculated triangles from the mesh.
 */
typedef struct {
    float   depth;
    Vec2    a, b, c;
    uint8_t r, g, b8;
} Object3D_Tri;

/**
 * @struct Object3D_Mesh
 * @brief Mesh of a Object3D
 */
typedef struct Object3D_Mesh {
  fastObjMesh* data;
  Object3D_Mesh();
  Object3D_Mesh( std::string path ) {
        this->data = fast_obj_read( path.c_str() );
        if ( !this->data )
            logger( 3, ("Could not load .obj file " + path).c_str() );
  }
} Object3D_Mesh;

/**
 * @class Object3D
 * @brief Represent and manage a 3D object in obj format
 */
class Object3D {

    private:

        std::vector<Vec2> _screen;
        std::vector<Vec3> _world;
        Object3D_Mesh* _mesh;

    public:

        Vec3 rotation;
        Vec3 position;
        std::vector<Object3D_Tri> tris;

        Object3D();
        Object3D( Object3D_Mesh* mesh );
        ~Object3D() = default;

        /**
         * @fn calculateTriangles
         * @brief Calculate and cache the triangles and their order from the mesh.
         */
        bool calculateTriangles( Camera* cam );

        /**
         * @fn display
         * @brief Draw the mesh on the given image.
         */
        bool display( Image* img );

};
