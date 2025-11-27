#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Light.h"
#include "Camera.h"
#include <vector>
#include <algorithm>

class Scene {
public:
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    Camera camera;
    Vec3 backgroundColor;
    bool showGrid;

    Scene() : backgroundColor(Vec3(0.08f, 0.08f, 0.1f)), showGrid(true) {
        // Default scene setup
        Material redMaterial(Vec3(0.9f, 0.2f, 0.15f), 0.5f, 32.0f);
        spheres.push_back(Sphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f, redMaterial));
        
        lights.push_back(Light(Vec3(2.0f, 2.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f), 1.0f));
    }

    HitRecord trace(const Ray& ray) const {
        HitRecord closest;
        closest.t = 1e30f;
        closest.hit = false;

        for (const auto& sphere : spheres) {
            HitRecord hit = sphere.intersect(ray);
            if (hit.hit && hit.t < closest.t) {
                closest = hit;
            }
        }

        return closest;
    }

    Vec3 shade(const Ray& ray, const HitRecord& hit) const {
        if (!hit.hit) {
            return backgroundColor;
        }

        Vec3 color(0, 0, 0);
        Vec3 viewDir = (ray.origin - hit.point).normalize();

        for (const auto& light : lights) {
            Vec3 lightDir = (light.position - hit.point).normalize();
            
            // Diffuse
            float diff = std::max(0.0f, hit.normal.dot(lightDir));
            Vec3 diffuse = hit.material.color * diff * hit.material.diffuse;
            
            // Specular (Blinn-Phong)
            Vec3 halfDir = (lightDir + viewDir).normalize();
            float spec = std::pow(std::max(0.0f, hit.normal.dot(halfDir)), hit.material.shininess);
            Vec3 specular = light.color * spec * hit.material.specularIntensity;
            
            color = color + (diffuse + specular) * light.intensity;
        }

        // Ambient
        Vec3 ambient = hit.material.color * hit.material.ambient;
        color = color + ambient;

        return color.clamp();
    }

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
};

