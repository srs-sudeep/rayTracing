---
sidebar_position: 7
---

# Sphere

The `Sphere` struct represents a spherical object that can be ray traced.

## Definition

```cpp title="cpp/include/Sphere.h"
struct HitRecord {
    float t;           // Distance along ray
    Vec3 point;        // Intersection point
    Vec3 normal;       // Surface normal at intersection
    Material material; // Material at this point
    bool hit;          // Whether there was a hit

    HitRecord() : t(-1), hit(false) {}
};

struct Sphere {
    Vec3 center;       // Center position
    float radius;      // Sphere radius
    Material material; // Surface material

    Sphere() : center(), radius(1.0f), material() {}
    
    Sphere(const Vec3& c, float r, const Material& mat) 
        : center(c), radius(r), material(mat) {}
};
```

## Ray-Sphere Intersection

The core of ray tracing is determining where a ray hits a sphere.

### Mathematical Background

A sphere is defined as all points at distance `r` from center `C`:

```
|P - C|² = r²
```

A ray is:
```
P(t) = O + t*D
```

Substituting:
```
|O + t*D - C|² = r²
```

Expanding:
```
(D·D)t² + 2(D·(O-C))t + ((O-C)·(O-C) - r²) = 0
```

This is a quadratic equation: `at² + bt + c = 0`

### Implementation

```cpp
HitRecord Sphere::intersect(const Ray& ray) const {
    HitRecord record;
    
    Vec3 oc = ray.origin - center;
    
    // Quadratic coefficients
    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - radius * radius;
    
    // Discriminant
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return record;  // No intersection
    }

    // Find nearest positive t
    float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
    if (t <= 0.001f) {
        t = (-b + std::sqrt(discriminant)) / (2.0f * a);
        if (t <= 0.001f) {
            return record;  // Behind camera
        }
    }

    // Fill hit record
    record.t = t;
    record.point = ray.at(t);
    record.normal = (record.point - center).normalize();
    record.material = material;
    record.hit = true;

    return record;
}
```

### Discriminant Cases

```
discriminant < 0:  Ray misses sphere
discriminant = 0:  Ray tangent to sphere (one point)
discriminant > 0:  Ray intersects sphere (two points)
```

```
        Miss              Tangent           Through
        
    ○                    ○───●             ○───●───●
   ╱                    ╱                 ╱         ╲
  →                    →                 →           →
```

## Surface Normal

The normal at any point on a sphere points directly outward from the center:

```cpp
Vec3 getNormal(const Vec3& point) const {
    return (point - center).normalize();
}
```

```
           ↗ Normal
          ╱
    ●────╱────● Surface point
   ╱    ╱
  ●    Center
```

## Scene Usage

Spheres are stored in the scene's vector:

```cpp
class Scene {
    std::vector<Sphere> spheres;
    
    void addSphere(const Vec3& center, float radius, const Material& mat) {
        spheres.push_back(Sphere(center, radius, mat));
    }
};
```

## Scene Presets

Different sphere arrangements are defined as presets:

```cpp
void loadPreset(ScenePreset preset) {
    spheres.clear();

    switch (preset) {
        case ScenePreset::SINGLE_SPHERE: {
            Material redMat(Vec3(0.9f, 0.2f, 0.15f), 0.5f, 32.0f);
            redMat.reflectivity = 0.3f;
            spheres.push_back(Sphere(Vec3(0, 0, 0), 1.0f, redMat));
            break;
        }

        case ScenePreset::THREE_SPHERES: {
            // Red center
            Material redMat(Vec3(0.9f, 0.2f, 0.15f), 0.6f, 64.0f);
            redMat.reflectivity = 0.3f;
            spheres.push_back(Sphere(Vec3(0, 0, 0), 1.0f, redMat));

            // Blue left
            Material blueMat(Vec3(0.2f, 0.4f, 0.9f), 0.7f, 128.0f);
            blueMat.reflectivity = 0.5f;
            spheres.push_back(Sphere(Vec3(-2.2f, -0.3f, 0.5f), 0.7f, blueMat));

            // Green right
            Material greenMat(Vec3(0.2f, 0.85f, 0.3f), 0.5f, 48.0f);
            greenMat.reflectivity = 0.2f;
            spheres.push_back(Sphere(Vec3(2.0f, -0.5f, 1.0f), 0.5f, greenMat));
            break;
        }
        
        // ... more presets
    }
}
```

## Finding Closest Hit

When tracing, we need the closest intersection:

```cpp
HitRecord Scene::trace(const Ray& ray) const {
    HitRecord closest;
    closest.t = 1e30f;  // Very far
    closest.hit = false;

    for (const auto& sphere : spheres) {
        HitRecord hit = sphere.intersect(ray);
        if (hit.hit && hit.t < closest.t) {
            closest = hit;  // This one is closer
        }
    }

    return closest;
}
```

This ensures we always render the closest visible surface.

