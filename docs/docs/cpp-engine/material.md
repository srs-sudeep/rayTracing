---
sidebar_position: 4
---

# Material

The `Material` struct defines the visual appearance of surfaces, controlling how they interact with light.

## Definition

```cpp title="cpp/include/Material.h"
struct Material {
    Vec3 color;              // Base color (RGB normalized)
    float ambient;           // Ambient light factor
    float diffuse;           // Diffuse reflection factor
    float specularIntensity; // Specular highlight intensity
    float shininess;         // Specular exponent (tightness)
    float reflectivity;      // Mirror reflection factor
    float transparency;      // Transparency (0 = opaque, 1 = fully transparent)
    float refractiveIndex;   // Index of refraction (1.0 = air, 1.5 = glass)
    
    Material() 
        : color(Vec3(0.9f, 0.2f, 0.15f))  // Default red
        , ambient(0.1f)
        , diffuse(0.7f)
        , specularIntensity(0.5f)
        , shininess(32.0f)
        , reflectivity(0.0f)
        , transparency(0.0f)
        , refractiveIndex(1.0f) {}

    Material(const Vec3& col, float spec, float shine)
        : color(col)
        , ambient(0.1f)
        , diffuse(0.7f)
        , specularIntensity(spec)
        , shininess(shine)
        , reflectivity(0.0f)
        , transparency(0.0f)
        , refractiveIndex(1.0f) {}
};
```

## Properties Explained

### Color

Base surface color as RGB values in the range [0, 1]:

```cpp
Vec3 red(0.9f, 0.2f, 0.15f);    // Warm red
Vec3 gold(1.0f, 0.84f, 0.0f);   // Gold
Vec3 chrome(0.95f, 0.95f, 0.95f); // Chrome/silver
```

### Ambient

Simulates indirect lighting. Even in shadow, surfaces aren't completely black:

```cpp
float ambient = 0.1f;  // 10% of color visible in shadow
Vec3 ambientColor = material.color * ambient;
```

### Diffuse

Lambertian reflection - light scatters equally in all directions. This is what makes matte surfaces look matte:

```cpp
float diffuse = 0.7f;  // 70% diffuse contribution
float lambert = std::max(0.0f, normal.dot(lightDir));
Vec3 diffuseColor = material.color * lambert * diffuse;
```

### Specular Intensity

Controls the brightness of the specular highlight (shiny spot):

- `0.0` - No highlight (pure matte)
- `0.5` - Moderate shine
- `1.0` - Maximum shine

### Shininess

The specular exponent controls how tight/focused the highlight is:

| Value | Appearance |
|-------|------------|
| 8 | Very soft, spread out |
| 32 | Moderate, plastic-like |
| 128 | Tight, metallic |
| 256 | Very tight, mirror-like |

```cpp
// Blinn-Phong specular
Vec3 halfDir = (lightDir + viewDir).normalize();
float specAngle = std::max(0.0f, normal.dot(halfDir));
float specular = std::pow(specAngle, shininess) * specularIntensity;
```

### Reflectivity

How much the surface acts as a mirror:

- `0.0` - No reflection (opaque)
- `0.3` - Subtle reflection
- `0.7` - Highly reflective
- `0.95` - Nearly perfect mirror

### Transparency

How see-through the material is:

- `0.0` - Fully opaque
- `0.5` - Semi-transparent
- `0.9` - Mostly transparent (glass-like)
- `1.0` - Fully transparent

When transparency > 0, refraction is calculated using Snell's law.

### Refractive Index (IOR)

Controls how much light bends when entering the material:

| Material | IOR | Effect |
|----------|-----|--------|
| Air | 1.0 | No bending |
| Water | 1.33 | Slight bending |
| Glass | 1.5 | Noticeable bending |
| Crystal | 2.0 | Strong bending |
| Diamond | 2.4 | Maximum sparkle |

Higher IOR = more light bending = more distortion of objects behind.

