#include "Object.h"

Object::Object()
    : materialType(DIFFUSE_AND_GLOSSY)
    , ior(1.3)
    , Kd(0.8)
    , Ks(0.2)
    , diffuseColour(0.2)
    , specularExponent(25)
{

}