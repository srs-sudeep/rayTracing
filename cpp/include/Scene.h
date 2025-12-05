#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"
#include "Camera.h"
#include <vector>
#include <algorithm>
#include <random>

// Scene preset types
enum class ScenePreset {
    SINGLE_SPHERE = 0,
    THREE_SPHERES = 1,
    MIRROR_SPHERES = 2,
    RAINBOW = 3
};

class Scene {
public:
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    Plane groundPlane;
    Camera camera;
    Vec3 backgroundColor;
    Vec3 horizonColor;
    bool showGroundPlane;
    int maxReflectionDepth;
    ScenePreset currentPreset;
    
    // Soft shadow settings
    bool softShadowsEnabled;
    int shadowSamples;
    
    // Random number generator for soft shadows
    mutable std::mt19937 rng;
    mutable std::uniform_real_distribution<float> dist;

    Scene() 
        : backgroundColor(Vec3(0.05f, 0.05f, 0.08f))
        , horizonColor(Vec3(0.12f, 0.12f, 0.15f))
        , showGroundPlane(true)
        , maxReflectionDepth(5)
        , currentPreset(ScenePreset::SINGLE_SPHERE)
        , softShadowsEnabled(false)
        , shadowSamples(8)
        , rng(42)
        , dist(0.0f, 1.0f)
    {
        // Ground plane with subtle reflectivity
        groundPlane.material.reflectivity = 0.15f;
        
        // Default light with radius for soft shadows
        lights.push_back(Light(Vec3(2.0f, 3.0f, -2.0f), Vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.5f));
        
        // Load default scene
        loadPreset(ScenePreset::SINGLE_SPHERE);
    }

    void loadPreset(ScenePreset preset) {
        currentPreset = preset;
        spheres.clear();

        switch (preset) {
            case ScenePreset::SINGLE_SPHERE: {
                // Classic single red sphere
                Material mat(Vec3(0.9f, 0.2f, 0.15f), 0.5f, 32.0f);
                mat.reflectivity = 0.3f;
                spheres.push_back(Sphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f, mat));
                break;
            }

            case ScenePreset::THREE_SPHERES: {
                // Red sphere (center)
                Material redMat(Vec3(0.9f, 0.2f, 0.15f), 0.6f, 64.0f);
                redMat.reflectivity = 0.3f;
                spheres.push_back(Sphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f, redMat));

                // Blue sphere (left)
                Material blueMat(Vec3(0.2f, 0.4f, 0.9f), 0.7f, 128.0f);
                blueMat.reflectivity = 0.5f;
                spheres.push_back(Sphere(Vec3(-2.2f, -0.3f, 0.5f), 0.7f, blueMat));

                // Green sphere (right)
                Material greenMat(Vec3(0.2f, 0.85f, 0.3f), 0.5f, 48.0f);
                greenMat.reflectivity = 0.2f;
                spheres.push_back(Sphere(Vec3(2.0f, -0.5f, 1.0f), 0.5f, greenMat));
                break;
            }

            case ScenePreset::MIRROR_SPHERES: {
                // Chrome mirror sphere (center)
                Material chromeMat(Vec3(0.95f, 0.95f, 0.95f), 1.0f, 256.0f);
                chromeMat.reflectivity = 0.95f;
                spheres.push_back(Sphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f, chromeMat));

                // Gold sphere (left)
                Material goldMat(Vec3(1.0f, 0.84f, 0.0f), 0.9f, 128.0f);
                goldMat.reflectivity = 0.8f;
                spheres.push_back(Sphere(Vec3(-2.0f, -0.4f, 0.3f), 0.6f, goldMat));

                // Rose gold sphere (right)
                Material roseMat(Vec3(0.95f, 0.5f, 0.5f), 0.85f, 96.0f);
                roseMat.reflectivity = 0.7f;
                spheres.push_back(Sphere(Vec3(1.8f, -0.5f, 0.8f), 0.5f, roseMat));

