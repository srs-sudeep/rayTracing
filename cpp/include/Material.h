#pragma once

#include "Vec3.h"

struct Material {
    Vec3 color;
    float ambient;
    float diffuse;
    float specularIntensity;
    float shininess;
    float reflectivity;
    float transparency;      // 0 = opaque, 1 = fully transparent
    float refractiveIndex;   // Index of refraction (1.0 = air, 1.5 = glass, 2.4 = diamond)
    
    Material() 
        : color(Vec3(0.9f, 0.2f, 0.15f))
        , ambient(0.1f)
        , diffuse(0.7f)
        , specularIntensity(0.5f)
        , shininess(32.0f)
        , reflectivity(0.0f)
        , transparency(0.0f)
        , refractiveIndex(1.0f) {}

    Material(const Vec3& col, float spec, float shine)
        : color(col)
        , ambient(0.1f)
        , diffuse(0.7f)
        , specularIntensity(spec)
        , shininess(shine)
        , reflectivity(0.0f)
        , transparency(0.0f)
        , refractiveIndex(1.0f) {}

    // Create a glass-like material
    static Material glass(const Vec3& tint = Vec3(1.0f, 1.0f, 1.0f)) {
        Material m;
        m.color = tint;
        m.ambient = 0.0f;
        m.diffuse = 0.1f;
        m.specularIntensity = 1.0f;
        m.shininess = 256.0f;
        m.reflectivity = 0.1f;
        m.transparency = 0.95f;
        m.refractiveIndex = 1.5f;
        return m;
    }

    // Create a diamond-like material
    static Material diamond() {
        Material m;
        m.color = Vec3(1.0f, 1.0f, 1.0f);
        m.ambient = 0.0f;
        m.diffuse = 0.05f;
        m.specularIntensity = 1.0f;
        m.shininess = 512.0f;
        m.reflectivity = 0.2f;
        m.transparency = 0.95f;
        m.refractiveIndex = 2.4f;
        return m;
    }

    // Create a water-like material
    static Material water() {
        Material m;
        m.color = Vec3(0.8f, 0.9f, 1.0f);
        m.ambient = 0.0f;
        m.diffuse = 0.1f;
        m.specularIntensity = 0.8f;
        m.shininess = 128.0f;
        m.reflectivity = 0.1f;
        m.transparency = 0.9f;
        m.refractiveIndex = 1.33f;
        return m;
    }
};

