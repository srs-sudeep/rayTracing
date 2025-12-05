#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Cylinder.h"
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
    RAINBOW = 3,
    GLASS_SPHERES = 4,
    PRIMITIVES = 5
};

class Scene {
public:
    std::vector<Sphere> spheres;
    std::vector<Box> boxes;
    std::vector<Cylinder> cylinders;
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
        boxes.clear();
        cylinders.clear();

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

            case ScenePreset::GLASS_SPHERES: {
                // Large glass sphere (center)
                Material glassMat = Material::glass();
                spheres.push_back(Sphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f, glassMat));

                // Diamond sphere (left)
                Material diamondMat = Material::diamond();
                spheres.push_back(Sphere(Vec3(-2.0f, -0.4f, 0.5f), 0.6f, diamondMat));

                // Tinted glass sphere (right) - blue tint
                Material blueTint = Material::glass(Vec3(0.85f, 0.9f, 1.0f));
                spheres.push_back(Sphere(Vec3(1.8f, -0.5f, 0.8f), 0.5f, blueTint));

                // Solid red sphere behind glass (to see refraction distortion)
                Material redMat(Vec3(0.95f, 0.15f, 0.15f), 0.6f, 64.0f);
                redMat.reflectivity = 0.2f;
                spheres.push_back(Sphere(Vec3(0.0f, -0.3f, -2.5f), 0.7f, redMat));

