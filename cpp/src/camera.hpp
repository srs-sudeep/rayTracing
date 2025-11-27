#pragma once

#include "math.hpp"

// ============================================================================
// Camera - Virtual camera for ray generation
// ============================================================================

struct Camera {
    Vec3 position;
    Vec3 target;
    Vec3 up;
    float fov;          // Field of view in degrees
    float aspectRatio;

    // Computed basis vectors
    Vec3 forward;
    Vec3 right;
    Vec3 upDir;

    Camera()
        : position(0, 0, -3)
        , target(0, 0, 0)
        , up(0, 1, 0)
        , fov(60.0f)
        , aspectRatio(1.0f) {
        updateBasis();
    }

    void updateBasis() {
        forward = (target - position).normalize();
        right = forward.cross(up).normalize();
        upDir = right.cross(forward).normalize();
    }

    void setPosition(float x, float y, float z) {
        position = Vec3(x, y, z);
        updateBasis();
    }

    void setTarget(float x, float y, float z) {
        target = Vec3(x, y, z);
        updateBasis();
    }

    void setFov(float degrees) {
        fov = degrees;
    }

    Ray getRay(float u, float v) const {
        // Convert FOV to radians and calculate scale
        float theta = fov * 3.14159265f / 180.0f;
        float halfHeight = std::tan(theta / 2.0f);
        float halfWidth = aspectRatio * halfHeight;

        // Calculate ray direction
        Vec3 horizontal = right * (2.0f * halfWidth);
        Vec3 vertical = upDir * (2.0f * halfHeight);
        
        Vec3 lowerLeftCorner = position + forward - horizontal * 0.5f - vertical * 0.5f;
        Vec3 rayTarget = lowerLeftCorner + horizontal * u + vertical * v;
        Vec3 rayDir = rayTarget - position;

        return Ray(position, rayDir);
    }
};

