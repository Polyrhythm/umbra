#include <algorithm>

#include "../geometry/geometry.h"

// Decide which algorhithm to use
#define MOLLER_TRUMBORE false

#define PI 3.14159265359

inline
float deg2rad(const float &deg)
{
    return deg * PI / 180;
}

inline
float clamp(const float& lo, const float& hi, const float& v)
{
    return std::max(lo, std::min(hi, v));
}

bool rayTriangleIntersect(const Vec3f& orig, const Vec3f& dir, const Vec3f& v0,
    const Vec3f& v1, const Vec3f& v2, float& t, float& u, float& v)
{
#ifdef MOLLER_TRUMBORE

#else

#endif
}

int main()
{
    return 0;
}