                // Small water sphere
                Material waterMat = Material::water();
                spheres.push_back(Sphere(Vec3(-0.7f, -0.7f, 1.5f), 0.3f, waterMat));
                break;
            }

            case ScenePreset::PRIMITIVES: {
                // Showcase different primitive types

                // Central metallic sphere
                Material sphereMat(Vec3(0.9f, 0.3f, 0.2f), 0.8f, 128.0f);
                sphereMat.reflectivity = 0.4f;
                spheres.push_back(Sphere(Vec3(0.0f, 0.0f, 0.0f), 0.8f, sphereMat));

                // Blue cube on the left
                Material boxMat(Vec3(0.2f, 0.4f, 0.9f), 0.7f, 64.0f);
                boxMat.reflectivity = 0.3f;
                boxes.push_back(Box(Vec3(-2.2f, -0.35f, 0.3f), Vec3(1.3f, 1.3f, 1.3f), boxMat));

                // Green cylinder on the right
                Material cylMat(Vec3(0.2f, 0.85f, 0.4f), 0.6f, 48.0f);
                cylMat.reflectivity = 0.25f;
                cylinders.push_back(Cylinder(Vec3(2.0f, -1.0f, 0.5f), 0.5f, 1.4f, cylMat));

                // Small golden sphere behind
                Material goldMat(Vec3(1.0f, 0.84f, 0.0f), 0.9f, 128.0f);
                goldMat.reflectivity = 0.7f;
                spheres.push_back(Sphere(Vec3(0.0f, -0.5f, -2.0f), 0.5f, goldMat));

                // Glass cube in front right
                Material glassMat = Material::glass();
                boxes.push_back(Box(Vec3(1.0f, -0.6f, 1.8f), Vec3(0.8f, 0.8f, 0.8f), glassMat));

                // Purple cylinder behind left
                Material purpleMat(Vec3(0.6f, 0.2f, 0.9f), 0.5f, 32.0f);
                purpleMat.reflectivity = 0.2f;
                cylinders.push_back(Cylinder(Vec3(-1.5f, -1.0f, -1.5f), 0.4f, 1.0f, purpleMat));

                // Small mirror sphere accent
                Material mirrorMat(Vec3(0.95f, 0.95f, 0.95f), 1.0f, 256.0f);
                mirrorMat.reflectivity = 0.9f;
                spheres.push_back(Sphere(Vec3(-0.7f, -0.75f, 1.5f), 0.25f, mirrorMat));
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

        // Test all boxes
        for (const auto& box : boxes) {
            HitRecord hit = box.intersect(ray);
            if (hit.hit && hit.t < closest.t) {
                closest = hit;
            }
        }

        // Test all cylinders
        for (const auto& cylinder : cylinders) {
            HitRecord hit = cylinder.intersect(ray);
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

        // Check all boxes
        for (const auto& box : boxes) {
            HitRecord hit = box.intersect(shadowRay);
            if (hit.hit && hit.t < lightDistance) {
                return true;
            }
        }

        // Check all cylinders
        for (const auto& cylinder : cylinders) {
            HitRecord hit = cylinder.intersect(shadowRay);
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

    // Calculate Fresnel reflectance using Schlick's approximation
    float fresnel(float cosTheta, float n1, float n2) const {
        float r0 = (n1 - n2) / (n1 + n2);
        r0 = r0 * r0;
        float x = 1.0f - cosTheta;
        return r0 + (1.0f - r0) * x * x * x * x * x;  // Schlick's approximation
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
        float transparency = hit.material.transparency;
        float reflectivity = hit.material.reflectivity;

        // Handle transparent materials with refraction
        if (transparency > 0.001f && depth < maxReflectionDepth) {
            Vec3 viewDir = ray.direction;
            Vec3 normal = hit.normal;
            
            // Determine if we're entering or exiting the material
            float n1, n2;
            bool entering = viewDir.dot(normal) < 0;
            
            if (entering) {
                n1 = 1.0f;  // Air
                n2 = hit.material.refractiveIndex;
            } else {
                n1 = hit.material.refractiveIndex;
                n2 = 1.0f;  // Air
                normal = normal * -1.0f;  // Flip normal when exiting
            }
            
            float eta = n1 / n2;
            float cosI = std::abs(viewDir.dot(normal));
            
            // Calculate Fresnel reflectance
            float fresnelReflect = fresnel(cosI, n1, n2);
            
            // Clamp fresnel to avoid over-reflection
            fresnelReflect = std::fmin(fresnelReflect, 0.95f);
            
            // Try to refract
            Vec3 refractDir = viewDir.refract(normal, eta);
            
            // Check for total internal reflection
            bool totalInternalReflection = (refractDir.lengthSquared() < 0.001f);
            
            Vec3 reflectDir = viewDir.reflect(normal);
            Vec3 reflectOrigin = hit.point + normal * 0.001f;
            Ray reflectRay(reflectOrigin, reflectDir);
            Vec3 reflectedColor = traceRay(reflectRay, depth + 1);
            
            if (totalInternalReflection) {
                // Total internal reflection - all light is reflected
                localColor = localColor * (1.0f - transparency) + reflectedColor * transparency;
            } else {
                // Mix refraction and reflection based on Fresnel
                Vec3 refractOrigin = hit.point - normal * 0.001f;
                Ray refractRay(refractOrigin, refractDir);
                Vec3 refractedColor = traceRay(refractRay, depth + 1);
                
                // Apply material tint to refracted color
                refractedColor = refractedColor * hit.material.color;
                
                // Blend reflection and refraction using Fresnel
                Vec3 transparentColor = reflectedColor * fresnelReflect + refractedColor * (1.0f - fresnelReflect);
                
                // Blend with local color based on transparency
                localColor = localColor * (1.0f - transparency) + transparentColor * transparency;
            }
        }
        // Handle reflective (but not transparent) materials
        else if (reflectivity > 0.001f && depth < maxReflectionDepth) {
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

    // Update first sphere's transparency/refraction properties
    void updateMainSphereTransparency(float transparency, float refractiveIndex) {
        if (!spheres.empty()) {
            spheres[0].material.transparency = std::fmax(0.0f, std::fmin(1.0f, transparency));
            spheres[0].material.refractiveIndex = std::fmax(1.0f, std::fmin(3.0f, refractiveIndex));
        }
    }

    // Get current transparency of main sphere
    float getMainSphereTransparency() const {
        return spheres.empty() ? 0.0f : spheres[0].material.transparency;
    }

    // Get current refractive index of main sphere
    float getMainSphereRefractiveIndex() const {
        return spheres.empty() ? 1.0f : spheres[0].material.refractiveIndex;
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

    int getBoxCount() const {
        return static_cast<int>(boxes.size());
    }

    int getCylinderCount() const {
        return static_cast<int>(cylinders.size());
    }

    int getTotalObjectCount() const {
        return static_cast<int>(spheres.size() + boxes.size() + cylinders.size());
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
