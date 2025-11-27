#pragma once

#include "Scene.h"
#include <vector>
#include <cstdint>

class Renderer {
public:
    int width;
    int height;

    Renderer() : width(512), height(512) {}

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
};
