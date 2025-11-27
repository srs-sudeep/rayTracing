#pragma once

#include "Scene.h"
#include <vector>
#include <cstdint>

class Renderer {
public:
    int width;
    int height;
    bool showGrid;
    int gridSize;

    Renderer() : width(512), height(512), showGrid(false), gridSize(64) {}

    std::vector<uint8_t> render(Scene& scene) {
        std::vector<uint8_t> buffer(width * height * 4);
        
        scene.camera.setAspectRatio(static_cast<float>(width) / height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Normalized coordinates [-1, 1]
                float u = (2.0f * x / width - 1.0f);
                float v = (1.0f - 2.0f * y / height);

                Ray ray = scene.camera.getRay(u, v);
                HitRecord hit = scene.trace(ray);
                Vec3 color = scene.shade(ray, hit);

                // Apply grid overlay if enabled and no hit
                if (showGrid && !hit.hit) {
                    color = applyGrid(x, y, color);
                }

                // Write to buffer
                int index = (y * width + x) * 4;
                buffer[index + 0] = static_cast<uint8_t>(color.x * 255.0f);
                buffer[index + 1] = static_cast<uint8_t>(color.y * 255.0f);
                buffer[index + 2] = static_cast<uint8_t>(color.z * 255.0f);
                buffer[index + 3] = 255;
            }
        }

        return buffer;
    }

private:
    Vec3 applyGrid(int x, int y, const Vec3& bgColor) const {
        // Create a subtle grid pattern
        bool onGridLine = (x % gridSize == 0) || (y % gridSize == 0);
        bool onCenterX = (x == width / 2);
        bool onCenterY = (y == height / 2);

        if (onCenterX || onCenterY) {
            // Center lines - slightly brighter
            return Vec3(
                bgColor.x + 0.08f,
                bgColor.y + 0.08f,
                bgColor.z + 0.1f
            ).clamp();
        } else if (onGridLine) {
            // Regular grid lines
            return Vec3(
                bgColor.x + 0.03f,
                bgColor.y + 0.03f,
                bgColor.z + 0.04f
            ).clamp();
        }

        return bgColor;
    }
};

