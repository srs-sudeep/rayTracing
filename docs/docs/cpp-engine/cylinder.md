---
sidebar_position: 8
---

# Cylinder

The `Cylinder` struct represents a cylinder aligned along the Y-axis with optional end caps.

## Definition

```cpp title="cpp/include/Cylinder.h"
struct Cylinder {
    Vec3 center;       // Center of the base
    float radius;      // Radius of the cylinder
    float height;      // Height (extends upward from center)
    Material material;
    bool capped;       // Whether to render end caps

    Cylinder() 
        : center(Vec3(0.0f, 0.0f, 0.0f))
        , radius(0.5f)
        , height(1.0f)
        , material()
        , capped(true) {}
    
    Cylinder(const Vec3& c, float r, float h, const Material& mat, bool caps = true) 
        : center(c)
        , radius(r)
        , height(h)
        , material(mat)
        , capped(caps) {}
};
```

## Geometry

The cylinder is defined by:
- **Base center** - The center point at the bottom
- **Radius** - Distance from axis to surface
- **Height** - How tall (extends in +Y direction)

```
        ╭───────╮  ← top cap (y = center.y + height)
       ╱         ╲
      │           │
      │  curved   │  ← cylinder body
      │  surface  │
       ╲         ╱
        ╰───────╯  ← bottom cap (y = center.y)
           ↑
        center
```

## Ray Intersection

The intersection test has two parts:

### 1. Curved Surface (Infinite Cylinder)

For an infinite cylinder along the Y-axis, a point (x, z) is on the surface if:

```
x² + z² = r²
```

Substituting the ray equation:
```
(origin.x + t·dir.x)² + (origin.z + t·dir.z)² = r²
```

This becomes a quadratic equation:
```cpp
float a = rd.x * rd.x + rd.z * rd.z;
float b = 2.0f * (ro.x * rd.x + ro.z * rd.z);
float c = ro.x * ro.x + ro.z * ro.z - radius * radius;

float discriminant = b * b - 4.0f * a * c;
```

After finding t, check if the hit point is within the height bounds.

### 2. End Caps

If `capped` is true, test intersection with two planes:
- Bottom cap at `y = center.y`
- Top cap at `y = center.y + height`

For each cap, check if the hit point is within the radius:
```cpp
float dist2 = (hit.x - center.x)² + (hit.z - center.z)²;
if (dist2 <= radius * radius) {
    // Valid cap hit
}
```

### Combined Test

```cpp
HitRecord intersect(const Ray& ray) const {
    HitRecord record;
    
    // 1. Test curved surface
    // Solve quadratic, check height bounds
    // Track closest valid hit
    
    // 2. Test caps (if enabled)
    // Test bottom cap plane
    // Test top cap plane
    // Check radius constraint
    
    // 3. Return closest of all hits
    return record;
}
```

## Normal Calculation

| Surface | Normal Direction |
|---------|-----------------|
| Curved surface | Radially outward: `normalize(hit.x - center.x, 0, hit.z - center.z)` |
| Top cap | Straight up: `(0, 1, 0)` |
| Bottom cap | Straight down: `(0, -1, 0)` |

## Usage Example

```cpp
// Create a green cylinder
Material greenMat(Vec3(0.2f, 0.85f, 0.4f), 0.6f, 48.0f);
greenMat.reflectivity = 0.25f;

// Cylinder at (2, -1, 0), radius 0.5, height 1.4
Cylinder cyl(Vec3(2.0f, -1.0f, 0.0f), 0.5f, 1.4f, greenMat);

// Uncapped cylinder (tube)
Cylinder tube(Vec3(0, 0, 0), 0.3f, 2.0f, mat, false);
```

## Capped vs Uncapped

| Capped | Use Case |
|--------|----------|
| `true` | Solid cylinder (default) - cans, pillars |
| `false` | Hollow tube - pipes, rings |

```
Capped:          Uncapped:
╭───────╮        ╭ ─ ─ ─ ╮
│       │        │       │
│       │        │       │
╰───────╯        ╰ ─ ─ ─ ╯
```

## Why Y-Axis Aligned?

The cylinder is aligned along the Y-axis for simplicity:
- Easier intersection math (only XZ plane quadratic)
- Natural orientation (standing upright)
- Consistent with ground plane

For different orientations, you would:
1. Transform the ray into cylinder-local space
2. Perform the intersection
3. Transform results back

## Performance

| Test | Approximate Cost |
|------|------------------|
| Curved surface | Quadratic solve (~15 FLOPs) |
| Height check | 2 comparisons |
| Each cap | 1 division, 2 multiplies, 1 compare |
| Total | ~25-35 FLOPs |

The cylinder is more expensive than AABB but cheaper than many curved surfaces.

## Mathematical Background

### Quadratic Formula

For `at² + bt + c = 0`:
```
t = (-b ± √(b² - 4ac)) / 2a
```

- **Discriminant < 0**: No intersection (ray misses)
- **Discriminant = 0**: One intersection (tangent)
- **Discriminant > 0**: Two intersections (through)

### Choosing the Right t

With two solutions:
- `t1` (smaller) = entry point
- `t2` (larger) = exit point

We want the first valid hit:
1. Check `t1` first - if valid and in bounds, use it
2. Otherwise check `t2` - ray might start inside cylinder

