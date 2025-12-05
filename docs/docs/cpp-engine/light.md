---
sidebar_position: 5
---

# Light

The `Light` struct represents a light source that illuminates the scene. RayTracer Studio supports **multiple colored lights** (up to 4) and **area lights** for soft shadows.

## Definition

```cpp title="cpp/include/Light.h"
struct Light {
    Vec3 position;   // Light position in world space
    Vec3 color;      // Light color (RGB, 0-1)
    float intensity; // Light brightness multiplier (0-2)
    float radius;    // Area light radius for soft shadows (0 = point light)

    Light() 
        : position(Vec3(2.0f, 2.0f, -1.0f))
        , color(Vec3(1.0f, 1.0f, 1.0f))
        , intensity(1.0f)
        , radius(0.5f) {}

    Light(const Vec3& pos, const Vec3& col, float intens, float rad = 0.5f)
        : position(pos)
        , color(col)
        , intensity(intens)
        , radius(rad) {}

    // Sample point on area light for soft shadows
    Vec3 getSamplePointDisk(float u, float v, const Vec3& target) const;
};
```

## Point Light vs Area Light

### Point Light (radius = 0)

Emits light from a single infinitesimal point, creating **hard shadows**:

```
            ╲ │ ╱
             ╲│╱
          ────●────  Light Position
             ╱│╲
            ╱ │ ╲
           ↙  ↓  ↘   Light rays from single point

Shadow: ████████████  (sharp edge)
```

### Area Light (radius > 0)

Emits light from a spherical area, creating **soft shadows**:

```
           ╲   │   ╱
            ╲  │  ╱
         ╱───(●)───╲  Light with radius
            ╱  │  ╲
           ╱   │   ╲
          ↙    ↓    ↘  Light rays from area

Shadow: ░░▓▓████▓▓░░  (soft penumbra)
```

## Area Light Sampling

For soft shadows, we sample multiple points on the light's surface:

```cpp
Vec3 getSamplePointDisk(float u, float v, const Vec3& target) const {
    if (radius <= 0.0f) return position;
    
    // Direction from light to target
    Vec3 toTarget = (target - position).normalize();
    
    // Create orthonormal basis on disk facing target
    Vec3 up = (std::abs(toTarget.y) < 0.9f) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
    Vec3 right = toTarget.cross(up).normalize();
    up = right.cross(toTarget).normalize();
    
    // Random point in unit disk (concentric mapping for uniform distribution)
    float r = std::sqrt(u);
    float theta = 2.0f * PI * v;
    float dx = r * std::cos(theta);
    float dy = r * std::sin(theta);
    
    return position + right * (dx * radius) + up * (dy * radius);
}
```

This creates a disk-shaped sampling area that always faces the point being shaded:

```
         ┌─────┐
         │ ○ ○ │
Target ◄─│○ ● ○│── Disk samples face target
         │ ○ ○ │
         └─────┘
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
    void setLightRadius(int index, float radius);  // For soft shadows
};
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
        
        // Shadow check (soft or hard depending on settings)
        float shadowFactor = calculateShadowFactor(hit.point, light);
        
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

### Radius (for Soft Shadows)

Controls the size of the area light:

| Value | Effect |
|-------|--------|
| 0.0 | Point light (hard shadows) |
| 0.1-0.3 | Small area (subtle softness) |
| 0.5 | Medium area (default) |
| 1.0-1.5 | Large area (very soft shadows) |

Larger radius = softer shadow edges.

## Shadow Rays

Each light casts its own shadows. With soft shadows enabled, multiple rays are cast:

```cpp
float calculateShadowFactor(const Vec3& point, const Light& light) const {
    if (!softShadowsEnabled || light.radius <= 0.0f) {
        // Hard shadows
        return isInShadowHard(point, light.position) ? 0.3f : 1.0f;
    }
    
    // Soft shadows - sample multiple points on area light
    int litSamples = 0;
    for (int i = 0; i < shadowSamples; i++) {
        Vec3 samplePos = light.getSamplePointDisk(random(), random(), point);
        if (!isInShadowHard(point, samplePos)) {
            litSamples++;
        }
    }
    
    float visibility = float(litSamples) / float(shadowSamples);
    return 0.3f + visibility * 0.7f;  // 0.3 to 1.0
}
```

With multiple lights, a point may be:
- **Fully lit**: Visible from all lights
- **Partially lit**: Visible from some lights (colored shadows!)
- **In shadow**: Blocked from all lights (only ambient)

## UI Controls

### Light Tab
- **Light Selector**: Tabs for each light (Light 1, Light 2, etc.)
- **Add (+) / Remove (−)**: Up to 4 lights
- **Color presets**: White, Warm, Cool, Red, Green, Blue
- **Intensity slider**: 0% to 200%
- **Position sliders**: X, Y, Z coordinates

### View Tab (Soft Shadows)
- **Soft Shadows toggle**: Enable/disable
- **Light Size slider**: Controls radius for all lights
- **Shadow Samples**: 4, 9, 16, or 25 rays per light

## API Reference

```typescript
// Add a new light (returns index)
addLight(x, y, z, r, g, b, intensity: number): number

// Remove light at index
removeLight(index: number): void

// Position
setLightPosition(index: number, x, y, z: number): void
getLightX/Y/Z(index: number): number

// Color
setLightColor(index: number, r, g, b: number): void
getLightR/G/B(index: number): number

// Intensity
setLightIntensity(index: number, intensity: number): void
getLightIntensity(index: number): number

// Radius (for soft shadows)
setLightRadius(index: number, radius: number): void
getLightRadius(index: number): number

// Utility
getLightCount(): number
resetLights(): void
```

## Creative Uses

### Two-Point Lighting
Classic film lighting setup:
- **Key light**: Main light, bright, white/warm
- **Fill light**: Softer, dimmer, from opposite side

### Colored Accent Lighting
- Red and blue lights from opposite sides create dramatic contrast
- Green light from below for spooky effect

### Soft vs Hard Shadows
- Use hard shadows (radius = 0) for harsh sunlight
- Use soft shadows (radius > 0) for indoor/overcast lighting
