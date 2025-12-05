---
sidebar_position: 7
---

# Box - Axis-Aligned Bounding Box

The `Box` struct represents an axis-aligned bounding box (AABB), one of the fundamental primitives in ray tracing.

## Definition

```cpp title="cpp/include/Box.h"
struct Box {
    Vec3 center;      // Center point of the box
    Vec3 halfSize;    // Half-extents in each dimension
    Material material;

    Box() 
        : center(Vec3(0.0f, 0.0f, 0.0f))
        , halfSize(Vec3(0.5f, 0.5f, 0.5f))
        , material() {}
    
    Box(const Vec3& c, const Vec3& size, const Material& mat) 
        : center(c)
        , halfSize(size * 0.5f)  // Convert full size to half-size
        , material(mat) {}
};
```

## Factory Methods

```cpp
// Create a unit cube (1x1x1) at a position
static Box unitCube(const Vec3& center, const Material& mat);
```

## Accessor Methods

```cpp
// Get the minimum corner (bottom-left-back)
Vec3 getMin() const {
    return Vec3(center.x - halfSize.x, 
                center.y - halfSize.y, 
                center.z - halfSize.z);
}

// Get the maximum corner (top-right-front)
Vec3 getMax() const {
    return Vec3(center.x + halfSize.x, 
                center.y + halfSize.y, 
                center.z + halfSize.z);
}
```

## Ray Intersection

The box uses the **slab method** for ray intersection, testing each axis independently:

```cpp
HitRecord intersect(const Ray& ray) const {
    HitRecord record;
    
    Vec3 minB = getMin();
    Vec3 maxB = getMax();

    float tmin = -1e30f;
    float tmax = 1e30f;
    Vec3 normalMin, normalMax;

    // Test X, Y, Z slabs
    for (each axis) {
        float t1 = (minB.axis - ray.origin.axis) / ray.direction.axis;
        float t2 = (maxB.axis - ray.origin.axis) / ray.direction.axis;
        
        // Find entry and exit points
        tNear = min(t1, t2);
        tFar = max(t1, t2);
        
        // Track the normal at entry point
        if (tNear > tmin) {
            tmin = tNear;
            normalMin = (entry normal for this axis);
        }
        if (tFar < tmax) {
            tmax = tFar;
        }
    }

    // Valid hit if entry before exit and exit in front of ray
    if (tmin > tmax || tmax < 0.001f) {
        return record;  // No hit
    }

    // Use appropriate t and normal
    record.t = (tmin > 0.001f) ? tmin : tmax;
    record.point = ray.at(record.t);
    record.normal = normalMin;
    record.material = material;
    record.hit = true;

    return record;
}
```

## The Slab Method

The slab method works by:

1. Treating the box as the intersection of 3 pairs of parallel planes (slabs)
2. For each axis, find where the ray enters and exits that slab
3. The ray is inside the box when it's inside ALL slabs simultaneously
4. Entry point = maximum of all entry distances
5. Exit point = minimum of all exit distances

```
       ┌─────────┐
       │   Box   │
    ───┼─────────┼───▶ Ray
       │         │
       └─────────┘
       ↑         ↑
      tmin      tmax
     (entry)   (exit)
```

**Valid intersection:** `tmin < tmax` and `tmax > 0`

## Normal Calculation

The normal depends on which face was hit:

| Face | Normal |
|------|--------|
| Left (-X) | (-1, 0, 0) |
| Right (+X) | (1, 0, 0) |
| Bottom (-Y) | (0, -1, 0) |
| Top (+Y) | (0, 1, 0) |
| Back (-Z) | (0, 0, -1) |
| Front (+Z) | (0, 0, 1) |

## Usage Example

```cpp
// Create a blue cube
Material blueMat(Vec3(0.2f, 0.4f, 0.9f), 0.7f, 64.0f);
blueMat.reflectivity = 0.3f;

// 1.5 units wide, at position (-2, 0, 0)
Box cube(Vec3(-2.0f, 0.0f, 0.0f), Vec3(1.5f, 1.5f, 1.5f), blueMat);

// Check intersection
HitRecord hit = cube.intersect(ray);
if (hit.hit) {
    // Process hit...
}
```

## Why Axis-Aligned?

AABB boxes have edges parallel to the coordinate axes. This constraint:

- **Simplifies intersection math** - No rotation matrices needed
- **Enables fast rejection** - Quick bounding box tests
- **Efficient in most scenes** - Many objects are naturally axis-aligned

For rotated boxes, you would transform the ray into box-local space, intersect with an AABB, then transform results back.

## Performance

AABB intersection is very fast:
- Only 6-12 floating point operations per axis
- No transcendental functions (sqrt, sin, cos)
- Often used for bounding volume hierarchies (BVH)

| Operation | Cost |
|-----------|------|
| Box intersection | ~20-30 FLOPs |
| Sphere intersection | ~15-25 FLOPs |
| Triangle intersection | ~50-80 FLOPs |

