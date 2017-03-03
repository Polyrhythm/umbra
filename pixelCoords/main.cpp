#include <iostream>
#include "../geometry/geometry.h"

int main()
{
    Matrix44f m(0.718762, 0.615033, -0.324214, 0, -0.393732, 0.744416, 0.539277, 0, 0.573024, -0.259959, 0.777216, 0, 0.526967, 1.254234, -2.53215, 1);
    Vec3f Plocal(-0.5f, 0.5f, -0.5f), Pworld;

    m.multVecMatrix(Plocal, Pworld);
    std::cerr << Pworld << std::endl;

    m.invert();
    Pworld = Vec3f{ -0.315792f, 1.4489f, -2.48901f };
    m.multVecMatrix(Pworld, Plocal);

    std::cerr << Plocal << std::endl;

    return 0;
}