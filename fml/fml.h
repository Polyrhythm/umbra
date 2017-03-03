#ifndef FML_H
#define FML_H

namespace fml {

template<typename T>
struct vec3
{
    T x, y, z;

    vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    vec3(const T& xx) : x(xx), y(xx), z(xx) {}
    vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}

    T length()
    {
        return sqrt(x * x + y * y + z * z);
    }
};

}

#endif