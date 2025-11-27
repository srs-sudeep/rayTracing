#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "Sphere.h"
#include <cmath>

struct Plane {
    Vec3 point;      // A point on the plane
    Vec3 normal;     // Plane normal (pointing up)
    Material material;
    bool showGrid;
    float gridScale;
    float gridThickness;

    Plane() 
        : point(Vec3(0.0f, -1.0f, 0.0f))
        , normal(Vec3(0.0f, 1.0f, 0.0f))
        , material()
        , showGrid(true)
        , gridScale(1.0f)
        , gridThickness(0.02f)
    {
        // Dark gray floor
        material.color = Vec3(0.15f, 0.15f, 0.17f);
        material.specularIntensity = 0.1f;
        material.shininess = 16.0f;
    }

    HitRecord intersect(const Ray& ray) const {
        HitRecord record;
        
        float denom = normal.dot(ray.direction);
        
        // Check if ray is parallel to plane
        if (std::abs(denom) < 0.0001f) {
            return record;
        }

        Vec3 p0l0 = point - ray.origin;
        float t = p0l0.dot(normal) / denom;

        // Check if intersection is in front of ray
        if (t < 0.001f) {
            return record;
        }

        record.t = t;
        record.point = ray.at(t);
        record.normal = normal;
        record.material = material;
        record.hit = true;

        // Apply grid pattern if enabled
        if (showGrid) {
            applyGridPattern(record);
        }

        return record;
    }

private:
    void applyGridPattern(HitRecord& record) const {
        float x = record.point.x;
        float z = record.point.z;

        // Scale coordinates
        float scaledX = x / gridScale;
        float scaledZ = z / gridScale;

        // Calculate grid lines
        float lineX = std::abs(scaledX - std::round(scaledX));
        float lineZ = std::abs(scaledZ - std::round(scaledZ));

        // Fade grid based on distance (like Blender)
        float distance = std::sqrt(x * x + z * z);
        float fade = std::exp(-distance * 0.05f);
        fade = std::fmax(0.0f, std::fmin(1.0f, fade));

        // Check if on a grid line
        bool onMainAxisX = std::abs(x) < gridThickness * 2.0f;
        bool onMainAxisZ = std::abs(z) < gridThickness * 2.0f;
        bool onGridLine = lineX < gridThickness || lineZ < gridThickness;

        // Major grid lines (every 5 units)
        float majorLineX = std::abs((scaledX / 5.0f) - std::round(scaledX / 5.0f));
        float majorLineZ = std::abs((scaledZ / 5.0f) - std::round(scaledZ / 5.0f));
        bool onMajorLine = majorLineX < gridThickness * 0.6f || majorLineZ < gridThickness * 0.6f;

        // Base color (dark floor)
        Vec3 baseColor = material.color;
        Vec3 gridColor = baseColor;

        if (onMainAxisX) {
            // Z-axis line (green in Blender, we'll use teal)
            gridColor = Vec3(0.2f, 0.5f, 0.5f) * fade + baseColor * (1.0f - fade);
        } else if (onMainAxisZ) {
            // X-axis line (red in Blender)
            gridColor = Vec3(0.5f, 0.2f, 0.2f) * fade + baseColor * (1.0f - fade);
        } else if (onMajorLine) {
            // Major grid lines - brighter
            float brightness = 0.25f * fade;
            gridColor = Vec3(
                baseColor.x + brightness,
                baseColor.y + brightness,
                baseColor.z + brightness
            );
        } else if (onGridLine) {
            // Minor grid lines - subtle
            float brightness = 0.08f * fade;
            gridColor = Vec3(
                baseColor.x + brightness,
                baseColor.y + brightness,
                baseColor.z + brightness
            );
        }

        record.material.color = gridColor;
    }
};

