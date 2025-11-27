#pragma once

#include "math.hpp"
#include "shapes.hpp"
#include "lights.hpp"
#include "camera.hpp"
#include <vector>
#include <algorithm>

// ============================================================================
// Scene - Contains all objects, lights, and camera
// ============================================================================

struct Scene {
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<Light> lights;
    Camera camera;
    Vec3 backgroundColor;
    Vec3 ambientColor;
    float ambientIntensity;

    Scene() 
        : backgroundColor(0.08f, 0.08f, 0.1f)
        , ambientColor(1.0f, 1.0f, 1.0f)
        , ambientIntensity(0.1f) {}

    void clear() {
        spheres.clear();
        planes.clear();
        lights.clear();
    }

    void addSphere(const Sphere& sphere) {
        spheres.push_back(sphere);
    }

    void addPlane(const Plane& plane) {
        planes.push_back(plane);
    }

    void addLight(const Light& light) {
        lights.push_back(light);
    }

    // Find closest intersection with any object
    bool intersect(const Ray& ray, float tMin, float tMax, HitRecord& rec) const {
        HitRecord tempRec;
        bool hitAnything = false;
        float closestSoFar = tMax;

        // Check spheres
        for (const auto& sphere : spheres) {
            if (sphere.intersect(ray, tMin, closestSoFar, tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        // Check planes
        for (const auto& plane : planes) {
            if (plane.intersect(ray, tMin, closestSoFar, tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
};

