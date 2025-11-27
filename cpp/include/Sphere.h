#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"

struct HitRecord {
    float t;
    Vec3 point;
    Vec3 normal;
    Material material;
    bool hit;

    HitRecord() : t(-1), hit(false) {}
};

struct Sphere {
    Vec3 center;
    float radius;
    Material material;

    Sphere() : center(), radius(1.0f), material() {}
    
    Sphere(const Vec3& c, float r, const Material& mat) 
        : center(c), radius(r), material(mat) {}

    HitRecord intersect(const Ray& ray) const {
        HitRecord record;
        
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return record;
        }

        float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
        if (t <= 0.001f) {
            t = (-b + std::sqrt(discriminant)) / (2.0f * a);
            if (t <= 0.001f) {
                return record;
            }
        }

        record.t = t;
        record.point = ray.at(t);
        record.normal = (record.point - center).normalize();
        record.material = material;
        record.hit = true;

        return record;
    }
};

