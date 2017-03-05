#include "PinholeCamera.h"

int main()
{
    PinholeCamera* cam = new PinholeCamera(35.0f, Vec2f(0.980f, 0.735f), 0.1f, 1000.0f,
        Vec2i(600, 480), FitStrategy::Fill);
}