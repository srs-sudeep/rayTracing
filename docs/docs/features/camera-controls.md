---
sidebar_position: 5
---

# Camera Controls

The camera system provides full control over viewpoint, including position, target, field of view, and interactive orbit.

## Camera Properties

```cpp
struct Camera {
    Vec3 position;       // Where the camera is
    Vec3 target;         // What it's looking at
    float fov;           // Field of view (degrees)
    float aspectRatio;   // Width / height
};
```

## Position

The camera's location in 3D space:

| Axis | Range | Effect |
|------|-------|--------|
| X | -10 to 10 | Left/Right |
| Y | -10 to 10 | Down/Up |
| Z | -15 to -1 | Far/Near |

Default: `(0, 0.5, -4)` - slightly elevated, looking at origin.

## Look-At Target

The point in space the camera focuses on:

```cpp
void setTarget(float x, float y, float z) {
    target = Vec3(x, y, z);
    updateBasis();
}
```

| Axis | Range | Effect |
|------|-------|--------|
| X | -5 to 5 | Look left/right |
| Y | -5 to 5 | Look down/up |
| Z | -5 to 5 | Look back/forward |

Moving the target:
- Tilts the camera without moving it
- Useful for focusing on specific objects
- Default: `(0, 0, 0)` - scene center

## Field of View

Controls how wide the camera sees:

```cpp
void setFov(float newFov) {
    fov = clamp(newFov, 10.0f, 120.0f);
    updateBasis();
}
```

| FOV | Effect |
|-----|--------|
| 20° | Telephoto - objects appear flatter, closer |
| 60° | Normal - similar to human vision |
| 90° | Wide - more visible, some distortion |
| 120° | Ultra wide - extreme edge distortion |

```
Narrow FOV:           Wide FOV:
    ╱╲                ╱────────╲
   ╱  ╲              ╱          ╲
  ●    ●            ●            ●
```

## Interactive Controls

### Orbit (Mouse Drag)

Click and drag on the canvas to orbit the camera around the target:

```cpp
void orbit(float deltaX, float deltaY) {
    Vec3 offset = position - target;
    float radius = offset.length();
    
    // Spherical coordinates
    float theta = atan2(offset.x, offset.z);  // Azimuth
    float phi = acos(offset.y / radius);       // Elevation
    
    // Apply rotation
    theta += deltaX * 0.01f;
    phi += deltaY * 0.01f;
    phi = clamp(phi, 0.1f, 3.04f);  // Prevent flip
    
    // Back to Cartesian
    position.x = target.x + radius * sin(phi) * sin(theta);
    position.y = target.y + radius * cos(phi);
    position.z = target.z + radius * sin(phi) * cos(theta);
}
```

### Zoom (Scroll Wheel)

Scroll to move closer or farther:

```cpp
void zoom(float delta) {
    Vec3 direction = normalize(position - target);
    float distance = length(position - target);
    distance = clamp(distance + delta, 1.0f, 20.0f);
    position = target + direction * distance;
}
```

## Camera Basis Vectors

The camera uses an orthonormal basis for ray generation:

```cpp
void updateBasis() {
    forward = normalize(target - position);  // Z'
    right = normalize(cross(forward, up));    // X'
    upDir = cross(right, forward);            // Y'
    
    float theta = fov * PI / 180.0f;
    viewportHeight = 2.0f * tan(theta / 2.0f);
    viewportWidth = viewportHeight * aspectRatio;
}
```

```
         upDir
           ↑
           │
           │    forward
           │   ╱
           │  ╱
           │ ╱
           ●─────────→ right
        Camera
```

## Ray Generation

For each pixel, generate a ray through it:

```cpp
Ray getRay(float u, float v) const {
    // u, v in [-1, 1]
    Vec3 horizontal = right * (u * viewportWidth * 0.5f);
    Vec3 vertical = upDir * (v * viewportHeight * 0.5f);
    Vec3 direction = forward + horizontal + vertical;
    return Ray(position, direction);
}
```

## Reset Camera

The "Reset Camera" button restores defaults:

```jsx
const handleCameraReset = () => {
    setCamera({
        position: { x: 0, y: 0.5, z: -4 },
        target: { x: 0, y: 0, z: 0 },
        fov: 60
    });
};
```

## UI Layout

The Camera tab contains:

1. **Position** section (X, Y, Z sliders)
2. **Look At Target** section (X, Y, Z sliders)
3. **Field of View** slider
4. **Distance** display
5. **Reset Camera** button
6. **Hint** about drag/scroll controls

