#pragma once

#include "Vec3.h"

struct Light {
    Vec3 position;
    Vec3 color;
    float intensity;

    Light() 
        : position(Vec3(2.0f, 2.0f, -1.0f))
        , color(Vec3(1.0f, 1.0f, 1.0f))
        , intensity(1.0f) {}

    Light(const Vec3& pos, const Vec3& col, float intens)
        : position(pos)
        , color(col)
        , intensity(intens) {}
};

