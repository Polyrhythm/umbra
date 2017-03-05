#include "PinholeCamera.h"

static const float inchToMM = 25.4;

PinholeCamera::PinholeCamera(const float& focalLength, const Vec2f& aperture, const float& zNear,
        const float& zFar, const Vec2i& imageSize, const FitStrategy& fitStrategy)
    : _focalLength(focalLength)
    , _aperture(aperture)
    , _zNear(zNear)
    , _zFar(zFar)
    , _imageSize(imageSize)
    , _fitStrategy(fitStrategy)
{
    _top = ((_aperture.y * inchToMM / 2) / _focalLength) * zNear;
    _bottom = -_top;
    _aspectRatio = _aperture.x / _aperture.y;
    _left = _bottom * _aspectRatio;


    std::printf("Screen window left/right coords %f %f\n", _left, _right);
}