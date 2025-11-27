#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"
#include "Camera.h"
#include <vector>
#include <algorithm>

class Scene {
public:
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    Plane groundPlane;
    Camera camera;
    Vec3 backgroundColor;
    Vec3 horizonColor;
    bool showGroundPlane;

    Scene() 
        : backgroundColor(Vec3(0.05f, 0.05f, 0.08f))
        , horizonColor(Vec3(0.12f, 0.12f, 0.15f))
        , showGroundPlane(true) 
    {
        // Default scene setup - red sphere
        Material redMaterial(Vec3(0.9f, 0.2f, 0.15f), 0.5f, 32.0f);
        spheres.push_back(Sphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f, redMaterial));
        
        // Default light
        lights.push_back(Light(Vec3(2.0f, 3.0f, -2.0f), Vec3(1.0f, 1.0f, 1.0f), 1.0f));
    }

    HitRecord trace(const Ray& ray) const {
        HitRecord closest;
        closest.t = 1e30f;
        closest.hit = false;

        // Test spheres
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

    // Check if a point is in shadow
    bool isInShadow(const Vec3& point, const Vec3& lightPos) const {
        Vec3 toLight = lightPos - point;
        float lightDistance = toLight.length();
        Vec3 lightDir = toLight.normalize();
        
        Ray shadowRay(point + lightDir * 0.001f, lightDir);
        
        // Check spheres
        for (const auto& sphere : spheres) {
            HitRecord hit = sphere.intersect(shadowRay);
            if (hit.hit && hit.t < lightDistance) {
                return true;
            }
        }
        
        return false;
    }

    Vec3 getBackgroundColor(const Ray& ray) const {
        // Gradient sky based on ray direction
        float t = 0.5f * (ray.direction.y + 1.0f);
        t = std::fmax(0.0f, std::fmin(1.0f, t));
        
        // Interpolate between horizon and sky
        return horizonColor * (1.0f - t) + backgroundColor * t;
    }

    Vec3 shade(const Ray& ray, const HitRecord& hit) const {
        if (!hit.hit) {
            return getBackgroundColor(ray);
        }

        Vec3 color(0, 0, 0);
        Vec3 viewDir = (ray.origin - hit.point).normalize();

        for (const auto& light : lights) {
            Vec3 lightDir = (light.position - hit.point).normalize();
            
            // Shadow check
            float shadowFactor = 1.0f;
            if (isInShadow(hit.point, light.position)) {
                shadowFactor = 0.3f; // Soft shadow - not completely dark
            }
            
            // Diffuse
            float diff = std::max(0.0f, hit.normal.dot(lightDir));
            Vec3 diffuse = hit.material.color * diff * hit.material.diffuse;
            
            // Specular (Blinn-Phong)
            Vec3 halfDir = (lightDir + viewDir).normalize();
            float spec = std::pow(std::max(0.0f, hit.normal.dot(halfDir)), hit.material.shininess);
            Vec3 specular = light.color * spec * hit.material.specularIntensity;
            
            color = color + (diffuse + specular) * light.intensity * shadowFactor;
        }

        // Ambient
        Vec3 ambient = hit.material.color * hit.material.ambient;
        color = color + ambient;

        return color.clamp();
    }

    // Update functions for JS bindings
    void updateMainSphere(float specular, float shininess) {
        if (!spheres.empty()) {
            spheres[0].material.specularIntensity = specular;
            spheres[0].material.shininess = shininess;
        }
    }

    void updateLight(float x, float y, float z) {
        if (!lights.empty()) {
            lights[0].position = Vec3(x, y, z);
        }
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
};
