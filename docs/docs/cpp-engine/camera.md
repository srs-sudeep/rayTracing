---
sidebar_position: 6
---

# Camera

The `Camera` struct defines the virtual camera that views the scene. It supports position, target-based aiming, field of view, and interactive orbit controls.

## Definition

```cpp title="cpp/include/Camera.h"
struct Camera {
    Vec3 position;       // Camera location in world space
    Vec3 target;         // Point the camera looks at
    Vec3 up;             // World up direction
    float fov;           // Field of view in degrees
    float aspectRatio;   // Width / height

    // Computed basis vectors
    Vec3 forward;        // Camera forward direction
    Vec3 right;          // Camera right direction
    Vec3 upDir;          // Camera up direction
    float viewportHeight;
    float viewportWidth;
};
```

## Initialization

```cpp
Camera()
    : position(Vec3(0.0f, 0.0f, -3.0f))
    , target(Vec3(0.0f, 0.0f, 0.0f))
    , up(Vec3(0.0f, 1.0f, 0.0f))
    , fov(60.0f)
    , aspectRatio(1.0f) {
    updateBasis();
}
```

## Basis Vectors

The camera uses an orthonormal basis (three perpendicular unit vectors):

```cpp
void updateBasis() {
    // Forward: from camera toward target
    forward = (target - position).normalize();
    
    // Right: perpendicular to forward and world up
    right = forward.cross(up).normalize();
    
    // Up: perpendicular to forward and right
    upDir = right.cross(forward).normalize();

    // Viewport size based on FOV
    float theta = fov * 3.14159265f / 180.0f;
    viewportHeight = 2.0f * std::tan(theta / 2.0f);
    viewportWidth = viewportHeight * aspectRatio;
}
```

```
        upDir (Y')
           ↑
           │
           │    forward (Z')
           │   ╱
           │  ╱
           │ ╱
           ●─────────→ right (X')
        Camera
```

## Ray Generation

For each pixel, generate a ray from the camera:

```cpp
Ray getRay(float u, float v) const {
    // u, v are in range [-1, 1] (normalized screen coordinates)
    Vec3 horizontal = right * (u * viewportWidth * 0.5f);
    Vec3 vertical = upDir * (v * viewportHeight * 0.5f);
    Vec3 direction = forward + horizontal + vertical;
    return Ray(position, direction);
}
```

```
Screen Space:          World Space:
    (-1,1)──(1,1)          ╲   │   ╱
       │      │             ╲  │  ╱
       │  ●   │              ╲ │ ╱
       │      │               ╲│╱
    (-1,-1)─(1,-1)             ●  Camera
```

## Field of View

FOV controls how much of the scene is visible:

```cpp
void setFov(float newFov) {
    fov = std::fmax(10.0f, std::fmin(120.0f, newFov));
    updateBasis();
}
```

| FOV | Effect |
|-----|--------|
| 20° | Telephoto - objects appear closer, flatter |
| 60° | Normal - similar to human vision |
| 90° | Wide angle - more visible, some distortion |
| 120° | Ultra wide - extreme distortion at edges |

```
Narrow FOV (20°):        Wide FOV (120°):
      ╱╲                    ╱────────╲
     ╱  ╲                  ╱          ╲
    ╱    ╲                ╱            ╲
   ╱      ╲              ╱              ╲
  ●        ●            ●                ●
```

## Look-At Target

The camera always points at the target:

```cpp
void setTarget(float x, float y, float z) {
    target = Vec3(x, y, z);
    updateBasis();
}
```

Moving the target:
- Points camera at different parts of the scene
- Useful for focusing on specific objects
- Combined with orbit for cinematic angles

## Orbit Controls

The camera can orbit around its target:

```cpp
void orbit(float deltaX, float deltaY) {
    Vec3 offset = position - target;
    float radius = offset.length();
    
    // Convert to spherical coordinates
    float theta = std::atan2(offset.x, offset.z);  // Azimuth
    float phi = std::acos(offset.y / radius);       // Elevation
    
    // Apply rotation from mouse movement
    theta += deltaX * 0.01f;
    phi += deltaY * 0.01f;
    
    // Clamp phi to avoid gimbal lock
    phi = std::fmax(0.1f, std::fmin(3.04f, phi));
    
    // Convert back to Cartesian
    position.x = target.x + radius * std::sin(phi) * std::sin(theta);
    position.y = target.y + radius * std::cos(phi);
    position.z = target.z + radius * std::sin(phi) * std::cos(theta);
    
    updateBasis();
}
```

This is triggered by mouse drag on the canvas.

## Zoom Controls

```cpp
void zoom(float delta) {
    Vec3 direction = (position - target).normalize();
    float distance = (position - target).length();
    
    // Clamp distance
    distance = std::fmax(1.0f, std::fmin(20.0f, distance + delta));
    
    position = target + direction * distance;
    updateBasis();
}
```

Triggered by scroll wheel.

## UI Controls

The Camera tab provides:

| Control | Range | Description |
|---------|-------|-------------|
| Position X | -10 to 10 | Camera X position |
| Position Y | -10 to 10 | Camera Y position |
| Position Z | -15 to -1 | Camera Z position |
| Target X | -5 to 5 | Look-at X |
| Target Y | -5 to 5 | Look-at Y |
| Target Z | -5 to 5 | Look-at Z |
| FOV | 20° to 120° | Field of view |

Plus interactive controls:
- **Drag on canvas** - Orbit camera
- **Scroll wheel** - Zoom in/out
- **Reset button** - Return to default view

