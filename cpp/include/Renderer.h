#pragma once

#include "Scene.h"
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <random>

// Anti-aliasing levels
enum class AALevel {
    NONE = 0,     // 1 sample per pixel
    AA_2X = 1,    // 2x2 = 4 samples per pixel
    AA_4X = 2     // 4x4 = 16 samples per pixel
};

class Renderer {
public:
    int width;
    int height;
    AALevel antiAliasing;
    
    // Random number generator for jittered sampling
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;

    Renderer() : width(512), height(512), antiAliasing(AALevel::NONE), rng(42), dist(0.0f, 1.0f) {}

    void setAntiAliasing(int level) {
        switch (level) {
            case 0: antiAliasing = AALevel::NONE; break;
            case 1: antiAliasing = AALevel::AA_2X; break;
            case 2: antiAliasing = AALevel::AA_4X; break;
            default: antiAliasing = AALevel::NONE; break;
        }
    }

    int getAntiAliasing() const {
        return static_cast<int>(antiAliasing);
    }

    // Get the number of samples based on AA level
    int getSamplesPerPixel() const {
        switch (antiAliasing) {
            case AALevel::NONE: return 1;
            case AALevel::AA_2X: return 4;   // 2x2
            case AALevel::AA_4X: return 16;  // 4x4
            default: return 1;
        }
    }

    // Get grid size for stratified sampling
    int getSampleGridSize() const {
        switch (antiAliasing) {
            case AALevel::NONE: return 1;
            case AALevel::AA_2X: return 2;
            case AALevel::AA_4X: return 4;
            default: return 1;
        }
    }

    std::vector<uint8_t> render(Scene& scene) {
        std::vector<uint8_t> buffer(width * height * 4);
        
        scene.camera.setAspectRatio(static_cast<float>(width) / height);

        int gridSize = getSampleGridSize();
        int totalSamples = gridSize * gridSize;
        float invSamples = 1.0f / static_cast<float>(totalSamples);
        float subpixelSize = 1.0f / static_cast<float>(gridSize);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Vec3 colorAccum(0, 0, 0);

                if (antiAliasing == AALevel::NONE) {
                    // No AA - single sample at pixel center
                    float u = (2.0f * x / width - 1.0f);
                    float v = (1.0f - 2.0f * y / height);
                    
                    Ray ray = scene.camera.getRay(u, v);
                    colorAccum = scene.traceRay(ray, 0);
                } else {
                    // Stratified sampling with jitter
                    for (int sy = 0; sy < gridSize; ++sy) {
                        for (int sx = 0; sx < gridSize; ++sx) {
                            // Calculate subpixel offset with jitter
                            float jitterX = dist(rng);
                            float jitterY = dist(rng);
                            
                            // Position within pixel (0 to 1) with stratified jitter
                            float subX = (sx + jitterX) * subpixelSize;
                            float subY = (sy + jitterY) * subpixelSize;
                            
                            // Convert to normalized coordinates [-1, 1]
                            float u = (2.0f * (x + subX) / width - 1.0f);
                            float v = (1.0f - 2.0f * (y + subY) / height);
                            
                            Ray ray = scene.camera.getRay(u, v);
                            Vec3 sampleColor = scene.traceRay(ray, 0);
                            
                            colorAccum = colorAccum + sampleColor;
                        }
                    }
                    
                    // Average all samples
                    colorAccum = colorAccum * invSamples;
                }

                // Clamp and write to buffer
                Vec3 finalColor = colorAccum.clamp();
                
                int index = (y * width + x) * 4;
                buffer[index + 0] = static_cast<uint8_t>(finalColor.x * 255.0f);
                buffer[index + 1] = static_cast<uint8_t>(finalColor.y * 255.0f);
                buffer[index + 2] = static_cast<uint8_t>(finalColor.z * 255.0f);
                buffer[index + 3] = 255;
            }
        }

        return buffer;
    }
};
