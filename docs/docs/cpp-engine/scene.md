---
sidebar_position: 9
---

# Scene

The `Scene` class manages all objects, lights, and the camera. It contains the core ray tracing logic.

## Definition

```cpp title="cpp/include/Scene.h"
enum class ScenePreset {
    SINGLE_SPHERE = 0,
    THREE_SPHERES = 1,
    MIRROR_SPHERES = 2,
    RAINBOW = 3
};

class Scene {
public:
    std::vector<Sphere> spheres;    // All spheres in scene
    std::vector<Light> lights;       // All light sources
    Plane groundPlane;               // Ground plane
    Camera camera;                   // Active camera
    Vec3 backgroundColor;            // Sky color
    Vec3 horizonColor;               // Horizon color
    bool showGroundPlane;            // Plane visibility
    int maxReflectionDepth;          // Max recursive bounces
    ScenePreset currentPreset;       // Active preset
};
```

## Core Methods

### Ray Tracing Entry Point

```cpp
Vec3 traceRay(const Ray& ray, int depth) const {
    // Stop at max recursion depth
    if (depth >= maxReflectionDepth) {
        return getBackgroundColor(ray);
    }

    // Find closest intersection
    HitRecord hit = trace(ray);

    if (!hit.hit) {
        return getBackgroundColor(ray);
    }

    // Calculate local lighting
    Vec3 localColor = calculateLocalLighting(ray, hit);

    // Add reflections if material is reflective
    float reflectivity = hit.material.reflectivity;
    if (reflectivity > 0.001f) {
        Vec3 reflectDir = ray.direction.reflect(hit.normal);
        Vec3 reflectOrigin = hit.point + hit.normal * 0.001f;
        Ray reflectRay(reflectOrigin, reflectDir);
        
        Vec3 reflectedColor = traceRay(reflectRay, depth + 1);
        
        // Fresnel blending
        float cosTheta = std::abs(hit.normal.dot(ray.direction * -1.0f));
        float fresnel = reflectivity + (1.0f - reflectivity) * 
                        std::pow(1.0f - cosTheta, 3.0f);
        
        localColor = localColor * (1.0f - fresnel) + reflectedColor * fresnel;
    }

    return localColor.clamp();
}
```

### Finding Intersections

```cpp
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
```

### Shadow Detection

```cpp
bool isInShadow(const Vec3& point, const Vec3& lightPos) const {
    Vec3 toLight = lightPos - point;
    float lightDistance = toLight.length();
    Vec3 lightDir = toLight.normalize();
    
    Ray shadowRay(point + lightDir * 0.001f, lightDir);
    
    for (const auto& sphere : spheres) {
        HitRecord hit = sphere.intersect(shadowRay);
        if (hit.hit && hit.t < lightDistance) {
            return true;
        }
    }
    
    return false;
}
```

### Local Lighting (Blinn-Phong)

```cpp
Vec3 calculateLocalLighting(const Ray& ray, const HitRecord& hit) const {
    Vec3 color(0, 0, 0);
    Vec3 viewDir = (ray.origin - hit.point).normalize();

    for (const auto& light : lights) {
        Vec3 lightDir = (light.position - hit.point).normalize();
        
        // Shadow check
        float shadowFactor = isInShadow(hit.point, light.position) ? 0.3f : 1.0f;
        
        // Diffuse
        float diff = std::max(0.0f, hit.normal.dot(lightDir));
        Vec3 diffuse = hit.material.color * diff * hit.material.diffuse;
        
        // Specular
        Vec3 halfDir = (lightDir + viewDir).normalize();
        float spec = std::pow(std::max(0.0f, hit.normal.dot(halfDir)), 
                              hit.material.shininess);
        Vec3 specular = light.color * spec * hit.material.specularIntensity;
        
        color = color + (diffuse + specular) * light.intensity * shadowFactor;
    }

    // Ambient
    color = color + hit.material.color * hit.material.ambient;

    return color;
}
```

### Background Gradient

```cpp
Vec3 getBackgroundColor(const Ray& ray) const {
    // Vertical gradient from horizon to sky
    float t = 0.5f * (ray.direction.y + 1.0f);
    t = std::fmax(0.0f, std::fmin(1.0f, t));
    
    return horizonColor * (1.0f - t) + backgroundColor * t;
}
```

## Scene Presets

```cpp
void loadPreset(ScenePreset preset) {
    spheres.clear();

    switch (preset) {
        case ScenePreset::SINGLE_SPHERE:
            // Single red sphere at origin
            break;
            
        case ScenePreset::THREE_SPHERES:
            // Red, blue, green spheres
            break;
            
        case ScenePreset::MIRROR_SPHERES:
            // Chrome and metallic spheres
            break;
            
        case ScenePreset::RAINBOW:
            // 7 colored spheres in an arc
            break;
    }
}
```

| Preset | Spheres | Description |
|--------|---------|-------------|
| Single | 1 | Classic red sphere |
| Three | 3 | Red, blue, green |
| Mirror | 5 | Chrome and metallic |
| Rainbow | 7 | ROYGBIV arc |

## Update Methods

These are called from JavaScript via Emscripten bindings:

```cpp
// Material updates
void updateMainSphere(float specular, float shininess, float reflectivity);
void updateSphereColor(float r, float g, float b);
void updateGroundReflectivity(float reflectivity);

// Light updates
void updateLight(float x, float y, float z);

// Camera updates
void updateCamera(float x, float y, float z);
void orbitCamera(float deltaX, float deltaY);
void zoomCamera(float delta);
void setCameraFov(float fov);
void setCameraTarget(float x, float y, float z);

// View settings
void setShowGroundPlane(bool show);
void setShowGrid(bool show);
void setGridScale(float scale);
void setMaxReflectionDepth(int depth);
```

## Rendering Flow

```
1. React state changes (user interaction)
        │
        ▼
2. WASM bindings called (updateLight, updateMaterial, etc.)
        │
        ▼
3. Scene state updated (C++ objects modified)
        │
        ▼
4. render() called with resolution
        │
        ▼
5. For each pixel:
   ├── Generate camera ray
   ├── traceRay(ray, 0)
   │   ├── Find intersection
   │   ├── Calculate lighting
   │   ├── Check shadows
   │   └── Recursively trace reflections
   └── Write RGBA to buffer
        │
        ▼
6. Return pixel buffer to JavaScript
        │
        ▼
7. Display on Canvas
```

