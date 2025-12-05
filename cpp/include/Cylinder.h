#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "Sphere.h"  // For HitRecord
#include <cmath>
#include <algorithm>

// Cylinder aligned along the Y-axis
struct Cylinder {
    Vec3 center;       // Center of the cylinder (base)
    float radius;      // Radius of the cylinder
    float height;      // Height of the cylinder
    Material material;
    bool capped;       // Whether to render end caps

    Cylinder() 
        : center(Vec3(0.0f, 0.0f, 0.0f))
        , radius(0.5f)
        , height(1.0f)
        , material()
        , capped(true) {}
    
    Cylinder(const Vec3& c, float r, float h, const Material& mat, bool caps = true) 
        : center(c)
        , radius(r)
        , height(h)
        , material(mat)
        , capped(caps) {}

    HitRecord intersect(const Ray& ray) const {
        HitRecord record;
        
        float yMin = center.y;
        float yMax = center.y + height;

        // Transform ray to cylinder's local space (cylinder is at origin)
        Vec3 ro = ray.origin - center;
        Vec3 rd = ray.direction;

        // Solve quadratic for infinite cylinder along Y axis
        // x² + z² = r²
        float a = rd.x * rd.x + rd.z * rd.z;
        float b = 2.0f * (ro.x * rd.x + ro.z * rd.z);
        float c = ro.x * ro.x + ro.z * ro.z - radius * radius;

        float tCylinder = 1e30f;
        Vec3 cylinderNormal;
        bool hitCylinder = false;

        // Check cylinder body
        if (std::abs(a) > 0.0001f) {
            float discriminant = b * b - 4.0f * a * c;
            
            if (discriminant >= 0.0f) {
                float sqrtD = std::sqrt(discriminant);
                float t1 = (-b - sqrtD) / (2.0f * a);
                float t2 = (-b + sqrtD) / (2.0f * a);

                // Check t1
                if (t1 > 0.001f) {
                    Vec3 p = ray.at(t1);
                    if (p.y >= yMin && p.y <= yMax) {
                        tCylinder = t1;
                        Vec3 hitPoint = ray.at(t1);
                        cylinderNormal = Vec3(hitPoint.x - center.x, 0.0f, hitPoint.z - center.z).normalize();
                        hitCylinder = true;
                    }
                }
                
                // Check t2 if t1 didn't work
                if (!hitCylinder && t2 > 0.001f) {
                    Vec3 p = ray.at(t2);
                    if (p.y >= yMin && p.y <= yMax) {
                        tCylinder = t2;
                        Vec3 hitPoint = ray.at(t2);
                        cylinderNormal = Vec3(hitPoint.x - center.x, 0.0f, hitPoint.z - center.z).normalize();
                        hitCylinder = true;
                    }
                }
            }
        }

        // Check end caps if enabled
        float tCap = 1e30f;
        Vec3 capNormal;
        bool hitCap = false;

        if (capped && std::abs(rd.y) > 0.0001f) {
            // Bottom cap (y = yMin)
            float tBottom = (yMin - ray.origin.y) / rd.y;
            if (tBottom > 0.001f) {
                Vec3 p = ray.at(tBottom);
                float dist2 = (p.x - center.x) * (p.x - center.x) + 
                              (p.z - center.z) * (p.z - center.z);
                if (dist2 <= radius * radius) {
                    if (tBottom < tCap) {
                        tCap = tBottom;
                        capNormal = Vec3(0.0f, -1.0f, 0.0f);
                        hitCap = true;
                    }
                }
            }

            // Top cap (y = yMax)
            float tTop = (yMax - ray.origin.y) / rd.y;
            if (tTop > 0.001f) {
                Vec3 p = ray.at(tTop);
                float dist2 = (p.x - center.x) * (p.x - center.x) + 
                              (p.z - center.z) * (p.z - center.z);
                if (dist2 <= radius * radius) {
                    if (tTop < tCap) {
                        tCap = tTop;
                        capNormal = Vec3(0.0f, 1.0f, 0.0f);
                        hitCap = true;
                    }
                }
            }
        }

        // Find the closest valid hit
        float tFinal;
        Vec3 finalNormal;
        
        if (hitCylinder && hitCap) {
            if (tCylinder < tCap) {
                tFinal = tCylinder;
                finalNormal = cylinderNormal;
            } else {
                tFinal = tCap;
                finalNormal = capNormal;
            }
        } else if (hitCylinder) {
            tFinal = tCylinder;
            finalNormal = cylinderNormal;
        } else if (hitCap) {
            tFinal = tCap;
            finalNormal = capNormal;
        } else {
            return record;  // No hit
        }

        record.t = tFinal;
        record.point = ray.at(tFinal);
        record.normal = finalNormal;
        record.material = material;
        record.hit = true;

        return record;
    }
};

