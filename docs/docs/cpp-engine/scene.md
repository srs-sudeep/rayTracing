---
sidebar_position: 9
---

# Scene

The `Scene` class manages all objects, lights, and the camera. It contains the core ray tracing logic including support for soft shadows.

## Definition

```cpp title="cpp/include/Scene.h"
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
    std::vector<Sphere> spheres;     // All spheres in scene
    std::vector<Box> boxes;          // All boxes in scene
    std::vector<Cylinder> cylinders; // All cylinders in scene
    std::vector<Light> lights;       // All light sources
    Plane groundPlane;               // Ground plane
    Camera camera;                   // Active camera
    Vec3 backgroundColor;            // Sky color
    Vec3 horizonColor;               // Horizon color
    bool showGroundPlane;            // Plane visibility
    int maxReflectionDepth;          // Max recursive bounces
    ScenePreset currentPreset;       // Active preset
    
    // Soft shadow settings
    bool softShadowsEnabled;         // Enable area light shadows
    int shadowSamples;               // Shadow rays per light
};
```

## Core Methods

### Ray Tracing Entry Point

```cpp
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
    if (transparency > 0.001f) {
        Vec3 normal = hit.normal;
        bool entering = ray.direction.dot(normal) < 0;
        
        // Determine refractive indices
        float n1 = entering ? 1.0f : hit.material.refractiveIndex;
        float n2 = entering ? hit.material.refractiveIndex : 1.0f;
        if (!entering) normal = normal * -1.0f;
        
        float eta = n1 / n2;
        float cosI = std::abs(ray.direction.dot(normal));
        
        // Fresnel reflectance (Schlick approximation)
        float fresnelReflect = fresnel(cosI, n1, n2);
        
        // Attempt refraction
        Vec3 refractDir = ray.direction.refract(normal, eta);
        
        if (refractDir.lengthSquared() < 0.001f) {
            // Total internal reflection
            Vec3 reflectedColor = traceReflection(ray, hit, depth);
            localColor = localColor * (1 - transparency) + reflectedColor * transparency;
        } else {
            // Mix reflection and refraction via Fresnel
            Vec3 reflectedColor = traceReflection(ray, hit, depth);
            Vec3 refractedColor = traceRefraction(ray, hit, refractDir, depth);
            
            Vec3 transparentColor = reflectedColor * fresnelReflect + 
                                    refractedColor * (1 - fresnelReflect);
            localColor = localColor * (1 - transparency) + transparentColor * transparency;
        }
    }
    // Handle reflective (opaque) materials
    else if (reflectivity > 0.001f) {
        Vec3 reflectedColor = traceReflection(ray, hit, depth);
        float fresnel = calculateFresnel(ray.direction, hit.normal, reflectivity);
        localColor = localColor * (1 - fresnel) + reflectedColor * fresnel;
    }

    return localColor.clamp();
}
```

### Fresnel Calculation

```cpp
// Schlick's approximation for Fresnel reflectance
float fresnel(float cosTheta, float n1, float n2) const {
    float r0 = (n1 - n2) / (n1 + n2);
    r0 = r0 * r0;
    float x = 1.0f - cosTheta;
    return r0 + (1.0f - r0) * x * x * x * x * x;
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
```

### Shadow Detection

#### Hard Shadows (Point Light)

