---
sidebar_position: 3
---

# Shadows

Shadows add depth and realism to the scene by simulating light occlusion. RayTracer Studio supports both **hard shadows** (point lights) and **soft shadows** (area lights).

## Shadow Rays

For each point we shade, we check if it's visible from the light source by casting a **shadow ray**:

```cpp
bool isInShadowHard(const Vec3& point, const Vec3& lightPos) const {
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

## Hard Shadows (Point Lights)

Point lights create hard shadows with sharp edges:

```cpp
float shadowFactor = isInShadowHard(point, light.position) ? 0.3f : 1.0f;
```

We use `0.3` instead of `0.0` to avoid completely black shadows, which look unnatural.

```
Hard Shadow:
    ██████████
    ██████████
    ██████████   Sharp, defined edge
    ██████████
    ██████████
```

## Soft Shadows (Area Lights) ✨

Area lights create realistic soft shadows with gradual falloff (penumbra). This is achieved by treating each light as a sphere with a `radius` property and casting multiple shadow rays.

### How It Works

```
Soft Shadow:
    ▓▓▓▓▓▓▓▓▓▓
  ░░▓▓▓▓▓▓▓▓░░
░░░░▓▓▓▓▓▓░░░░   Gradual falloff
░░░░░░▓▓░░░░░░   (penumbra)
  ░░░░░░░░░░
```

Instead of sampling a single point, we sample multiple points on the light's surface:

```cpp
float calculateShadowFactor(const Vec3& point, const Light& light) const {
    if (!softShadowsEnabled || light.radius <= 0.0f) {
        // Hard shadows - simple binary test
        return isInShadowHard(point, light.position) ? 0.3f : 1.0f;
    }
    
    // Soft shadows - multiple samples on the area light
    int litSamples = 0;
    int sqrtSamples = static_cast<int>(std::sqrt(shadowSamples));
    
    for (int i = 0; i < sqrtSamples; ++i) {
        for (int j = 0; j < sqrtSamples; ++j) {
            // Stratified random offset within each cell
            float u = (i + random()) / sqrtSamples;
            float v = (j + random()) / sqrtSamples;
            
            // Get sample point on area light (disk facing the point)
            Vec3 samplePos = light.getSamplePointDisk(u, v, point);
            
            // Test shadow ray to this sample
            if (!isInShadowHard(point, samplePos)) {
                litSamples++;
            }
        }
    }
    
    // Calculate soft shadow factor (0.3 - 1.0)
    float visibility = float(litSamples) / float(totalSamples);
    return 0.3f + visibility * 0.7f;
}
```

### Light Sampling

The `Light` struct provides methods to sample points on its surface:

```cpp
struct Light {
    Vec3 position;
    Vec3 color;
    float intensity;
    float radius;  // Area light radius (0 = point light)

    // Sample point on disk facing target (most efficient)
    Vec3 getSamplePointDisk(float u, float v, const Vec3& target) const {
        if (radius <= 0.0f) return position;
        
        // Direction from light to target
        Vec3 toTarget = (target - position).normalize();
        
        // Create orthonormal basis on disk
        Vec3 right = toTarget.cross(up).normalize();
        Vec3 up = right.cross(toTarget).normalize();
        
        // Random point in unit disk (concentric mapping)
        float r = std::sqrt(u);
        float theta = 2.0f * PI * v;
        
        return position + right * (r * cos(theta) * radius) 
                        + up * (r * sin(theta) * radius);
    }
};
```

### Stratified Sampling

For smoother shadows, we use **stratified sampling** with jitter:

```
Regular Sampling:          Stratified + Jitter:

  ●   ●   ●   ●            ●    ●   ●    ●
                             ↘   ↙   ↘   ↙
  ●   ●   ●   ●              ●   ●    ●   ●
                               ↗   ↖   ↗
  ●   ●   ●   ●            ●   ●    ●    ●
                             ↙   ↗    ↖
  ●   ●   ●   ●              ●   ●   ●    ●

(Visible patterns)         (Smooth, no patterns)
```

## UI Controls

Soft shadows can be configured in the **View** tab:

| Control | Range | Effect |
|---------|-------|--------|
| **Soft Shadows** | On/Off | Enable area light shadows |
| **Light Size** | 0.1 - 1.5 | Larger = softer shadows |
| **Shadow Samples** | 4, 9, 16, 25 | More = smoother but slower |

## Performance Impact

Shadow calculations are the most expensive part of rendering:

| Shadow Type | Rays per Light | Relative Cost |
|-------------|----------------|---------------|
| Hard (Off) | 1 | 1× |
| Soft (4 samples) | 4 | ~4× |
| Soft (9 samples) | 9 | ~9× |
| Soft (16 samples) | 16 | ~16× |
| Soft (25 samples) | 25 | ~25× |

**Tip:** Use lower resolution or disable AA when using many shadow samples.

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
Vec3 calculateLocalLighting(const Ray& ray, const HitRecord& hit) const {
    Vec3 color(0, 0, 0);

    for (const auto& light : lights) {
        Vec3 lightDir = (light.position - hit.point).normalize();
        
        // Calculate shadow factor (soft or hard)
        float shadowFactor = calculateShadowFactor(hit.point, light);
        
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

## Light Position Effects

Moving the light changes shadow direction and softness:

| Light Position | Shadow Effect |
|---------------|---------------|
| High above | Short shadows |
| Low angle | Long shadows |
| Close to object | Sharper soft shadows |
| Far from object | Softer soft shadows |

Use the Light tab sliders to experiment with different positions.
