---
sidebar_position: 5
---

# Light

The `Light` struct represents a point light source that illuminates the scene. RayTracer Studio supports **multiple colored lights** for complex lighting setups.

## Definition

```cpp title="cpp/include/Light.h"
struct Light {
    Vec3 position;   // Light position in world space
    Vec3 color;      // Light color (RGB, 0-1)
    float intensity; // Light brightness multiplier (0-2)

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

## Multiple Lights

The scene stores lights in a dynamic array, supporting up to 4 simultaneous lights:

```cpp
class Scene {
    std::vector<Light> lights;
    
    // Add a new light
    int addLight(float x, float y, float z, float r, float g, float b, float intensity);
    
    // Remove a light by index
    void removeLight(int index);
    
    // Update individual light properties
    void setLightPosition(int index, float x, float y, float z);
    void setLightColor(int index, float r, float g, float b);
    void setLightIntensity(int index, float intensity);
};
```

## Point Light Model

A point light emits light equally in all directions from a single point:

```
            ╲ │ ╱
             ╲│╱
          ────●────  Light Position
             ╱│╲
            ╱ │ ╲
           ↙  ↓  ↘   Light rays in all directions
```

## Lighting Calculation

For each surface point, contributions from **all lights** are accumulated:

```cpp
Vec3 calculateLocalLighting(const Ray& ray, const HitRecord& hit) const {
    Vec3 color(0, 0, 0);
    Vec3 viewDir = (ray.origin - hit.point).normalize();

    // Iterate over ALL lights
    for (const auto& light : lights) {
        Vec3 lightDir = (light.position - hit.point).normalize();
        
        // Shadow check for this specific light
        float shadowFactor = isInShadow(hit.point, light.position) ? 0.3f : 1.0f;
        
        // Diffuse (Lambertian)
        float diff = std::max(0.0f, hit.normal.dot(lightDir));
        Vec3 diffuse = hit.material.color * diff * hit.material.diffuse;
        
        // Specular (Blinn-Phong) - uses light color
        Vec3 halfDir = (lightDir + viewDir).normalize();
        float spec = std::pow(std::max(0.0f, hit.normal.dot(halfDir)), hit.material.shininess);
        Vec3 specular = light.color * spec * hit.material.specularIntensity;
        
        // Accumulate with intensity and shadow
        color = color + (diffuse + specular) * light.intensity * shadowFactor;
    }

    // Add ambient (not affected by individual lights)
    color = color + hit.material.color * hit.material.ambient;
    
    return color;
}
```

## Light Properties

### Position

Where the light is located in 3D space:

| Axis | Range | Effect |
|------|-------|--------|
| X | -5 to 5 | Left/right |
| Y | -5 to 5 | Up/down |
| Z | -5 to 5 | Forward/back |

### Color

Light color affects the appearance of illuminated surfaces:

| Preset | RGB | Effect |
|--------|-----|--------|
| White | (1.0, 1.0, 1.0) | Natural colors |
| Warm | (1.0, 0.9, 0.7) | Sunset/incandescent |
| Cool | (0.8, 0.9, 1.0) | Moonlight/LED |
| Red | (1.0, 0.3, 0.3) | Dramatic accent |
| Green | (0.3, 1.0, 0.4) | Spooky/sci-fi |
| Blue | (0.3, 0.5, 1.0) | Cold/underwater |

```cpp
// Surface color is multiplied by light color
Vec3 litColor = surfaceColor * lightColor;

// Examples:
// Red surface + Blue light = Dark (no red in blue light)
// Red surface + White light = Red
// White surface + Blue light = Blue
```

### Intensity

A multiplier for brightness (0.0 to 2.0):

| Value | Effect |
|-------|--------|
| 0.0 | Off |
| 0.5 | Dim (50%) |
| 1.0 | Normal |
| 1.5 | Bright (150%) |
| 2.0 | Very bright |

## Shadow Rays

Each light casts its own shadows:

```cpp
bool Scene::isInShadow(const Vec3& point, const Vec3& lightPos) const {
    Vec3 toLight = lightPos - point;
    float lightDistance = toLight.length();
    Vec3 lightDir = toLight.normalize();
    
    Ray shadowRay(point + lightDir * 0.001f, lightDir);
    
    for (const auto& sphere : spheres) {
        HitRecord hit = sphere.intersect(shadowRay);
        if (hit.hit && hit.t < lightDistance) {
            return true;  // Light is blocked
        }
    }
    return false;
}
```

With multiple lights, a point may be:
- **Fully lit**: Visible from all lights
- **Partially lit**: Visible from some lights (colored shadows!)
- **In shadow**: Blocked from all lights (only ambient)

## UI Controls

The Lights tab provides full control over multiple lights:

### Light Selector
- Tabs for each light (Light 1, Light 2, etc.)
- Add (+) and Remove (−) buttons
- Up to 4 lights maximum

### Per-Light Controls
- **Color presets**: White, Warm, Cool, Red, Green, Blue
- **Intensity slider**: 0% to 200%
- **Position sliders**: X, Y, Z coordinates

## API Reference

```typescript
// Add a new light (returns index)
addLight(x, y, z, r, g, b, intensity: number): number

// Remove light at index
removeLight(index: number): void

// Update light position
setLightPosition(index: number, x, y, z: number): void

// Update light color
setLightColor(index: number, r, g, b: number): void

// Update light intensity
setLightIntensity(index: number, intensity: number): void

// Get current light count
getLightCount(): number

// Get light properties
getLightX/Y/Z(index: number): number
getLightR/G/B(index: number): number
getLightIntensity(index: number): number
```

## Creative Uses

### Two-Point Lighting
Classic film lighting setup:
- **Key light**: Main light, bright, white/warm
- **Fill light**: Softer, dimmer, from opposite side

### Colored Accent Lighting
- Red and blue lights from opposite sides create dramatic contrast
- Green light from below for spooky effect

### Rim Lighting
- Light from behind the subject creates a glowing edge
- Enhance with high intensity