                // Small accent spheres
                Material accentMat(Vec3(0.3f, 0.3f, 0.35f), 1.0f, 256.0f);
                accentMat.reflectivity = 0.9f;
                spheres.push_back(Sphere(Vec3(-0.8f, -0.75f, 1.5f), 0.25f, accentMat));
                spheres.push_back(Sphere(Vec3(0.6f, -0.8f, 1.8f), 0.2f, accentMat));
                break;
            }

            case ScenePreset::RAINBOW: {
                // Rainbow of spheres in an arc
                Vec3 colors[] = {
                    Vec3(0.95f, 0.2f, 0.2f),   // Red
                    Vec3(0.95f, 0.5f, 0.1f),   // Orange
                    Vec3(0.95f, 0.9f, 0.1f),   // Yellow
                    Vec3(0.2f, 0.9f, 0.3f),    // Green
                    Vec3(0.2f, 0.6f, 0.95f),   // Blue
                    Vec3(0.5f, 0.2f, 0.9f),    // Indigo
                    Vec3(0.8f, 0.3f, 0.8f)     // Violet
                };

                float startX = -3.0f;
                float spacing = 1.0f;

                for (int i = 0; i < 7; i++) {
                    Material mat(colors[i], 0.6f, 64.0f);
                    mat.reflectivity = 0.35f;
                    
                    float x = startX + i * spacing;
                    float y = -0.5f + std::sin(i * 0.5f) * 0.3f;
                    float z = 0.5f + std::cos(i * 0.4f) * 0.5f;
                    float radius = 0.4f + (i == 3 ? 0.1f : 0.0f); // Bigger green center
                    
                    spheres.push_back(Sphere(Vec3(x, y, z), radius, mat));
                }
                break;
            }
        }
    }

    HitRecord trace(const Ray& ray) const {
        HitRecord closest;
        closest.t = 1e30f;
        closest.hit = false;

        // Test all spheres
        for (const auto& sphere : spheres) {
            HitRecord hit = sphere.intersect(ray);
            if (hit.hit && hit.t < closest.t) {
                closest = hit;
            }
        }

        // Test ground plane
        if (showGroundPlane) {
            HitRecord planeHit = groundPlane.intersect(ray);
            if (planeHit.hit && planeHit.t < closest.t) {
                closest = planeHit;
            }
        }

        return closest;
    }

    // Check if a point is in shadow (single ray - hard shadows)
    bool isInShadowHard(const Vec3& point, const Vec3& lightPos) const {
        Vec3 toLight = lightPos - point;
        float lightDistance = toLight.length();
        Vec3 lightDir = toLight.normalize();
        
        Ray shadowRay(point + lightDir * 0.001f, lightDir);
        
        // Check all spheres
        for (const auto& sphere : spheres) {
            HitRecord hit = sphere.intersect(shadowRay);
            if (hit.hit && hit.t < lightDistance) {
                return true;
            }
        }
        
        return false;
    }

    // Calculate shadow factor with soft shadows (area lights)
    // Returns 0.0 = fully in shadow, 1.0 = fully lit
    float calculateShadowFactor(const Vec3& point, const Light& light) const {
        if (!softShadowsEnabled || light.radius <= 0.0f) {
            // Hard shadows - simple binary test
            return isInShadowHard(point, light.position) ? 0.3f : 1.0f;
        }
        
        // Soft shadows - multiple samples on the area light
        int litSamples = 0;
        int samples = shadowSamples;
        
        // Use stratified sampling for better distribution
        int sqrtSamples = static_cast<int>(std::sqrt(static_cast<float>(samples)));
        if (sqrtSamples < 2) sqrtSamples = 2;
        
        for (int i = 0; i < sqrtSamples; ++i) {
            for (int j = 0; j < sqrtSamples; ++j) {
                // Stratified random offset within each cell
                float u = (i + dist(rng)) / sqrtSamples;
                float v = (j + dist(rng)) / sqrtSamples;
                
                // Get sample point on area light (disk facing the point)
                Vec3 samplePos = light.getSamplePointDisk(u, v, point);
                
                // Test shadow ray to this sample
                if (!isInShadowHard(point, samplePos)) {
                    litSamples++;
                }
            }
        }
        
        // Calculate soft shadow factor
        int totalSamples = sqrtSamples * sqrtSamples;
        float visibility = static_cast<float>(litSamples) / static_cast<float>(totalSamples);
        
        // Remap to avoid completely black shadows (ambient remains)
        return 0.3f + visibility * 0.7f;
    }

    Vec3 getBackgroundColor(const Ray& ray) const {
        float t = 0.5f * (ray.direction.y + 1.0f);
        t = std::fmax(0.0f, std::fmin(1.0f, t));
        return horizonColor * (1.0f - t) + backgroundColor * t;
    }

    Vec3 calculateLocalLighting(const Ray& ray, const HitRecord& hit) const {
        Vec3 color(0, 0, 0);
        Vec3 viewDir = (ray.origin - hit.point).normalize();

        for (const auto& light : lights) {
            Vec3 lightDir = (light.position - hit.point).normalize();
            
            // Calculate shadow factor (soft or hard depending on settings)
            float shadowFactor = calculateShadowFactor(hit.point, light);
            
            float diff = std::max(0.0f, hit.normal.dot(lightDir));
            Vec3 diffuse = hit.material.color * diff * hit.material.diffuse;
            
            Vec3 halfDir = (lightDir + viewDir).normalize();
            float spec = std::pow(std::max(0.0f, hit.normal.dot(halfDir)), hit.material.shininess);
            Vec3 specular = light.color * spec * hit.material.specularIntensity;
            
            color = color + (diffuse + specular) * light.intensity * shadowFactor;
        }

        Vec3 ambient = hit.material.color * hit.material.ambient;
        color = color + ambient;

        return color;
    }

    Vec3 traceRay(const Ray& ray, int depth) const {
        if (depth >= maxReflectionDepth) {
            return getBackgroundColor(ray);
        }

        HitRecord hit = trace(ray);

        if (!hit.hit) {
            return getBackgroundColor(ray);
        }

        Vec3 localColor = calculateLocalLighting(ray, hit);

        float reflectivity = hit.material.reflectivity;
        if (reflectivity > 0.001f && depth < maxReflectionDepth) {
            Vec3 viewDir = ray.direction;
            Vec3 reflectDir = viewDir.reflect(hit.normal);
            
            Vec3 reflectOrigin = hit.point + hit.normal * 0.001f;
            Ray reflectRay(reflectOrigin, reflectDir);
            
            Vec3 reflectedColor = traceRay(reflectRay, depth + 1);
            
            float cosTheta = std::abs(hit.normal.dot(viewDir * -1.0f));
            float fresnelFactor = reflectivity + (1.0f - reflectivity) * std::pow(1.0f - cosTheta, 3.0f);
            fresnelFactor = std::fmin(1.0f, fresnelFactor);
            
            localColor = localColor * (1.0f - fresnelFactor) + reflectedColor * fresnelFactor;
        }

        return localColor.clamp();
    }

    Vec3 shade(const Ray& ray, const HitRecord& hit) const {
        return traceRay(ray, 0);
    }

    // Update first sphere's material (for UI control)
    void updateMainSphere(float specular, float shininess, float reflectivity) {
        if (!spheres.empty()) {
            spheres[0].material.specularIntensity = specular;
            spheres[0].material.shininess = shininess;
            spheres[0].material.reflectivity = reflectivity;
        }
    }

    void updateSphereColor(float r, float g, float b) {
        if (!spheres.empty()) {
            spheres[0].material.color = Vec3(r, g, b);
        }
    }

    void updateGroundReflectivity(float reflectivity) {
        groundPlane.material.reflectivity = reflectivity;
    }

    // ========================================
    // Light Management
    // ========================================
    
    void updateLight(float x, float y, float z) {
        if (!lights.empty()) {
            lights[0].position = Vec3(x, y, z);
        }
    }

    // Add a new light to the scene
    int addLight(float x, float y, float z, float r, float g, float b, float intensity) {
        lights.push_back(Light(Vec3(x, y, z), Vec3(r, g, b), intensity));
        return static_cast<int>(lights.size() - 1);
    }

    // Remove a light by index
    void removeLight(int index) {
        if (index >= 0 && index < static_cast<int>(lights.size()) && lights.size() > 1) {
            lights.erase(lights.begin() + index);
        }
    }

    // Update a specific light's position
    void setLightPosition(int index, float x, float y, float z) {
        if (index >= 0 && index < static_cast<int>(lights.size())) {
            lights[index].position = Vec3(x, y, z);
        }
    }

    // Update a specific light's color
    void setLightColor(int index, float r, float g, float b) {
        if (index >= 0 && index < static_cast<int>(lights.size())) {
            lights[index].color = Vec3(r, g, b);
        }
    }

    // Update a specific light's intensity
    void setLightIntensity(int index, float intensity) {
        if (index >= 0 && index < static_cast<int>(lights.size())) {
            lights[index].intensity = std::fmax(0.0f, std::fmin(2.0f, intensity));
        }
    }

    // Get light count
    int getLightCount() const {
        return static_cast<int>(lights.size());
    }

    // Get light properties
    float getLightX(int index) const { 
        return (index >= 0 && index < static_cast<int>(lights.size())) ? lights[index].position.x : 0.0f; 
    }
    float getLightY(int index) const { 
        return (index >= 0 && index < static_cast<int>(lights.size())) ? lights[index].position.y : 0.0f; 
    }
    float getLightZ(int index) const { 
        return (index >= 0 && index < static_cast<int>(lights.size())) ? lights[index].position.z : 0.0f; 
    }
    float getLightR(int index) const { 
        return (index >= 0 && index < static_cast<int>(lights.size())) ? lights[index].color.x : 1.0f; 
    }
    float getLightG(int index) const { 
        return (index >= 0 && index < static_cast<int>(lights.size())) ? lights[index].color.y : 1.0f; 
    }
    float getLightB(int index) const { 
        return (index >= 0 && index < static_cast<int>(lights.size())) ? lights[index].color.z : 1.0f; 
    }
    float getLightIntensity(int index) const { 
        return (index >= 0 && index < static_cast<int>(lights.size())) ? lights[index].intensity : 1.0f; 
    }

    // Reset to single default light
    void resetLights() {
        lights.clear();
        lights.push_back(Light(Vec3(2.0f, 3.0f, -2.0f), Vec3(1.0f, 1.0f, 1.0f), 1.0f));
    }

    void updateCamera(float posX, float posY, float posZ) {
        camera.position = Vec3(posX, posY, posZ);
        camera.updateBasis();
    }

    void orbitCamera(float deltaX, float deltaY) {
        camera.orbit(deltaX, deltaY);
    }

    void zoomCamera(float delta) {
        camera.zoom(delta);
    }

    void setShowGroundPlane(bool show) {
        showGroundPlane = show;
    }

    void setGridScale(float scale) {
        groundPlane.gridScale = scale;
    }

    void setShowGrid(bool show) {
        groundPlane.showGrid = show;
    }

    void setMaxReflectionDepth(int depth) {
        maxReflectionDepth = std::max(1, std::min(10, depth));
    }

    int getSphereCount() const {
        return static_cast<int>(spheres.size());
    }

    // Camera FOV and target
    void setCameraFov(float fov) {
        camera.setFov(fov);
    }

    void setCameraTarget(float x, float y, float z) {
        camera.setTarget(x, y, z);
    }

    float getCameraFov() const { return camera.getFov(); }
    float getCameraTargetX() const { return camera.getTargetX(); }
    float getCameraTargetY() const { return camera.getTargetY(); }
    float getCameraTargetZ() const { return camera.getTargetZ(); }

    // ========================================
    // Soft Shadow Settings
    // ========================================
    
    void setSoftShadows(bool enabled) {
        softShadowsEnabled = enabled;
    }

    bool getSoftShadows() const {
        return softShadowsEnabled;
    }

    void setShadowSamples(int samples) {
        shadowSamples = std::max(1, std::min(64, samples));
    }

    int getShadowSamples() const {
        return shadowSamples;
    }

    void setLightRadius(int index, float radius) {
        if (index >= 0 && index < static_cast<int>(lights.size())) {
            lights[index].radius = std::fmax(0.0f, std::fmin(2.0f, radius));
        }
    }

    float getLightRadius(int index) const {
        return (index >= 0 && index < static_cast<int>(lights.size())) ? lights[index].radius : 0.5f;
    }
};