## Factory Methods

Pre-configured materials for common transparent materials:

```cpp
// Clear glass material
static Material glass(const Vec3& tint = Vec3(1.0f, 1.0f, 1.0f)) {
    Material m;
    m.color = tint;
    m.transparency = 0.95f;
    m.refractiveIndex = 1.5f;
    m.specularIntensity = 1.0f;
    m.shininess = 256.0f;
    return m;
}

// Diamond material (high dispersion)
static Material diamond() {
    Material m;
    m.transparency = 0.95f;
    m.refractiveIndex = 2.4f;
    m.specularIntensity = 1.0f;
    m.shininess = 512.0f;
    return m;
}

// Water-like material
static Material water() {
    Material m;
    m.color = Vec3(0.8f, 0.9f, 1.0f);  // Slight blue tint
    m.transparency = 0.9f;
    m.refractiveIndex = 1.33f;
    return m;
}
```

## Material Presets

The UI provides these presets:

### Opaque Materials

| Preset | Specular | Shininess | Reflectivity | Transparency |
|--------|----------|-----------|--------------|--------------|
| **Matte** | 0.1 | 8 | 0.0 | 0.0 |
| **Plastic** | 0.4 | 32 | 0.1 | 0.0 |
| **Glossy** | 0.6 | 64 | 0.3 | 0.0 |
| **Metal** | 0.9 | 128 | 0.7 | 0.0 |
| **Chrome** | 1.0 | 256 | 0.95 | 0.0 |

### Transparent Materials

| Preset | Specular | Shininess | Transparency | IOR |
|--------|----------|-----------|--------------|-----|
| **Glass** | 1.0 | 256 | 0.95 | 1.5 |
| **Diamond** | 1.0 | 512 | 0.95 | 2.4 |
| **Water** | 0.8 | 128 | 0.9 | 1.33 |

## Shading Calculation

The complete shading equation combines all components:

```cpp
Vec3 shade(const Ray& ray, const HitRecord& hit) {
    Vec3 viewDir = (ray.origin - hit.point).normalize();
    Vec3 color(0, 0, 0);

    for (const auto& light : lights) {
        Vec3 lightDir = (light.position - hit.point).normalize();
        
        // Shadow factor
        float shadow = isInShadow(hit.point, light.position) ? 0.3f : 1.0f;
        
        // Diffuse (Lambertian)
        float diff = std::max(0.0f, hit.normal.dot(lightDir));
        Vec3 diffuse = hit.material.color * diff * hit.material.diffuse;
        
        // Specular (Blinn-Phong)
        Vec3 halfDir = (lightDir + viewDir).normalize();
        float spec = std::pow(
            std::max(0.0f, hit.normal.dot(halfDir)), 
            hit.material.shininess
        );
        Vec3 specular = light.color * spec * hit.material.specularIntensity;
        
        color = color + (diffuse + specular) * shadow;
    }

    // Ambient
    color = color + hit.material.color * hit.material.ambient;

    // Reflections (if material is reflective)
    if (hit.material.reflectivity > 0.001f) {
        Vec3 reflectDir = ray.direction.reflect(hit.normal);
        Ray reflectRay(hit.point + hit.normal * 0.001f, reflectDir);
        Vec3 reflected = traceRay(reflectRay, depth + 1);
        
        // Blend with Fresnel-like effect
        float fresnel = calculateFresnel(viewDir, hit.normal, reflectivity);
        color = color * (1.0f - fresnel) + reflected * fresnel;
    }

    return color.clamp();
}
```

## Fresnel Effect

Real materials are more reflective at grazing angles. We approximate this:

```cpp
float cosTheta = std::abs(normal.dot(viewDir * -1.0f));
float fresnelFactor = reflectivity + (1.0f - reflectivity) * std::pow(1.0f - cosTheta, 3.0f);
```

This makes edges of spheres more reflective than their centers.

