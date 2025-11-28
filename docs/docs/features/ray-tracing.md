---
sidebar_position: 1
---

# Ray Tracing Fundamentals

This page explains the core ray tracing algorithm implemented in RayTracer Studio.

## What is Ray Tracing?

Ray tracing is a rendering technique that simulates the physical behavior of light. Instead of calculating how light bounces around a scene (which is computationally expensive), we trace rays **backwards** from the camera into the scene.

```
Traditional Light:    Ray Tracing:
                     
    ☀ ─→ → → → 👁      ☀ ← ← ← ← 👁
    Light to Eye       Eye to Light
```

## The Algorithm

For each pixel in the image:

1. **Generate a ray** from the camera through the pixel
2. **Find intersections** with objects in the scene
3. **Calculate shading** at the closest intersection
4. **Write color** to the output buffer

```cpp
for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
        Ray ray = camera.getRay(x, y);
        HitRecord hit = scene.trace(ray);
        Vec3 color = scene.shade(ray, hit);
        buffer[y * width + x] = color;
    }
}
```

## Ray Generation

Rays originate from the camera and pass through each pixel:

```cpp
Ray Camera::getRay(float u, float v) const {
    // u, v are normalized screen coordinates [-1, 1]
    Vec3 horizontal = right * (u * viewportWidth * 0.5f);
    Vec3 vertical = upDir * (v * viewportHeight * 0.5f);
    Vec3 direction = forward + horizontal + vertical;
    return Ray(position, direction);
}
```

```
         Image Plane
        ┌───────────┐
        │  ● pixel  │
        │ ╱         │
        │╱          │
     ●──────────────  Camera
```

## Intersection Testing

### Ray-Sphere Intersection

A sphere is defined as points at distance `r` from center `C`:

```
|P - C|² = r²
```

Substituting the ray equation `P = O + tD`:

```
|O + tD - C|² = r²
```

This gives a quadratic equation in `t`:

```
at² + bt + c = 0

where:
  a = D·D
  b = 2D·(O-C)
  c = (O-C)·(O-C) - r²
```

```cpp
float discriminant = b*b - 4*a*c;

if (discriminant < 0) {
    // No intersection
} else {
    float t = (-b - sqrt(discriminant)) / (2*a);
    // t is the distance to intersection
}
```

### Ray-Plane Intersection

A plane is defined by point `P₀` and normal `N`:

```
(P - P₀) · N = 0
```

Solving for `t`:

```
t = (P₀ - O) · N / (D · N)
```

## Shading Model

We use the **Blinn-Phong** shading model, which combines:

### Ambient Light

Simulates indirect illumination. Prevents completely black shadows:

```cpp
Vec3 ambient = color * 0.1f;
```

### Diffuse Reflection (Lambertian)

Light scatters equally in all directions. Intensity depends on angle:

```cpp
float diffuse = max(0, N · L);  // N = normal, L = light direction
Vec3 diffuseColor = color * diffuse;
```

### Specular Highlight

Shiny spots from direct reflection:

```cpp
Vec3 H = normalize(L + V);  // Half-vector
float specular = pow(max(0, N · H), shininess);
Vec3 specularColor = lightColor * specular;
```

### Combined

```cpp
Vec3 finalColor = ambient + (diffuse + specular) * shadowFactor;
```

## Shadow Rays

To check if a point is in shadow:

1. Cast a ray from the point toward the light
2. If it hits anything before reaching the light, the point is in shadow

```cpp
bool isInShadow(Vec3 point, Vec3 lightPos) {
    Vec3 toLight = lightPos - point;
    float distance = toLight.length();
    Ray shadowRay(point + offset, normalize(toLight));
    
    for (object in scene) {
        if (object.intersect(shadowRay).t < distance) {
            return true;  // Blocked!
        }
    }
    return false;
}
```

## Putting It Together

```cpp
Vec3 shade(Ray ray, HitRecord hit) {
    Vec3 color = Vec3(0, 0, 0);
    
    for (light in lights) {
        Vec3 L = normalize(light.position - hit.point);
        Vec3 V = normalize(ray.origin - hit.point);
        Vec3 H = normalize(L + V);
        
        // Diffuse
        float diff = max(0, hit.normal.dot(L));
        
        // Specular
        float spec = pow(max(0, hit.normal.dot(H)), shininess);
        
        // Shadow
        float shadow = isInShadow(hit.point, light.position) ? 0.3 : 1.0;
        
        color += (diffuse * diff + specular * spec) * shadow;
    }
    
    // Ambient
    color += hit.material.color * ambient;
    
    return color;
}
```

## Performance

Ray tracing is computationally intensive:

| Resolution | Rays | Time Factor |
|------------|------|-------------|
| 256×256 | 65,536 | 1× |
| 512×512 | 262,144 | 4× |
| 1024×1024 | 1,048,576 | 16× |

Each ray may spawn additional rays for:
- Shadows (1 per light)
- Reflections (1 per bounce)
- Future: Refractions, ambient occlusion

