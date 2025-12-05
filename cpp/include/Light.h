#pragma once

#include "Vec3.h"

struct Light {
    Vec3 position;
    Vec3 color;
    float intensity;
    float radius;  // For area light / soft shadows (0 = point light)

    Light() 
        : position(Vec3(2.0f, 2.0f, -1.0f))
        , color(Vec3(1.0f, 1.0f, 1.0f))
        , intensity(1.0f)
        , radius(0.5f) {}

    Light(const Vec3& pos, const Vec3& col, float intens)
        : position(pos)
        , color(col)
        , intensity(intens)
        , radius(0.5f) {}

    Light(const Vec3& pos, const Vec3& col, float intens, float rad)
        : position(pos)
        , color(col)
        , intensity(intens)
        , radius(rad) {}

    // Get a random point on the area light surface (sphere)
    // Uses stratified sampling for better distribution
    Vec3 getSamplePoint(float u, float v) const {
        if (radius <= 0.0f) return position;
        
        // Convert uniform random [0,1] to spherical coordinates
        float theta = 2.0f * 3.14159265f * u;
        float phi = std::acos(1.0f - 2.0f * v);
        
        // Convert to Cartesian offset
        float x = radius * std::sin(phi) * std::cos(theta);
        float y = radius * std::sin(phi) * std::sin(theta);
        float z = radius * std::cos(phi);
        
        return position + Vec3(x, y, z);
    }

    // Get a sample point on a disk facing a target point (more efficient)
    Vec3 getSamplePointDisk(float u, float v, const Vec3& target) const {
        if (radius <= 0.0f) return position;
        
        // Direction from light to target
        Vec3 toTarget = (target - position).normalize();
        
        // Create orthonormal basis on disk
        Vec3 up = (std::abs(toTarget.y) < 0.9f) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
        Vec3 right = toTarget.cross(up).normalize();
        up = right.cross(toTarget).normalize();
        
        // Random point in unit disk (using concentric mapping for uniform distribution)
        float r = std::sqrt(u);
        float theta = 2.0f * 3.14159265f * v;
        float dx = r * std::cos(theta);
        float dy = r * std::sin(theta);
        
        return position + right * (dx * radius) + up * (dy * radius);
    }
};
