#include <algorithm>
#include <cmath>
#include "logger.h"

#include "Object3D.hpp"
#include "geometry.hpp"


Object3D::Object3D() = default;
Object3D::Object3D( Object3D_Mesh* mesh ) {

    this->_mesh = mesh;

}

bool Object3D::calculateTriangles( Camera* cam ) {

    fastObjMesh* m   = this->_mesh->data;
    const Vec3   pos = this->position;
    const Vec3   rot = this->rotation;

    Vec3 ax(1, 0, 0), ay(0, 1, 0), az(0, 0, 1);
    for (Vec3* v : { &ax, &ay, &az }) {
        rotationX(v, rot.x);
        rotationY(v, rot.y);
        rotationZ(v, rot.z);
    }

    _screen.resize(m->position_count);
    _world.resize(m->position_count);
    for (unsigned i = 1; i < m->position_count; ++i) {
        const float* p = m->positions + 3 * i;
        Vec3 w(ax.x * p[0] + ay.x * p[1] + az.x * p[2] + pos.x,
               ax.y * p[0] + ay.y * p[1] + az.y * p[2] + pos.y,
               ax.z * p[0] + ay.z * p[1] + az.z * p[2] + pos.z);
        _world[i]  = w;
        _screen[i] = translate2D(&w, cam);
    }
    tris.clear();

    unsigned base = 0;
    for (unsigned f = 0; f < m->face_count; ++f) {
        unsigned n = m->face_vertices[f];

        uint8_t r = 255, g = 255, b = 255;
        if (m->material_count > 0) {
            unsigned mi = m->face_materials[f];
            if (mi < m->material_count) {
                const fastObjMaterial& mat = m->materials[mi];
                r = (uint8_t)(mat.Kd[0] * 255.0f);
                g = (uint8_t)(mat.Kd[1] * 255.0f);
                b = (uint8_t)(mat.Kd[2] * 255.0f);
            }
        }

        unsigned ia = m->indices[base].p;
        for (unsigned k = 1; k + 1 < n; ++k) {
            unsigned ib = m->indices[base + k].p;
            unsigned ic = m->indices[base + k + 1].p;

            const Vec2& sa = _screen[ia];
            const Vec2& sb = _screen[ib];
            const Vec2& sc = _screen[ic];

            float area = (sb.x - sa.x) * (sc.y - sa.y) - (sb.y - sa.y) * (sc.x - sa.x);

            if (area >= 0.0f) continue;

            const Vec3& A = _world[ia];
            const Vec3& B = _world[ib];
            const Vec3& C = _world[ic];

            float e1x = B.x - A.x, e1y = B.y - A.y, e1z = B.z - A.z;
            float e2x = C.x - A.x, e2y = C.y - A.y, e2z = C.z - A.z;

            float nx = e1y * e2z - e1z * e2y;
            float ny = e1z * e2x - e1x * e2z;
            float nz = e1x * e2y - e1y * e2x;
            float len = std::sqrt(nx * nx + ny * ny + nz * nz);
            if (len > 0.0f) { nx /= len; ny /= len; nz /= len; }

            const float lx = 0.0f, ly = 0.0f, lz = -1.0f;
            float diffuse = std::max(0.0f, nx * lx + ny * ly + nz * lz);
            float light   = 0.4f + 0.8f * diffuse;

            uint8_t R = (uint8_t)(r * light);
            uint8_t G = (uint8_t)(g * light);
            uint8_t B8 = (uint8_t)(b * light);

            float cx = (A.x + B.x + C.x) / 3.0f;
            float cy = (A.y + B.y + C.y) / 3.0f;
            float cz = (A.z + B.z + C.z) / 3.0f;
            float depth = cx * cx + cy * cy + cz * cz;

            tris.push_back({ depth, _screen[ia], _screen[ib], _screen[ic], R, G, B8 });
        }
        base += n;
    }
    std::sort(tris.begin(), tris.end(),
              [](const Object3D_Tri& x, const Object3D_Tri& y) { return x.depth > y.depth; });

    return true;

}


bool Object3D::display( Image* img ) {

    for (const Object3D_Tri& t : tris) {
        img->setDrawingColor(rgb(t.r, t.g, t.b8));
        img->drawTriangle(t.a.x, t.a.y, t.b.x, t.b.y, t.c.x, t.c.y);
    }
    return true;

}

