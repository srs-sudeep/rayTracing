#pragma once

#include "Vec3.h"
#include "Ray.h"
#include <cmath>

struct Camera {
    Vec3 position;
    Vec3 target;
    Vec3 up;
    float fov;        // Field of view in degrees
    float aspectRatio;

    // Computed basis vectors
    Vec3 forward;
    Vec3 right;
    Vec3 upDir;
    float viewportHeight;
    float viewportWidth;

    Camera()
        : position(Vec3(0.0f, 0.0f, -3.0f))
        , target(Vec3(0.0f, 0.0f, 0.0f))
        , up(Vec3(0.0f, 1.0f, 0.0f))
        , fov(60.0f)
        , aspectRatio(1.0f) {
        updateBasis();
    }

    void updateBasis() {
        forward = (target - position).normalize();
        right = forward.cross(up).normalize();
        upDir = right.cross(forward).normalize();

        float theta = fov * 3.14159265f / 180.0f;
        viewportHeight = 2.0f * std::tan(theta / 2.0f);
        viewportWidth = viewportHeight * aspectRatio;
    }

    Ray getRay(float u, float v) const {
        // u, v are in range [-1, 1]
        Vec3 horizontal = right * (u * viewportWidth * 0.5f);
        Vec3 vertical = upDir * (v * viewportHeight * 0.5f);
        Vec3 direction = forward + horizontal + vertical;
        return Ray(position, direction);
    }

    void setAspectRatio(float ratio) {
        aspectRatio = ratio;
        updateBasis();
    }

    void orbit(float deltaX, float deltaY) {
        // Orbit around target
        Vec3 offset = position - target;
        float radius = offset.length();
        
        // Convert to spherical coordinates
        float theta = std::atan2(offset.x, offset.z);
        float phi = std::acos(offset.y / radius);
        
        // Apply rotation
        theta += deltaX * 0.01f;
        phi += deltaY * 0.01f;
        
        // Clamp phi to avoid gimbal lock
        phi = std::fmax(0.1f, std::fmin(3.04f, phi));
        
        // Convert back to cartesian
        position.x = target.x + radius * std::sin(phi) * std::sin(theta);
        position.y = target.y + radius * std::cos(phi);
        position.z = target.z + radius * std::sin(phi) * std::cos(theta);
        
        updateBasis();
    }

    void zoom(float delta) {
        Vec3 direction = (position - target).normalize();
        float distance = (position - target).length();
        distance = std::fmax(1.0f, std::fmin(20.0f, distance + delta));
        position = target + direction * distance;
        updateBasis();
    }

    void setFov(float newFov) {
        fov = std::fmax(10.0f, std::fmin(120.0f, newFov));
        updateBasis();
    }

    void setTarget(float x, float y, float z) {
        target = Vec3(x, y, z);
        updateBasis();
    }

    float getFov() const { return fov; }
    float getTargetX() const { return target.x; }
    float getTargetY() const { return target.y; }
    float getTargetZ() const { return target.z; }
};

