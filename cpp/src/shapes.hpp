#pragma once

#include "math.hpp"
#include "materials.hpp"
#include <algorithm>

// ============================================================================
// HitRecord - Information about a ray-object intersection
// ============================================================================

struct HitRecord {
    float t;            // Distance along ray
    Vec3 point;         // Hit point
    Vec3 normal;        // Surface normal at hit point
    Material material;  // Material at hit point
    bool hit;           // Whether there was a hit

    HitRecord() : t(0), hit(false) {}
};

// ============================================================================
// Sphere
// ============================================================================

struct Sphere {
    Vec3 center;
    float radius;
    Material material;

    Sphere() : center(0, 0, 0), radius(1.0f) {}
    
    Sphere(const Vec3& c, float r, const Material& mat)
        : center(c), radius(r), material(mat) {}

    bool intersect(const Ray& ray, float tMin, float tMax, HitRecord& rec) const {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return false;
        }

        float sqrtD = std::sqrt(discriminant);
        float t = (-b - sqrtD) / (2.0f * a);
        
        if (t < tMin || t > tMax) {
            t = (-b + sqrtD) / (2.0f * a);
            if (t < tMin || t > tMax) {
                return false;
            }
        }

        rec.t = t;
        rec.point = ray.at(t);
        rec.normal = (rec.point - center).normalize();
        rec.material = material;
        rec.hit = true;

        return true;
    }
};

// ============================================================================
// Plane - Infinite plane
// ============================================================================

struct Plane {
    Vec3 point;     // A point on the plane
    Vec3 normal;    // Normal vector
    Material material;

    Plane() : point(0, -1, 0), normal(0, 1, 0) {}

    Plane(const Vec3& p, const Vec3& n, const Material& mat)
        : point(p), normal(n.normalize()), material(mat) {}

    bool intersect(const Ray& ray, float tMin, float tMax, HitRecord& rec) const {
        float denom = normal.dot(ray.direction);
        
        // Check if ray is parallel to plane
        if (std::abs(denom) < 1e-6f) {
            return false;
        }

        Vec3 diff = point - ray.origin;
        float t = diff.dot(normal) / denom;

        if (t < tMin || t > tMax) {
            return false;
        }

        rec.t = t;
        rec.point = ray.at(t);
        rec.normal = normal;
        rec.material = material;
        rec.hit = true;

        return true;
    }
};

