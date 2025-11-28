---
sidebar_position: 2
---

# Vec3 - 3D Vector

The `Vec3` struct is the foundation of all 3D mathematics in the ray tracer. It represents points, directions, colors, and more.

## Definition

```cpp title="cpp/include/Vec3.h"
struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};
```

## Operations

### Arithmetic Operations

```cpp
// Addition - combine vectors
Vec3 operator+(const Vec3& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

// Subtraction - find difference
Vec3 operator-(const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

// Scalar multiplication - scale vector
Vec3 operator*(float t) const {
    return Vec3(x * t, y * t, z * t);
}

// Component-wise multiplication - for colors
Vec3 operator*(const Vec3& v) const {
    return Vec3(x * v.x, y * v.y, z * v.z);
}
```

### Vector Products

```cpp
// Dot product - measure alignment
float dot(const Vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

// Cross product - find perpendicular vector
Vec3 cross(const Vec3& v) const {
    return Vec3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}
```

### Length and Normalization

```cpp
// Length (magnitude)
float length() const {
    return std::sqrt(x * x + y * y + z * z);
}

// Squared length (faster, for comparisons)
float lengthSquared() const {
    return x * x + y * y + z * z;
}

// Unit vector (length = 1)
Vec3 normalize() const {
    float len = length();
    if (len > 0) {
        return Vec3(x / len, y / len, z / len);
    }
    return Vec3(0, 0, 0);
}
```

### Reflection

```cpp
// Reflect vector around normal (for mirror reflections)
Vec3 reflect(const Vec3& normal) const {
    return *this - normal * (2.0f * this->dot(normal));
}
```

### Color Utilities

```cpp
// Clamp RGB values to [0, 1] range
Vec3 clamp() const {
    return Vec3(
        std::fmax(0.0f, std::fmin(1.0f, x)),
        std::fmax(0.0f, std::fmin(1.0f, y)),
        std::fmax(0.0f, std::fmin(1.0f, z))
    );
}
```

## Usage Examples

### Representing Points

```cpp
Vec3 cameraPosition(0.0f, 0.5f, -4.0f);
Vec3 sphereCenter(0.0f, 0.0f, 0.0f);
```

### Representing Directions

```cpp
Vec3 up(0.0f, 1.0f, 0.0f);
Vec3 rayDirection = (target - origin).normalize();
```

### Representing Colors

```cpp
Vec3 red(0.9f, 0.2f, 0.15f);    // RGB normalized to [0, 1]
Vec3 white(1.0f, 1.0f, 1.0f);
Vec3 background(0.05f, 0.05f, 0.08f);
```

### Lighting Calculations

```cpp
// Diffuse lighting (Lambertian)
Vec3 lightDir = (lightPos - hitPoint).normalize();
float diffuse = std::max(0.0f, normal.dot(lightDir));
Vec3 diffuseColor = surfaceColor * diffuse;

// Specular highlight (Blinn-Phong)
Vec3 halfDir = (lightDir + viewDir).normalize();
float specular = std::pow(std::max(0.0f, normal.dot(halfDir)), shininess);
Vec3 specularColor = lightColor * specular;
```

## Mathematical Background

### Dot Product

The dot product measures how parallel two vectors are:
- **Parallel same direction**: `a · b = |a| * |b|`
- **Perpendicular**: `a · b = 0`
- **Parallel opposite**: `a · b = -|a| * |b|`

Used for:
- Lighting calculations (N · L)
- Checking if ray hits front of surface
- Finding angles between vectors

### Cross Product

The cross product finds a vector perpendicular to both inputs:
- Result is perpendicular to the plane containing both vectors
- Follows right-hand rule for direction

Used for:
- Building camera coordinate system
- Computing surface normals

### Reflection Formula

For a vector V reflecting off a surface with normal N:

```
R = V - 2(V · N)N
```

This is used for mirror reflections in recursive ray tracing.

