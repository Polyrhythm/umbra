#ifndef OPTIONS_H
#define OPTIONS_H

struct Options
{
    uint32_t width;
    uint32_t height;
    float fov;
    float imageAspectRatio;
    uint8_t maxDepth;
    Vec3f backgroundColour;
    float bias;
};

#endif