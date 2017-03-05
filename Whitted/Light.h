#ifndef LIGHT_H
#define LIGHT_H

#include "../geometry/geometry.h"

class Light
{
public:
    Light(const Vec3f& b, const Vec3f& i);
    Vec3f position;
    Vec3f intensity;
};

#endif