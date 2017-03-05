#include <math.h>
#include <algorithm>
#include "Math.h"

#define PI 3.14159265359

bool Math::solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    return true;
}

bool Math::rayTriangleIntersect(const Vec3f& v0, const Vec3f& v1,
    const Vec3f& v2, const Vec3f& orig, const Vec3f& dir,
    float& tNear, float& u, float& v)
{
    Vec3f edge1 = v1 - v0;
    Vec3f edge2 = v2 - v0;
    Vec3f pvec = dir.crossProduct(edge2);
    float det = edge1.dotProduct(pvec);
    if (det == 0 || det < 0) return false;

    Vec3f tvec = orig - v0;
    u = tvec.dotProduct(pvec);
    if (u < 0 || u > det) return false;

    Vec3f qvec = tvec.crossProduct(edge1);
    v = dir.dotProduct(qvec);
    if (v < 0 || u + v > det) return false;

    float invDet = 1 / det;

    tNear = edge2.dotProduct(qvec) * invDet;
    u *= invDet;
    v *= invDet;

    return true;
}

float Math::clamp(const float& lo, const float& hi, const float& v)
{
    return std::max(lo, std::min(hi, v));
}

float Math::deg2rad(const float& deg)
{
    return deg * PI / 180;
}

Vec3f Math::mix(const Vec3f& a, const Vec3f& b, const float& val)
{
    return a * (1 - val) + b * val;
}

Vec3f Math::reflect(const Vec3f& I, const Vec3f& N)
{
    return I - 2 * I.dotProduct(N) * N;
}

Vec3f Math::refract(const Vec3f& I, const Vec3f& N, const float& ior)
{
    float cosi = clamp(-1, 1, I.dotProduct(N));
    float etai = 1, etat = ior;
    Vec3f n = N;
    if (cosi < 0) {
        cosi = -cosi;
    }
    else {
        std::swap(etai, etat);
        n = -N;
    }

    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);

    return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
}

void Math::fresnel(const Vec3f& I, const Vec3f& N, const float& ior, float& kr)
{
    float cosi = clamp(-1, 1, I.dotProduct(N));
    float etai = 1, etat = ior;
    if (cosi > 0) {
        std::swap(etai, etat);
    }

    // Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) /
            ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) /
            ((etai * cosi) + (etat * cost));
        kr = (Rs + Rp * Rp) / 2;
    }
}
