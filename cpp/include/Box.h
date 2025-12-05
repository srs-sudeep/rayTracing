#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "Sphere.h"  // For HitRecord
#include <cmath>
#include <algorithm>

// Axis-Aligned Bounding Box (AABB)
struct Box {
    Vec3 center;
    Vec3 halfSize;  // Half-extents in each dimension
    Material material;

    Box() 
        : center(Vec3(0.0f, 0.0f, 0.0f))
        , halfSize(Vec3(0.5f, 0.5f, 0.5f))
        , material() {}
    
    Box(const Vec3& c, const Vec3& size, const Material& mat) 
        : center(c)
        , halfSize(size * 0.5f)
        , material(mat) {}

    // Create a unit cube centered at a position
    static Box unitCube(const Vec3& center, const Material& mat) {
        return Box(center, Vec3(1.0f, 1.0f, 1.0f), mat);
    }

    // Get min and max corners
    Vec3 getMin() const {
        return Vec3(center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z);
    }

    Vec3 getMax() const {
        return Vec3(center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z);
    }

    HitRecord intersect(const Ray& ray) const {
        HitRecord record;
        
        Vec3 minB = getMin();
        Vec3 maxB = getMax();

        float tmin = -1e30f;
        float tmax = 1e30f;
        Vec3 normalMin, normalMax;

        // X slab
        if (std::abs(ray.direction.x) > 0.0001f) {
            float tx1 = (minB.x - ray.origin.x) / ray.direction.x;
            float tx2 = (maxB.x - ray.origin.x) / ray.direction.x;
            
            float tNear = std::min(tx1, tx2);
            float tFar = std::max(tx1, tx2);
            
            Vec3 nNear = Vec3(tx1 < tx2 ? -1.0f : 1.0f, 0.0f, 0.0f);
            Vec3 nFar = Vec3(tx1 < tx2 ? 1.0f : -1.0f, 0.0f, 0.0f);
            
            if (tNear > tmin) { tmin = tNear; normalMin = nNear; }
            if (tFar < tmax) { tmax = tFar; normalMax = nFar; }
        } else {
            if (ray.origin.x < minB.x || ray.origin.x > maxB.x) {
                return record;
            }
        }

        // Y slab
        if (std::abs(ray.direction.y) > 0.0001f) {
            float ty1 = (minB.y - ray.origin.y) / ray.direction.y;
            float ty2 = (maxB.y - ray.origin.y) / ray.direction.y;
            
            float tNear = std::min(ty1, ty2);
            float tFar = std::max(ty1, ty2);
            
            Vec3 nNear = Vec3(0.0f, ty1 < ty2 ? -1.0f : 1.0f, 0.0f);
            Vec3 nFar = Vec3(0.0f, ty1 < ty2 ? 1.0f : -1.0f, 0.0f);
            
            if (tNear > tmin) { tmin = tNear; normalMin = nNear; }
            if (tFar < tmax) { tmax = tFar; normalMax = nFar; }
        } else {
            if (ray.origin.y < minB.y || ray.origin.y > maxB.y) {
                return record;
            }
        }

        // Z slab
        if (std::abs(ray.direction.z) > 0.0001f) {
            float tz1 = (minB.z - ray.origin.z) / ray.direction.z;
            float tz2 = (maxB.z - ray.origin.z) / ray.direction.z;
            
            float tNear = std::min(tz1, tz2);
            float tFar = std::max(tz1, tz2);
            
            Vec3 nNear = Vec3(0.0f, 0.0f, tz1 < tz2 ? -1.0f : 1.0f);
            Vec3 nFar = Vec3(0.0f, 0.0f, tz1 < tz2 ? 1.0f : -1.0f);
            
            if (tNear > tmin) { tmin = tNear; normalMin = nNear; }
            if (tFar < tmax) { tmax = tFar; normalMax = nFar; }
        } else {
            if (ray.origin.z < minB.z || ray.origin.z > maxB.z) {
                return record;
            }
        }

        // Check if we have a valid intersection
        if (tmin > tmax || tmax < 0.001f) {
            return record;
        }

        // Use the appropriate t value and normal
        float t;
        Vec3 normal;
        
        if (tmin > 0.001f) {
            t = tmin;
            normal = normalMin;
        } else {
            t = tmax;
            normal = normalMax;
        }

        record.t = t;
        record.point = ray.at(t);
        record.normal = normal;
        record.material = material;
        record.hit = true;

        return record;
    }
};

