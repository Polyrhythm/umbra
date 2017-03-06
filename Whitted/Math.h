#ifndef MATH_H
#define MATH_H

#include "../geometry/geometry.h"

class Math {
public:
    static float PI;

    static bool solveQuadratic(const float &a, const float &b,
        const float &c, float &x0, float &x1);

    static bool rayTriangleIntersect(const Vec3f& v0, const Vec3f& v1,
        const Vec3f& v2, const Vec3f& orig, const Vec3f& dir,
        float& tNear, float& u, float& v);

    static float clamp(const float& lo, const float& hi, const float& v);

    static float deg2rad(const float& deg);
    static Vec3f mix(const Vec3f& a, const Vec3f& b, const float& val);

    static Vec3f reflect(const Vec3f& T, const Vec3f& N);
    static Vec3f refract(const Vec3f& I, const Vec3f& N, const float& ior);

    static void fresnel(const Vec3f& I, const Vec3f& N, const float& ior, float& kr);

    static float sphericalTheta(const Vec3f& v);
    static float sphericalPhi(const Vec3f& v);
};

#endif