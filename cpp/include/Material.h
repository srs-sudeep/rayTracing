#pragma once

#include "Vec3.h"

struct Material {
    Vec3 color;
    float ambient;
    float diffuse;
    float specularIntensity;
    float shininess;
    float reflectivity;
    
    Material() 
        : color(Vec3(0.9f, 0.2f, 0.15f))
        , ambient(0.1f)
        , diffuse(0.7f)
        , specularIntensity(0.5f)
        , shininess(32.0f)
        , reflectivity(0.0f) {}

    Material(const Vec3& col, float spec, float shine)
        : color(col)
        , ambient(0.1f)
        , diffuse(0.7f)
        , specularIntensity(spec)
        , shininess(shine)
        , reflectivity(0.0f) {}
};

