#ifndef OBJECT_H
#define OBJECT_H

#include "../geometry/geometry.h"

enum MaterialType {
    DIFFUSE_AND_GLOSSY,
    REFLECTION_AND_REFRACTION,
    REFLECTION
};

class Object
{
public:
    Object();
    virtual ~Object() {};
    virtual bool intersect(const Vec3f&, const Vec3f&, float&, uint32_t&, Vec2f&) const = 0;
    virtual void getSurfaceProperties(const Vec3f&, const Vec3f&,
        const uint32_t&, const Vec2f&, Vec3f&, Vec2f&) const = 0;
    virtual Vec3f evalDiffuseColour(const Vec2f&) const { return diffuseColour; }

    MaterialType materialType;
    float ior;
    float Kd, Ks;
    Vec3f diffuseColour;
    float specularExponent;
};

#endif