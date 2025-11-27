#pragma once

#include "Vec3.h"

struct Ray {
    Vec3 origin;
    Vec3 direction;

    Ray() : origin(), direction(Vec3(0, 0, 1)) {}
    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d.normalize()) {}

    Vec3 at(float t) const {
        return origin + direction * t;
    }
};

