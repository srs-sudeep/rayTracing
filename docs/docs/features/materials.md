---
sidebar_position: 4
---

# Materials

Materials define how surfaces interact with light, controlling color, shininess, and reflectivity.

## Material Properties

```cpp
struct Material {
    Vec3 color;              // Base RGB color
    float ambient;           // Ambient light factor (0.1)
    float diffuse;           // Diffuse factor (0.7)
    float specularIntensity; // Specular brightness
    float shininess;         // Specular tightness
    float reflectivity;      // Mirror reflection
};
```

## Color

The base color of the surface, defined as RGB values in [0, 1]:

```cpp
Vec3 red(0.9f, 0.2f, 0.15f);
Vec3 blue(0.2f, 0.4f, 0.9f);
Vec3 gold(1.0f, 0.84f, 0.0f);
```

### Available Colors

| Color | RGB |
|-------|-----|
| Red | (0.9, 0.2, 0.15) |
| Blue | (0.2, 0.4, 0.9) |
| Green | (0.2, 0.85, 0.3) |
| Gold | (1.0, 0.84, 0.0) |
| Purple | (0.6, 0.2, 0.9) |
| White | (0.95, 0.95, 0.95) |

## Ambient

Simulates indirect lighting. Even in shadow, surfaces have some visibility:

```cpp
Vec3 ambient = material.color * material.ambient;
// Typically 0.1 (10%)
```

## Diffuse

Lambertian reflection for matte surfaces. Light scatters equally:

```cpp
float lambert = max(0, normal.dot(lightDir));
Vec3 diffuse = material.color * lambert * material.diffuse;
```

The `diffuse` factor (typically 0.7) controls contribution.

## Specular

### Intensity

How bright the specular highlight is:

- `0.0` - No highlight (pure matte)
- `0.5` - Moderate
- `1.0` - Maximum brightness

### Shininess

How tight/focused the highlight is:

| Shininess | Look |
|-----------|------|
| 8 | Very soft, spread |
| 32 | Plastic-like |
| 128 | Metallic |
| 256 | Near-mirror |

```cpp
// Blinn-Phong
Vec3 halfDir = normalize(lightDir + viewDir);
float spec = pow(max(0, normal.dot(halfDir)), shininess);
Vec3 specular = light.color * spec * material.specularIntensity;
```

## Reflectivity

How much the surface acts as a mirror:

```cpp
if (reflectivity > 0.001f) {
    Vec3 reflectDir = ray.direction.reflect(normal);
    Vec3 reflectedColor = traceRay(reflectRay, depth + 1);
    
    // Blend based on Fresnel
    finalColor = localColor * (1 - fresnel) + reflectedColor * fresnel;
}
```

## Material Presets

The UI provides quick presets:

### Matte
```cpp
{ specular: 0.1, shininess: 8, reflectivity: 0.0 }
```
Flat, non-shiny surface. Think chalk or unfinished wood.

### Plastic
```cpp
{ specular: 0.4, shininess: 32, reflectivity: 0.1 }
```
Slightly shiny with small specular highlight. Subtle reflection.

### Glossy
```cpp
{ specular: 0.6, shininess: 64, reflectivity: 0.3 }
```
Clear specular, noticeable reflection. Like polished plastic.

### Metal
```cpp
{ specular: 0.9, shininess: 128, reflectivity: 0.7 }
```
Bright, tight specular. Strong reflections. Metallic appearance.

### Mirror
```cpp
{ specular: 1.0, shininess: 256, reflectivity: 0.95 }
```
Near-perfect reflection. Chrome or polished silver.

## Visual Comparison

```
Matte         Plastic       Glossy        Metal         Mirror
  ○             ○             ⊙             ◉             ◯
no shine    soft shine    clear spot   bright spot   pure reflect
```

## Combining Properties

Materials are a combination of all properties:

```cpp
Material gold;
gold.color = Vec3(1.0f, 0.84f, 0.0f);  // Gold color
gold.specularIntensity = 0.9f;          // Bright highlight
gold.shininess = 128.0f;                // Tight highlight
gold.reflectivity = 0.8f;               // Very reflective
```

## Ground Plane Material

The ground has its own material:

```cpp
groundPlane.material.color = Vec3(0.15f, 0.15f, 0.17f);  // Dark gray
groundPlane.material.specularIntensity = 0.1f;           // Subtle
groundPlane.material.reflectivity = 0.15f;               // Slight reflection
```

Adjust ground reflectivity in the View tab.

