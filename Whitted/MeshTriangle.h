#ifndef MESH_TRIANGLE_H
#define MESH_TRIANGLE_H

#include <memory>
#include "Object.h"

class MeshTriangle : public Object
{
public:
    MeshTriangle(const Vec3f* verts, const uint32_t* vertsIndex,
        const uint32_t& numTris, const Vec2f* st);

    bool intersect(const Vec3f& orig, const Vec3f& dir, float& tNear,
        uint32_t& index, Vec2f& uv) const;

    void getSurfaceProperties(const Vec3f& P, const Vec3f& I,
        const uint32_t& index, const Vec2f& uv, Vec3f& N, Vec2f& st) const;

    Vec3f evalDiffuseColour(const Vec2f& st) const;

    std::unique_ptr<Vec3f[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;
    std::unique_ptr<Vec2f[]> stCoordinates;
};

#endif