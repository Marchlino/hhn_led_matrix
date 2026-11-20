#include "geometry.hpp"
#include "fast_obj.h"
#include <string>

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
 * @class Object3D
 * @brief Represent and manage a 3D object in obj format
 */
class Object3D {

    private:

        std::vector<Vec2> _screen;
        std::vector<Vec3> _world;
        std::vector<Object3D_Tri> _tris;
        fastObjMesh* _mesh;

    public:

        Vec3 rotation;
        Vec3 position;

        Object3D();
        Object3D( std::string path );
        ~Object3D();

        /**
         * @fn loadMesh
         * @brief Load an .obj file from a given path
         */
        bool loadMesh( std::string path );

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
