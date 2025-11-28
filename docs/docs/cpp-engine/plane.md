---
sidebar_position: 8
---

# Plane

The `Plane` struct represents an infinite ground plane with a Blender-style grid pattern.

## Definition

```cpp title="cpp/include/Plane.h"
struct Plane {
    Vec3 point;          // A point on the plane
    Vec3 normal;         // Plane normal (pointing up)
    Material material;   // Surface material
    bool showGrid;       // Whether to show grid lines
    float gridScale;     // Size of grid cells
    float gridThickness; // Width of grid lines

    Plane() 
        : point(Vec3(0.0f, -1.0f, 0.0f))
        , normal(Vec3(0.0f, 1.0f, 0.0f))
        , showGrid(true)
        , gridScale(1.0f)
        , gridThickness(0.02f)
    {
        material.color = Vec3(0.15f, 0.15f, 0.17f);  // Dark gray
        material.specularIntensity = 0.1f;
        material.shininess = 16.0f;
    }
};
```

## Ray-Plane Intersection

A plane is defined by a point `P₀` and normal `N`. A point is on the plane if:

```
(P - P₀) · N = 0
```

### Implementation

```cpp
HitRecord Plane::intersect(const Ray& ray) const {
    HitRecord record;
    
    // Check if ray is parallel to plane
    float denom = normal.dot(ray.direction);
    if (std::abs(denom) < 0.0001f) {
        return record;  // Parallel, no intersection
    }

    // Calculate t parameter
    Vec3 p0l0 = point - ray.origin;
    float t = p0l0.dot(normal) / denom;

    if (t < 0.001f) {
        return record;  // Behind camera
    }

    record.t = t;
    record.point = ray.at(t);
    record.normal = normal;
    record.material = material;
    record.hit = true;

    // Apply grid pattern
    if (showGrid) {
        applyGridPattern(record);
    }

    return record;
}
```

## Grid Pattern

The grid creates a Blender-style infinite floor with axes and grid lines:

```cpp
void applyGridPattern(HitRecord& record) const {
    float x = record.point.x;
    float z = record.point.z;

    // Scale coordinates by grid size
    float scaledX = x / gridScale;
    float scaledZ = z / gridScale;

    // Distance from nearest grid line
    float lineX = std::abs(scaledX - std::round(scaledX));
    float lineZ = std::abs(scaledZ - std::round(scaledZ));

    // Fade grid with distance (for realism)
    float distance = std::sqrt(x * x + z * z);
    float fade = std::exp(-distance * 0.05f);
    fade = std::fmax(0.0f, std::fmin(1.0f, fade));

    // Check for axis lines and grid lines
    bool onMainAxisX = std::abs(x) < gridThickness * 2.0f;
    bool onMainAxisZ = std::abs(z) < gridThickness * 2.0f;
    bool onGridLine = lineX < gridThickness || lineZ < gridThickness;

    // Major grid lines (every 5 units)
    float majorLineX = std::abs((scaledX / 5.0f) - std::round(scaledX / 5.0f));
    float majorLineZ = std::abs((scaledZ / 5.0f) - std::round(scaledZ / 5.0f));
    bool onMajorLine = majorLineX < gridThickness * 0.6f || 
                       majorLineZ < gridThickness * 0.6f;

    // Apply colors
    Vec3 baseColor = material.color;
    
    if (onMainAxisX) {
        // Z-axis (teal)
        record.material.color = Vec3(0.2f, 0.5f, 0.5f) * fade + baseColor * (1.0f - fade);
    } else if (onMainAxisZ) {
        // X-axis (red)
        record.material.color = Vec3(0.5f, 0.2f, 0.2f) * fade + baseColor * (1.0f - fade);
    } else if (onMajorLine) {
        // Major lines - brighter
        float brightness = 0.25f * fade;
        record.material.color = baseColor + Vec3(brightness, brightness, brightness);
    } else if (onGridLine) {
        // Minor lines - subtle
        float brightness = 0.08f * fade;
        record.material.color = baseColor + Vec3(brightness, brightness, brightness);
    }
}
```

## Grid Features

### Axis Lines

Color-coded axes for orientation:
- **X-axis**: Red tint
- **Z-axis**: Teal tint

### Major/Minor Lines

Two levels of grid detail:
- **Major lines**: Every 5 units, brighter
- **Minor lines**: Every 1 unit, subtle

### Distance Fade

Grid lines fade with distance using exponential falloff:

```cpp
float fade = std::exp(-distance * 0.05f);
```

This prevents visual clutter in the distance and creates natural perspective.

## UI Controls

Grid settings are in the View tab:

| Control | Description |
|---------|-------------|
| Ground Plane | Toggle plane visibility |
| Grid Lines | Toggle grid pattern |
| Grid Scale | Size of grid cells (0.5x - 4x) |
| Ground Reflect | Plane reflectivity |

## Visual Result

```
Far (faded)
     │     │     │     │
─────┼─────┼─────┼─────┼─────
     │     │     │     │
═════╪═════╪═════╪═════╪═════  Major lines (every 5)
     │     │     │     │
─────┼─────┼─────┼─────┼─────  Minor lines
     │  Z-axis (teal)  │
═════╪══●══╪═════╪═════╪═════  X-axis (red)
     │     │     │     │
─────┼─────┼─────┼─────┼─────
     │     │     │     │
Near (sharp)
```

## Shadows on Plane

The plane participates in shadow calculations:

```cpp
// In Scene::shade()
if (isInShadow(hit.point, light.position)) {
    shadowFactor = 0.3f;  // Darken shadowed areas
}
```

Spheres cast shadows onto the ground plane, enhancing depth perception.

## Reflections

The plane can reflect spheres when `reflectivity > 0`:

```cpp
groundPlane.material.reflectivity = 0.15f;  // Subtle reflection
```

This creates a polished floor effect where spheres are partially visible in the ground.

