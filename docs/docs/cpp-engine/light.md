---
sidebar_position: 5
---

# Light

The `Light` struct represents a point light source that illuminates the scene.

## Definition

```cpp title="cpp/include/Light.h"
struct Light {
    Vec3 position;   // Light position in world space
    Vec3 color;      // Light color (RGB)
    float intensity; // Light brightness multiplier

    Light() 
        : position(Vec3(2.0f, 2.0f, -1.0f))
        , color(Vec3(1.0f, 1.0f, 1.0f))  // White light
        , intensity(1.0f) {}

    Light(const Vec3& pos, const Vec3& col, float intens)
        : position(pos)
        , color(col)
        , intensity(intens) {}
};
```

## Point Light Model

A point light is the simplest light type - it emits light equally in all directions from a single point:

```
            ╲ │ ╱
             ╲│╱
          ────●────  Light Position
             ╱│╲
            ╱ │ ╲
           ↙  ↓  ↘   Light rays in all directions
```

## Lighting Calculation

For each surface point, we calculate how the light affects it:

```cpp
Vec3 calculateLighting(const Vec3& hitPoint, const Vec3& normal, 
                       const Vec3& viewDir, const Material& mat) {
    Vec3 result(0, 0, 0);
    
    for (const auto& light : lights) {
        // Direction from hit point to light
        Vec3 lightDir = (light.position - hitPoint).normalize();
        
        // Diffuse contribution
        float diff = std::max(0.0f, normal.dot(lightDir));
        Vec3 diffuse = mat.color * diff * mat.diffuse;
        
        // Specular contribution (Blinn-Phong)
        Vec3 halfDir = (lightDir + viewDir).normalize();
        float spec = std::pow(std::max(0.0f, normal.dot(halfDir)), mat.shininess);
        Vec3 specular = light.color * spec * mat.specularIntensity;
        
        // Combine with light intensity
        result = result + (diffuse + specular) * light.intensity;
    }
    
    return result;
}
```

## Properties

### Position

Where the light is located in 3D space. Can be controlled via the UI:

```cpp
void updateLight(float x, float y, float z) {
    globalScene.lights[0].position = Vec3(x, y, z);
}
```

Moving the light changes:
- Which parts of objects are illuminated
- Shadow direction and length
- Specular highlight position

### Color

Light color affects the final appearance of illuminated surfaces:

```cpp
// White light - natural colors
Vec3 white(1.0f, 1.0f, 1.0f);

// Warm light - sunset effect
Vec3 warm(1.0f, 0.9f, 0.7f);

// Cool light - moonlight effect
Vec3 cool(0.8f, 0.9f, 1.0f);

// Colored light - artistic effect
Vec3 red(1.0f, 0.3f, 0.3f);
```

The surface color is multiplied by the light color:
```cpp
Vec3 litColor = surfaceColor * lightColor;
// Red surface + Blue light = Dark (no red in blue light)
// Red surface + White light = Red
```

### Intensity

A multiplier for the light's overall brightness:

```cpp
float intensity = 1.0f;   // Normal brightness
float intensity = 0.5f;   // Half brightness (dimmer)
float intensity = 2.0f;   // Double brightness (brighter)
```

## Shadow Rays

To determine if a point is in shadow, we trace a ray toward the light:

```cpp
bool Scene::isInShadow(const Vec3& point, const Vec3& lightPos) const {
    Vec3 toLight = lightPos - point;
    float lightDistance = toLight.length();
    Vec3 lightDir = toLight.normalize();
    
    // Start slightly off the surface to avoid self-shadowing
    Ray shadowRay(point + lightDir * 0.001f, lightDir);
    
    // Check if any object blocks the path to the light
    for (const auto& sphere : spheres) {
        HitRecord hit = sphere.intersect(shadowRay);
        if (hit.hit && hit.t < lightDistance) {
            return true;  // Object between point and light
        }
    }
    
    return false;  // Clear path to light
}
```

## Shadow Implementation

In the scene, shadows are applied as a multiplier:

```cpp
float shadowFactor = 1.0f;
if (isInShadow(hit.point, light.position)) {
    shadowFactor = 0.3f;  // Soft shadow (30% light)
}

Vec3 litColor = (diffuse + specular) * shadowFactor;
```

Using `0.3` instead of `0.0` creates softer, more realistic shadows that aren't completely black.

## UI Control

The light position is controlled by 3 sliders in the Light tab:

| Slider | Range | Description |
|--------|-------|-------------|
| X | -5 to 5 | Left/right position |
| Y | -5 to 5 | Up/down position |
| Z | -5 to 5 | Forward/back position |

```jsx
// React component binds to WASM
wasmModule.updateLight(light.x, light.y, light.z);
```

