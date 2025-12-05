---
sidebar_position: 6
---

# Refractions

Refraction simulates how light bends when passing through transparent materials like glass, water, or diamonds.

## How It Works

When light enters a material with a different density, it changes direction according to **Snell's Law**:

```
n₁ sin(θ₁) = n₂ sin(θ₂)
```

Where:
- `n₁` = refractive index of first material (e.g., air = 1.0)
- `n₂` = refractive index of second material (e.g., glass = 1.5)
- `θ₁` = angle of incidence
- `θ₂` = angle of refraction

## Material Properties

### Transparency

Controls how see-through the material is:

| Value | Effect |
|-------|--------|
| 0.0 | Fully opaque |
| 0.5 | Semi-transparent |
| 0.9 | Glass-like |
| 1.0 | Fully transparent |

### Index of Refraction (IOR)

Controls how much light bends:

| Material | IOR | Visual Effect |
|----------|-----|---------------|
| Air | 1.0 | No bending |
| Water | 1.33 | Slight distortion |
| Glass | 1.5 | Noticeable bending |
| Crystal | 2.0 | Strong distortion |
| Diamond | 2.4 | Maximum sparkle |

Higher IOR = more bending = more distortion of objects behind.

## Transparent Material Presets

The UI provides quick presets for common transparent materials:

### Glass
```cpp
transparency: 0.95
refractiveIndex: 1.5
specular: 1.0
shininess: 256
```
Clear, window-like appearance with realistic light bending.

### Diamond
```cpp
transparency: 0.95
refractiveIndex: 2.4
specular: 1.0
shininess: 512
```
High dispersion creates sparkle and strong distortion.

### Water
```cpp
transparency: 0.9
refractiveIndex: 1.33
color: (0.8, 0.9, 1.0)  // Slight blue tint
```
Subtle bending with characteristic blue tint.

## Fresnel Effect

Real transparent materials reflect more light at grazing angles. We use **Schlick's approximation**:

```cpp
float fresnel(float cosTheta, float n1, float n2) {
    float r0 = pow((n1 - n2) / (n1 + n2), 2);
    return r0 + (1 - r0) * pow(1 - cosTheta, 5);
}
```

**Effect:**
- Looking straight through glass → mostly refraction
- Looking at glass edge → mostly reflection

This creates realistic-looking transparent objects with reflective edges.

## Total Internal Reflection

When light travels from a dense material to a less dense one (e.g., inside glass looking out), at steep angles it cannot exit—it reflects instead.

**Critical angle formula:**
```
θc = arcsin(n₂/n₁)
```

For glass-to-air: θc ≈ 42°

The ray tracer detects this and uses reflection when refraction is impossible.

## Ray Tracing Implementation

```cpp
// In traceRay():
if (material.transparency > 0.001f) {
    // Determine if entering or exiting material
    bool entering = ray.direction.dot(normal) < 0;
    
    // Calculate eta (n1/n2 ratio)
    float eta = entering ? (1.0f / material.refractiveIndex) 
                         : material.refractiveIndex;
    
    // Attempt refraction
    Vec3 refractDir = ray.direction.refract(normal, eta);
    
    if (totalInternalReflection) {
        // Use reflection only
        color = traceReflection();
    } else {
        // Blend reflection and refraction via Fresnel
        Vec3 reflected = traceReflection();
        Vec3 refracted = traceRefraction();
        color = reflected * fresnel + refracted * (1 - fresnel);
    }
}
```

## Glass Spheres Scene

The "Glass" scene preset demonstrates refraction:

- **Large glass sphere** - Shows distortion of background
- **Diamond sphere** - High IOR creates more bending
- **Blue-tinted glass** - Colored transparency
- **Water sphere** - Subtle refraction
- **Red sphere behind** - Shows how objects distort through glass

Select "Glass" (💎) in the scene toolbar to see these effects.

## UI Controls

### Material Tab

1. **Transparency slider** - 0% to 100%
2. **IOR slider** - 1.0 to 3.0
3. **IOR presets** - Quick buttons for Air, Water, Glass, Crystal, Diamond

The IOR slider is disabled when transparency is 0 (opaque materials don't refract).

## Performance Impact

Refraction adds significant cost:

| Feature | Ray Cost |
|---------|----------|
| Opaque material | 1 ray |
| Reflection only | 2 rays |
| Refraction | 2-3 rays |
| Glass in glass | Exponential |

**Tips:**
- Lower max bounces when using transparency
- Fewer transparent objects = faster rendering
- Simple scenes with glass look best

## Visual Examples

### IOR Comparison

```
IOR 1.0 (Air)     IOR 1.5 (Glass)   IOR 2.4 (Diamond)
     ○                 ◐                  ◑
  no bend         some bend          strong bend
```

### Fresnel at Edges

```
Looking straight:      Looking at edge:
    ┌─────┐               ╱─────╲
    │ see │              ╱reflect╲
    │thru │             ╱         ╲
    └─────┘
```

## Common Issues

### Black areas inside glass
Increase max bounces to allow rays to exit the material.

### No visible refraction
Make sure transparency > 0 and IOR ≠ 1.0.

### Too much reflection, not enough see-through
This is realistic! Real glass reflects at edges. Lower reflectivity for more see-through appearance.

