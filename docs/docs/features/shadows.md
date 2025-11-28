---
sidebar_position: 3
---

# Shadows

Shadows add depth and realism to the scene by simulating light occlusion.

## Shadow Rays

For each point we shade, we check if it's visible from the light source by casting a **shadow ray**:

```cpp
bool isInShadow(const Vec3& point, const Vec3& lightPos) const {
    Vec3 toLight = lightPos - point;
    float lightDistance = toLight.length();
    Vec3 lightDir = toLight.normalize();
    
    // Offset to avoid self-shadowing
    Ray shadowRay(point + lightDir * 0.001f, lightDir);
    
    // Check all objects
    for (const auto& sphere : spheres) {
        HitRecord hit = sphere.intersect(shadowRay);
        if (hit.hit && hit.t < lightDistance) {
            return true;  // Object blocks the light
        }
    }
    
    return false;  // Clear path to light
}
```

## Visual Concept

```
        ☀ Light
       ╱│
      ╱ │
     ╱  │
    ╱   │    Shadow ray
   ╱    │    blocked!
  ╱     ▼
 ●──────●──────────────── Ground
 Sphere  └── Shadow region
```

## Hard vs Soft Shadows

### Hard Shadows (Current Implementation)

Point lights create hard shadows with sharp edges:

```cpp
float shadowFactor = isInShadow(point, light.position) ? 0.3f : 1.0f;
```

We use `0.3` instead of `0.0` to avoid completely black shadows, which look unnatural.

### Soft Shadows (Future Enhancement)

Area lights create soft shadows with gradual falloff:

```cpp
// Sample multiple points on the light
float shadow = 0.0f;
for (int i = 0; i < samples; i++) {
    Vec3 lightPoint = light.position + randomOffset();
    if (!isInShadow(point, lightPoint)) {
        shadow += 1.0f;
    }
}
shadow /= samples;  // Average occlusion
```

## Shadow Acne

A common artifact where surfaces incorrectly shadow themselves due to floating-point precision:

```
Without offset:          With offset:
                        
  Surface ●═══════       Surface ●═══════
           ╲                      ╲
            ╲ Ray starts ON        ╲ Ray starts ABOVE
             ╲ surface (bad!)       ╲ surface (good!)
```

Solution: Offset the shadow ray origin:

```cpp
Ray shadowRay(point + lightDir * 0.001f, lightDir);
//                           ↑
//                    Small offset (epsilon)
```

## Shadow Application

Shadows are applied in the lighting calculation:

```cpp
Vec3 calculateLighting(const Ray& ray, const HitRecord& hit) {
    Vec3 color(0, 0, 0);

    for (const auto& light : lights) {
        Vec3 lightDir = (light.position - hit.point).normalize();
        
        // Check shadow
        float shadowFactor = 1.0f;
        if (isInShadow(hit.point, light.position)) {
            shadowFactor = 0.3f;
        }
        
        // Apply to diffuse and specular
        float diff = max(0, hit.normal.dot(lightDir));
        Vec3 diffuse = hit.material.color * diff * hit.material.diffuse;
        
        float spec = pow(max(0, hit.normal.dot(halfDir)), shininess);
        Vec3 specular = light.color * spec * hit.material.specularIntensity;
        
        color += (diffuse + specular) * light.intensity * shadowFactor;
    }

    // Ambient is NOT affected by shadows
    color += hit.material.color * hit.material.ambient;

    return color;
}
```

Note: Ambient light is not affected by shadows, ensuring shadowed areas aren't completely black.

## Shadows on Ground Plane

The ground plane receives shadows from spheres:

```
Side view:
        
    ☀
     ╲
      ╲
       ●  Sphere
      ╱│╲
     ╱ │ ╲
────●──┴──●──── Ground
    └────┘
     Shadow
```

This is automatically handled since the shadow ray checks all objects including spheres.

## Light Position Effects

Moving the light changes shadow direction and size:

| Light Position | Shadow Effect |
|---------------|---------------|
| High above | Short shadows |
| Low angle | Long shadows |
| Behind object | Shadow toward camera |
| In front | Shadow away from camera |

Use the Light tab sliders to experiment with different positions.

## Performance

Shadow rays add significant cost:

- Without shadows: 1 ray per pixel
- With shadows: 1 + 1 ray per light per pixel

For N lights: `pixels × (1 + N)` rays

