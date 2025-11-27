#pragma once

#include "math.hpp"
#include "scene.hpp"
#include <vector>
#include <cstdint>
#include <algorithm>

// ============================================================================
// Renderer - Ray tracing renderer
// ============================================================================

class Renderer {
public:
    int maxBounces;
    bool showGrid;
    int gridSize;

    Renderer() 
        : maxBounces(3)
        , showGrid(false)
        , gridSize(32) {}

    // Shade a hit point using Blinn-Phong
    Vec3 shade(const Scene& scene, const Ray& ray, const HitRecord& hit) const {
        Vec3 color(0, 0, 0);
        Vec3 viewDir = (ray.origin - hit.point).normalize();

        // Ambient
        Vec3 ambient = hit.material.color * scene.ambientColor * scene.ambientIntensity;
        color = color + ambient;

        // For each light
        for (const auto& light : scene.lights) {
            Vec3 lightDir = light.getDirection(hit.point);
            
            // Shadow check
            Ray shadowRay(hit.point + hit.normal * 0.001f, lightDir);
            HitRecord shadowHit;
            float lightDist = light.getDistance(hit.point);
            
            if (scene.intersect(shadowRay, 0.001f, lightDist, shadowHit)) {
                continue; // In shadow
            }

            // Diffuse (Lambertian)
            float diff = std::max(0.0f, hit.normal.dot(lightDir));
            Vec3 diffuse = hit.material.color * light.color * diff * light.intensity * 0.7f;
            color = color + diffuse;

            // Specular (Blinn-Phong)
            Vec3 halfDir = (lightDir + viewDir).normalize();
            float spec = std::pow(std::max(0.0f, hit.normal.dot(halfDir)), hit.material.shininess);
            Vec3 specular = light.color * spec * hit.material.specularIntensity * light.intensity;
            color = color + specular;
        }

        return color;
    }

    // Trace a ray through the scene
    Vec3 trace(const Scene& scene, const Ray& ray, int depth = 0) const {
        if (depth > maxBounces) {
            return scene.backgroundColor;
        }

        HitRecord hit;
        if (scene.intersect(ray, 0.001f, 1e30f, hit)) {
            Vec3 color = shade(scene, ray, hit);

            // Reflections
            if (hit.material.reflectivity > 0.001f && depth < maxBounces) {
                Vec3 reflectDir = ray.direction.reflect(hit.normal);
                Ray reflectRay(hit.point + hit.normal * 0.001f, reflectDir);
                Vec3 reflectColor = trace(scene, reflectRay, depth + 1);
                color = Vec3::lerp(color, reflectColor, hit.material.reflectivity);
            }

            // Clamp color
            color.x = std::min(1.0f, color.x);
            color.y = std::min(1.0f, color.y);
            color.z = std::min(1.0f, color.z);

            return color;
        }

        return scene.backgroundColor;
    }

    // Apply grid overlay to a pixel
    Vec3 applyGrid(const Vec3& color, int x, int y, int width, int height) const {
        if (!showGrid) return color;

        // Draw grid lines
        bool isGridLine = (x % gridSize == 0) || (y % gridSize == 0);
        bool isCenterX = (x == width / 2);
        bool isCenterY = (y == height / 2);

        if (isCenterX || isCenterY) {
            // Center lines - more visible
            return Vec3::lerp(color, Vec3(0.4f, 0.4f, 0.5f), 0.4f);
        } else if (isGridLine) {
            // Regular grid lines - subtle
            return Vec3::lerp(color, Vec3(0.3f, 0.3f, 0.35f), 0.15f);
        }

        return color;
    }

    // Render the entire scene
    std::vector<uint8_t> render(const Scene& scene, int width, int height) const {
        std::vector<uint8_t> buffer(width * height * 4);

        // Update camera aspect ratio
        Scene& mutableScene = const_cast<Scene&>(scene);
        mutableScene.camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Normalized coordinates (0 to 1)
                float u = static_cast<float>(x) / static_cast<float>(width);
                float v = 1.0f - static_cast<float>(y) / static_cast<float>(height);

                // Get ray from camera
                Ray ray = scene.camera.getRay(u, v);

                // Trace ray
                Vec3 color = trace(scene, ray);

                // Apply grid overlay
                color = applyGrid(color, x, y, width, height);

                // Gamma correction (approximate)
                color.x = std::sqrt(color.x);
                color.y = std::sqrt(color.y);
                color.z = std::sqrt(color.z);

                // Write to buffer
                int index = (y * width + x) * 4;
                buffer[index + 0] = static_cast<uint8_t>(std::min(255.0f, color.x * 255.0f));
                buffer[index + 1] = static_cast<uint8_t>(std::min(255.0f, color.y * 255.0f));
                buffer[index + 2] = static_cast<uint8_t>(std::min(255.0f, color.z * 255.0f));
                buffer[index + 3] = 255;
            }
        }

        return buffer;
    }
};

