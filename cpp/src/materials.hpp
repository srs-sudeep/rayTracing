#pragma once

#include "math.hpp"

// ============================================================================
// Material - Surface material properties
// ============================================================================

struct Material {
    Vec3 color;              // Base color (albedo)
    float specularIntensity; // How shiny (0 = matte, 1 = very shiny)
    float shininess;         // Specular exponent (higher = tighter highlight)
    float reflectivity;      // Mirror reflection amount (0-1)
    float transparency;      // Transparency amount (0-1)
    float refractiveIndex;   // Index of refraction (glass ~1.5)

    Material() 
        : color(0.8f, 0.8f, 0.8f)
        , specularIntensity(0.5f)
        , shininess(32.0f)
        , reflectivity(0.0f)
        , transparency(0.0f)
        , refractiveIndex(1.0f) {}

    Material(const Vec3& col, float spec, float shin)
        : color(col)
        , specularIntensity(spec)
        , shininess(shin)
        , reflectivity(0.0f)
        , transparency(0.0f)
        , refractiveIndex(1.0f) {}

    // Preset materials
    static Material plastic(const Vec3& color) {
        Material m;
        m.color = color;
        m.specularIntensity = 0.3f;
        m.shininess = 16.0f;
        return m;
    }

    static Material metal(const Vec3& color) {
        Material m;
        m.color = color;
        m.specularIntensity = 0.9f;
        m.shininess = 128.0f;
        m.reflectivity = 0.6f;
        return m;
    }

    static Material glass() {
        Material m;
        m.color = Vec3(1.0f, 1.0f, 1.0f);
        m.specularIntensity = 1.0f;
        m.shininess = 256.0f;
        m.transparency = 0.9f;
        m.refractiveIndex = 1.5f;
        return m;
    }

    static Material mirror() {
        Material m;
        m.color = Vec3(0.95f, 0.95f, 0.95f);
        m.specularIntensity = 1.0f;
        m.shininess = 512.0f;
        m.reflectivity = 0.95f;
        return m;
    }
};

