#pragma once

#include "math.hpp"

// ============================================================================
// Light Types
// ============================================================================

enum class LightType {
    Point,
    Directional
};

// ============================================================================
// Light - Light source in the scene
// ============================================================================

struct Light {
    LightType type;
    Vec3 position;      // Position for point light
    Vec3 direction;     // Direction for directional light
    Vec3 color;         // Light color
    float intensity;    // Light brightness

    Light() 
        : type(LightType::Point)
        , position(2.0f, 2.0f, -1.0f)
        , direction(0.0f, -1.0f, 0.0f)
        , color(1.0f, 1.0f, 1.0f)
        , intensity(1.0f) {}

    // Get direction from hit point to light
    Vec3 getDirection(const Vec3& hitPoint) const {
        if (type == LightType::Directional) {
            return (direction * -1.0f).normalize();
        }
        return (position - hitPoint).normalize();
    }

    // Get distance to light (infinite for directional)
    float getDistance(const Vec3& hitPoint) const {
        if (type == LightType::Directional) {
            return 1e30f; // Very large number
        }
        return (position - hitPoint).length();
    }
};