```cpp
bool isInShadowHard(const Vec3& point, const Vec3& lightPos) const {
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

#### Soft Shadows (Area Light)

```cpp
float calculateShadowFactor(const Vec3& point, const Light& light) const {
    if (!softShadowsEnabled || light.radius <= 0.0f) {
        // Hard shadows - binary test
        return isInShadowHard(point, light.position) ? 0.3f : 1.0f;
    }
    
    // Soft shadows - stratified sampling
    int litSamples = 0;
    int sqrtSamples = static_cast<int>(std::sqrt(shadowSamples));
    
    for (int i = 0; i < sqrtSamples; ++i) {
        for (int j = 0; j < sqrtSamples; ++j) {
            // Stratified random offset
            float u = (i + random()) / sqrtSamples;
            float v = (j + random()) / sqrtSamples;
            
            // Sample point on area light
            Vec3 samplePos = light.getSamplePointDisk(u, v, point);
            
            if (!isInShadowHard(point, samplePos)) {
                litSamples++;
            }
        }
    }
    
    float visibility = float(litSamples) / float(sqrtSamples * sqrtSamples);
    return 0.3f + visibility * 0.7f;  // 0.3 to 1.0
}
```

### Local Lighting (Blinn-Phong)

```cpp
Vec3 calculateLocalLighting(const Ray& ray, const HitRecord& hit) const {
    Vec3 color(0, 0, 0);
    Vec3 viewDir = (ray.origin - hit.point).normalize();

    for (const auto& light : lights) {
        Vec3 lightDir = (light.position - hit.point).normalize();
        
        // Shadow check (soft or hard)
        float shadowFactor = calculateShadowFactor(hit.point, light);
        
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
    boxes.clear();
    cylinders.clear();

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

        case ScenePreset::GLASS_SPHERES:
            // Glass, diamond, and water spheres
            // Demonstrates refraction and transparency
            break;

        case ScenePreset::PRIMITIVES:
            // Mixed shapes: spheres, boxes, cylinders
            // Showcases all primitive types
            break;
    }
}
```

| Preset | Objects | Description |
|--------|---------|-------------|
| Single | 1 sphere | Classic red sphere |
| Three | 3 spheres | Red, blue, green |
| Mirror | 5 spheres | Chrome and metallic |
| Rainbow | 7 spheres | ROYGBIV arc |
| Glass | 5 spheres | Transparent/refractive materials |
| Primitives | 3 spheres, 2 boxes, 2 cylinders | Mixed shapes showcase |

## Update Methods

These are called from JavaScript via Emscripten bindings:

```cpp
// Material updates
void updateMainSphere(float specular, float shininess, float reflectivity);
void updateMainSphereTransparency(float transparency, float refractiveIndex);
void updateSphereColor(float r, float g, float b);
void updateGroundReflectivity(float reflectivity);

// Object counts
int getSphereCount();
int getBoxCount();
int getCylinderCount();
int getTotalObjectCount();

// Light updates
void updateLight(float x, float y, float z);
int addLight(float x, float y, float z, float r, float g, float b, float intensity);
void removeLight(int index);
void setLightPosition(int index, float x, float y, float z);
void setLightColor(int index, float r, float g, float b);
void setLightIntensity(int index, float intensity);
void setLightRadius(int index, float radius);  // For soft shadows

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

// Soft shadow settings
void setSoftShadows(bool enabled);
void setShadowSamples(int samples);
```

## Rendering Flow

```
1. React state changes (user interaction)
        │
        ▼
2. WASM bindings called (updateLight, setSoftShadows, etc.)
        │
        ▼
3. Scene state updated (C++ objects modified)
        │
        ▼
4. render() called with resolution
        │
        ▼
5. For each pixel (with AA if enabled):
   ├── Generate camera ray(s)
   ├── traceRay(ray, 0)
   │   ├── Find intersection
   │   ├── Calculate lighting
   │   │   └── For each light:
   │   │       └── calculateShadowFactor()
   │   │           ├── Hard: 1 shadow ray
   │   │           └── Soft: N shadow rays (stratified)
   │   ├── Apply shadow factors
   │   └── Recursively trace reflections
   └── Average samples, write RGBA to buffer
        │
        ▼
6. Return pixel buffer to JavaScript
        │
        ▼
7. Display on Canvas
```

## Performance Considerations

### Shadow Ray Cost

| Shadow Type | Rays/Light/Hit | With 2 Lights |
|-------------|----------------|---------------|
| Hard | 1 | 2 |
| Soft (4) | 4 | 8 |
| Soft (9) | 9 | 18 |
| Soft (16) | 16 | 32 |
| Soft (25) | 25 | 50 |

### Optimization Tips

1. Use hard shadows during interaction, enable soft for final render
2. Lower resolution with higher shadow samples can look better than vice versa
3. Disable AA when using many shadow samples
4. Reduce max bounces when soft shadows are enabled
