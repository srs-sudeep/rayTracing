---
sidebar_position: 1
---

# C++ Engine Overview

The ray tracing engine is written in modern C++ and compiled to WebAssembly using Emscripten. It consists of several modular header files, each responsible for a specific aspect of the rendering pipeline.

## File Structure

```
cpp/
├── include/
│   ├── Vec3.h        # 3D vector mathematics
│   ├── Ray.h         # Ray representation
│   ├── Material.h    # Surface material properties
│   ├── Light.h       # Point light source
│   ├── Camera.h      # Virtual camera with controls
│   ├── Sphere.h      # Sphere primitive and intersection
│   ├── Plane.h       # Infinite ground plane with grid
│   ├── Scene.h       # Scene graph and ray tracing logic
│   └── Renderer.h    # Main render loop
├── core.cpp          # Emscripten bindings entry point
└── build.sh          # Compilation script
```

## Compilation

The build script (`build.sh`) compiles the C++ code to WebAssembly:

```bash
emcc core.cpp \
    -I"include" \
    -O3 \                          # Maximum optimization
    -s WASM=1 \                    # Output WebAssembly
    -s MODULARIZE=1 \              # Create ES6 module
    -s EXPORT_ES6=1 \              # ES6 export syntax
    -s ALLOW_MEMORY_GROWTH=1 \     # Dynamic memory
    -s ENVIRONMENT='web' \         # Web environment
    -s EXPORT_NAME='createRaytracerModule' \
    --bind \                       # Enable embind
    -o "src/wasm/raytracer.js"
```

## Emscripten Bindings

The `core.cpp` file exposes C++ functions to JavaScript using Emscripten's `embind`:

```cpp
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(raytracer_module) {
    // Render function
    emscripten::function("render", &render);
    
    // Light controls
    emscripten::function("updateLight", &updateLight);
    
    // Material controls
    emscripten::function("updateMaterial", &updateMaterial);
    emscripten::function("updateSphereColor", &updateSphereColor);
    
    // Camera controls
    emscripten::function("updateCamera", &updateCamera);
    emscripten::function("orbitCamera", &orbitCamera);
    emscripten::function("zoomCamera", &zoomCamera);
    emscripten::function("setCameraFov", &setCameraFov);
    emscripten::function("setCameraTarget", &setCameraTarget);
    
    // Scene controls
    emscripten::function("loadScenePreset", &loadScenePreset);
    emscripten::function("getSphereCount", &getSphereCount);
    
    // View controls
    emscripten::function("setShowGrid", &setShowGrid);
    emscripten::function("setShowGroundPlane", &setShowGroundPlane);
    emscripten::function("setMaxReflectionDepth", &setMaxReflectionDepth);
    
    // Vector type for return values
    emscripten::register_vector<uint8_t>("VectorUint8");
}
```

## Design Principles

### 1. Header-Only Architecture

Most logic is in header files for simplicity and to avoid linking issues with Emscripten:

```cpp
// Vec3.h - Complete implementation in header
struct Vec3 {
    float x, y, z;
    
    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    // ... more methods
};
```

### 2. No External Dependencies

The engine uses only standard C++ libraries (`<cmath>`, `<vector>`, `<algorithm>`) to ensure compatibility with Emscripten.

### 3. Global State Pattern

For simplicity, global instances are used for the scene and renderer:

```cpp
Scene globalScene;
Renderer globalRenderer;

std::vector<uint8_t> render(int width, int height) {
    globalRenderer.width = width;
    globalRenderer.height = height;
    return globalRenderer.render(globalScene);
}
```

## Ray Tracing Pipeline

```
For each pixel (x, y):
    1. Generate ray from camera through pixel
    2. Find closest intersection with scene objects
    3. If hit:
       a. Calculate lighting (diffuse + specular)
       b. Check for shadows
       c. If reflective: recursively trace reflected ray
       d. Blend colors based on material properties
    4. Write RGBA to buffer
```

## Next Steps

Explore each component in detail:

- [Vec3](./vec3) - Vector mathematics
- [Ray](./ray) - Ray representation
- [Material](./material) - Surface properties
- [Light](./light) - Light sources
- [Camera](./camera) - Virtual camera
- [Sphere](./sphere) - Sphere intersection
- [Plane](./plane) - Ground plane
- [Scene](./scene) - Scene management
- [Renderer](./renderer) - Render loop

