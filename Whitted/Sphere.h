#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

class Sphere : public Object
{
public:
    Sphere(const Vec3f& c, const float& r);

    bool intersect(const Vec3f& orig, const Vec3f& dir,
        float& tNear, uint32_t& index, Vec2f& uv) const;

    void getSurfaceProperties(const Vec3f& P, const Vec3f& I,
        const uint32_t& index, const Vec2f& uv, Vec3f& N, Vec2f& st) const;

    Vec3f center;
    float radius, radius2;
};

#endif