#ifndef PINHOLE_CAMERA_H
#define PINHOLE_CAMERA_H

#include "../geometry/geometry.h"

enum FitStrategy { Fill, Cover };

class PinholeCamera
{
public:
    PinholeCamera(const float& focalLength, const Vec2f& aperture, const float& zNear,
        const float& zFar, const Vec2i& imageSize, const FitStrategy& fitStrategy);

private:
    float _focalLength;
    Vec2f _aperture;
    float _zNear;
    float _zFar;
    Vec2i _imageSize;
    FitStrategy _fitStrategy;
    Matrix44f _cameraToWorld;

    float _angleOfView;
    float _left;
    float _right;
    float _bottom;
    float _top;
    float _aspectRatio;
};

#endif