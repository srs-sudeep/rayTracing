---
sidebar_position: 3
---

# Ray

A ray is the fundamental geometric primitive in ray tracing. It represents a line starting from a point and extending infinitely in a direction.

## Definition

```cpp title="cpp/include/Ray.h"
struct Ray {
    Vec3 origin;     // Starting point
    Vec3 direction;  // Normalized direction vector

    Ray() : origin(), direction(Vec3(0, 0, 1)) {}
    
    Ray(const Vec3& o, const Vec3& d) 
        : origin(o), direction(d.normalize()) {}

    // Get point along ray at parameter t
    Vec3 at(float t) const {
        return origin + direction * t;
    }
};
```

## Ray Equation

A ray is mathematically defined as:

```
P(t) = O + t * D

Where:
  P(t) = Point on ray at parameter t
  O    = Origin (starting point)
  D    = Direction (unit vector)
  t    = Parameter (distance along ray)
```

## Key Properties

### Origin

The starting point of the ray. For primary rays, this is the camera position.

### Direction

A **normalized** (unit length) vector indicating the ray's direction. Normalization ensures consistent intersection calculations.

### Parameter t

The parameter `t` represents distance along the ray:
- `t = 0`: At the origin
- `t > 0`: In front of origin (valid intersections)
- `t < 0`: Behind origin (ignored)

## Usage in Ray Tracing

### Primary Rays (Camera Rays)

```cpp
Ray Camera::getRay(float u, float v) const {
    // u, v are normalized screen coordinates [-1, 1]
    Vec3 horizontal = right * (u * viewportWidth * 0.5f);
    Vec3 vertical = upDir * (v * viewportHeight * 0.5f);
    Vec3 direction = forward + horizontal + vertical;
    return Ray(position, direction);
}
```

### Shadow Rays

```cpp
// Check if point is in shadow
bool Scene::isInShadow(const Vec3& point, const Vec3& lightPos) const {
    Vec3 toLight = lightPos - point;
    float lightDistance = toLight.length();
    Vec3 lightDir = toLight.normalize();
    
    // Offset origin slightly to avoid self-intersection
    Ray shadowRay(point + lightDir * 0.001f, lightDir);
    
    // Check for blockers
    for (const auto& sphere : spheres) {
        HitRecord hit = sphere.intersect(shadowRay);
        if (hit.hit && hit.t < lightDistance) {
            return true;  // Something blocks the light
        }
    }
    return false;
}
```

### Reflection Rays

```cpp
// Trace reflected ray
Vec3 reflectDir = rayDirection.reflect(normal);
Vec3 reflectOrigin = hitPoint + normal * 0.001f;  // Offset to avoid self-hit
Ray reflectRay(reflectOrigin, reflectDir);

Vec3 reflectedColor = traceRay(reflectRay, depth + 1);
```

## Ray-Object Intersection

The `at(t)` function is crucial for finding intersection points:

```cpp
HitRecord Sphere::intersect(const Ray& ray) const {
    // ... solve for t using quadratic formula ...
    
    if (hasIntersection) {
        HitRecord record;
        record.t = t;
        record.point = ray.at(t);  // Get intersection point
        record.normal = (record.point - center).normalize();
        return record;
    }
}
```

## Self-Intersection Prevention

When spawning secondary rays (shadow, reflection), offset the origin slightly:

```cpp
// BAD: May hit the same surface due to floating-point errors
Ray shadowRay(hitPoint, lightDir);

// GOOD: Offset along normal to avoid self-intersection
Ray shadowRay(hitPoint + normal * 0.001f, lightDir);
```

The offset value (0.001) is called the "epsilon" or "shadow bias".

## Diagram

```
                 Direction (D)
                     ↗
                    /
                   /
     Origin (O)   /
         •───────•───────•───────→
         t=0     t=1     t=2
                 
         P(0)=O  P(1)    P(2)
```

