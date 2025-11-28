---
sidebar_position: 2
---

# Reflections

RayTracer Studio implements recursive reflections with a Fresnel-like effect for realistic mirror surfaces.

## How Reflections Work

When a ray hits a reflective surface:

1. Calculate the **reflection direction**
2. Spawn a new ray in that direction
3. **Recursively** trace the reflected ray
4. Blend the reflected color with the surface color

```cpp
Vec3 traceRay(Ray ray, int depth) {
    if (depth >= maxDepth) return backgroundColor;
    
    HitRecord hit = trace(ray);
    if (!hit.hit) return backgroundColor;
    
    Vec3 localColor = calculateLighting(ray, hit);
    
    if (hit.material.reflectivity > 0) {
        Vec3 reflectDir = ray.direction.reflect(hit.normal);
        Ray reflectRay(hit.point + offset, reflectDir);
        
        Vec3 reflectedColor = traceRay(reflectRay, depth + 1);
        
        localColor = blend(localColor, reflectedColor, reflectivity);
    }
    
    return localColor;
}
```

## Reflection Formula

The reflection of vector `V` around normal `N`:

```
R = V - 2(V · N)N
```

```cpp
Vec3 Vec3::reflect(const Vec3& normal) const {
    return *this - normal * (2.0f * this->dot(normal));
}
```

```
        N (normal)
        ↑
        │
    V ╲ │ ╱ R (reflection)
       ╲│╱
    ────●────  Surface
```

## Fresnel Effect

Real materials are more reflective at **grazing angles** (when you look at them from the side):

```cpp
float cosTheta = abs(normal.dot(viewDir * -1.0f));
float fresnelFactor = reflectivity + 
    (1.0f - reflectivity) * pow(1.0f - cosTheta, 3.0f);
```

This creates a more realistic effect where:
- **Center of sphere**: Less reflective
- **Edges of sphere**: More reflective

```
     Low reflection
          ↓
         ╭───╮
        ╱     ╲ ← High reflection
       │       │
       │   ●   │  (viewing angle)
       │       │
        ╲     ╱ ← High reflection
         ╰───╯
          ↑
     Low reflection
```

## Recursion Depth

Reflections are recursive - a mirror can reflect another mirror infinitely. We limit this with `maxReflectionDepth`:

```cpp
if (depth >= maxReflectionDepth) {
    return getBackgroundColor(ray);
}
```

| Depth | Effect |
|-------|--------|
| 1 | Single reflection |
| 3 | Mirror-in-mirror visible |
| 5 | Default, good quality |
| 8 | Maximum, for complex scenes |

## Material Reflectivity

The `reflectivity` property controls how mirror-like a surface is:

| Value | Appearance |
|-------|------------|
| 0.0 | No reflection (matte) |
| 0.3 | Subtle reflection (glossy plastic) |
| 0.7 | Strong reflection (polished metal) |
| 0.95 | Near-perfect mirror |

## Color Blending

The final color is a blend of local color and reflected color:

```cpp
finalColor = localColor * (1 - fresnel) + reflectedColor * fresnel;
```

For a perfect mirror (`reflectivity = 1.0`), the surface takes on entirely the reflected color.

## Ground Plane Reflections

The ground plane can also be reflective, creating a polished floor effect:

```cpp
groundPlane.material.reflectivity = 0.15f;  // Subtle floor reflection
```

This allows spheres to be partially visible in the ground.

## Self-Intersection Prevention

When spawning reflection rays, we offset the origin slightly to avoid hitting the same surface:

```cpp
Vec3 reflectOrigin = hit.point + hit.normal * 0.001f;
Ray reflectRay(reflectOrigin, reflectDir);
```

Without this offset, floating-point errors could cause the ray to immediately hit the surface it just reflected off of.

## Performance Impact

Each reflection level multiplies the number of rays:

| Bounces | Rays per Pixel (worst case) |
|---------|----------------------------|
| 1 | 2 |
| 3 | 8 |
| 5 | 32 |
| 8 | 256 |

Reduce `maxBounces` in the View tab for better performance.

## Scene Presets

Different presets showcase reflections:

- **Mirror Spheres**: Chrome and metallic balls with high reflectivity
- **Three Spheres**: Various reflectivity levels to compare

