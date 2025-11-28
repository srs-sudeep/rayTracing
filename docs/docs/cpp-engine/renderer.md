---
sidebar_position: 10
---

# Renderer

The `Renderer` class is responsible for the main render loop, iterating over all pixels and producing the final image.

## Definition

```cpp title="cpp/include/Renderer.h"
class Renderer {
public:
    int width;   // Render width in pixels
    int height;  // Render height in pixels

    Renderer() : width(512), height(512) {}

    std::vector<uint8_t> render(Scene& scene);
};
```

## Render Loop

```cpp
std::vector<uint8_t> render(Scene& scene) {
    // Allocate RGBA buffer
    std::vector<uint8_t> buffer(width * height * 4);
    
    // Set camera aspect ratio
    scene.camera.setAspectRatio(static_cast<float>(width) / height);

    // Iterate over every pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Convert pixel to normalized coordinates [-1, 1]
            float u = (2.0f * x / width - 1.0f);
            float v = (1.0f - 2.0f * y / height);

            // Generate ray through this pixel
            Ray ray = scene.camera.getRay(u, v);
            
            // Trace ray and get color
            Vec3 color = scene.traceRay(ray, 0);

            // Write to buffer (RGBA format)
            int index = (y * width + x) * 4;
            buffer[index + 0] = static_cast<uint8_t>(color.x * 255.0f);  // R
            buffer[index + 1] = static_cast<uint8_t>(color.y * 255.0f);  // G
            buffer[index + 2] = static_cast<uint8_t>(color.z * 255.0f);  // B
            buffer[index + 3] = 255;  // A (fully opaque)
        }
    }

    return buffer;
}
```

## Coordinate Systems

### Pixel Coordinates
- `x`: 0 to width-1 (left to right)
- `y`: 0 to height-1 (top to bottom)

### Normalized Device Coordinates (NDC)
- `u`: -1 to 1 (left to right)
- `v`: -1 to 1 (bottom to top)

```cpp
float u = (2.0f * x / width - 1.0f);   // Map [0, width] to [-1, 1]
float v = (1.0f - 2.0f * y / height);  // Map [0, height] to [1, -1]
```

Note: `v` is flipped because screen Y increases downward, but world Y increases upward.

## Buffer Format

The output is a flat array of bytes in RGBA order:

```
Buffer layout:
[R₀, G₀, B₀, A₀, R₁, G₁, B₁, A₁, R₂, ...]

Pixel at (x, y):
index = (y * width + x) * 4
R = buffer[index + 0]
G = buffer[index + 1]
B = buffer[index + 2]
A = buffer[index + 3]
```

## JavaScript Integration

### Calling render()

```javascript
// From RaytracerCanvas.jsx
const pixelVector = wasmModule.render(resolution, resolution);
```

### Copying to JavaScript Array

```javascript
const size = pixelVector.size();
const pixelData = new Uint8ClampedArray(size);

for (let i = 0; i < size; i++) {
    pixelData[i] = pixelVector.get(i);
}
```

### Memory Cleanup

```javascript
// IMPORTANT: Free C++ memory to prevent leaks
pixelVector.delete();
```

### Drawing to Canvas

```javascript
const imageData = new ImageData(pixelData, resolution, resolution);
ctx.putImageData(imageData, 0, 0);
```

## Performance Considerations

### Resolution Impact

| Resolution | Pixels | Relative Time |
|------------|--------|---------------|
| 256×256 | 65,536 | 1x |
| 512×512 | 262,144 | 4x |
| 768×768 | 589,824 | 9x |
| 1024×1024 | 1,048,576 | 16x |

Time scales quadratically with resolution.

### Optimization Flags

The build script uses `-O3` for maximum optimization:

```bash
emcc core.cpp -O3 ...
```

This enables:
- Loop unrolling
- Function inlining
- SIMD vectorization (where possible)
- Dead code elimination

### Future Optimizations

Potential improvements not yet implemented:

1. **Multi-threading**: Use Web Workers to parallelize
2. **Tile-based rendering**: Render in chunks for progress feedback
3. **Adaptive sampling**: More samples for complex areas
4. **BVH acceleration**: Spatial data structure for many objects

## Complete Flow

```
JavaScript:                    C++ (WASM):
                              
render(512, 512) ─────────────► Renderer::render()
                                    │
                                    ├── for y in 0..height
                                    │   └── for x in 0..width
                                    │       ├── getRay(u, v)
                                    │       ├── traceRay()
                                    │       │   ├── trace() [intersection]
                                    │       │   ├── shade() [lighting]
                                    │       │   └── reflect [recursion]
                                    │       └── buffer[index] = color
                                    │
pixelVector ◄─────────────────────── return buffer
    │
    ├── Copy to Uint8ClampedArray
    ├── pixelVector.delete()
    │
    └── ctx.putImageData()
```

