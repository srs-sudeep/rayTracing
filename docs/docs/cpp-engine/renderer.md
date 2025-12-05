---
sidebar_position: 10
---

# Renderer

The `Renderer` class is responsible for the main render loop, iterating over all pixels and producing the final image. It includes support for **anti-aliasing** through supersampling.

## Definition

```cpp title="cpp/include/Renderer.h"
enum class AALevel {
    NONE = 0,     // 1 sample per pixel
    AA_2X = 1,    // 2×2 = 4 samples per pixel
    AA_4X = 2     // 4×4 = 16 samples per pixel
};

class Renderer {
public:
    int width;              // Render width in pixels
    int height;             // Render height in pixels
    AALevel antiAliasing;   // Anti-aliasing level
    
    Renderer() : width(512), height(512), antiAliasing(AALevel::NONE) {}

    void setAntiAliasing(int level);
    int getAntiAliasing() const;
    int getSamplesPerPixel() const;
    
    std::vector<uint8_t> render(Scene& scene);
};
```

## Render Loop

### Without Anti-Aliasing

```cpp
for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
        // Convert pixel to normalized coordinates [-1, 1]
        float u = (2.0f * x / width - 1.0f);
        float v = (1.0f - 2.0f * y / height);

        // Generate ray through pixel center
        Ray ray = scene.camera.getRay(u, v);
        
        // Trace ray and get color
        Vec3 color = scene.traceRay(ray, 0);

        // Write to buffer
        buffer[index] = color;
    }
}
```

### With Anti-Aliasing (Supersampling)

```cpp
int gridSize = getSampleGridSize();  // 2 for 2×2, 4 for 4×4
float subpixelSize = 1.0f / gridSize;
float invSamples = 1.0f / (gridSize * gridSize);

for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
        Vec3 colorAccum(0, 0, 0);
        
        // Stratified sampling with jitter
        for (int sy = 0; sy < gridSize; ++sy) {
            for (int sx = 0; sx < gridSize; ++sx) {
                // Random jitter within subpixel
                float jitterX = random();  // 0 to 1
                float jitterY = random();
                
                // Subpixel position
                float subX = (sx + jitterX) * subpixelSize;
                float subY = (sy + jitterY) * subpixelSize;
                
                // Convert to NDC
                float u = (2.0f * (x + subX) / width - 1.0f);
                float v = (1.0f - 2.0f * (y + subY) / height);
                
                Ray ray = scene.camera.getRay(u, v);
                colorAccum = colorAccum + scene.traceRay(ray, 0);
            }
        }
        
        // Average all samples
        buffer[index] = (colorAccum * invSamples).clamp();
    }
}
```

## Anti-Aliasing Explained

### The Aliasing Problem

Without AA, each pixel samples only the center, causing jagged edges:

```
Actual edge:          Sampled (no AA):
                     
    ╱                     ████
   ╱                      ████
  ╱                     ████
 ╱                      ████
                      
Smooth diagonal       Stair-stepped (jaggies)
```

### Supersampling Solution

Sample multiple points per pixel and average:

```
2×2 Supersampling:         4×4 Supersampling:

  ┌───┬───┐                 ┌─┬─┬─┬─┐
  │ ● │ ● │                 │●│●│●│●│
  ├───┼───┤                 ├─┼─┼─┼─┤
  │ ● │ ● │                 │●│●│●│●│
  └───┴───┘                 ├─┼─┼─┼─┤
                            │●│●│●│●│
  4 samples                 ├─┼─┼─┼─┤
                            │●│●│●│●│
                            └─┴─┴─┴─┘
                            
                            16 samples
```

### Jittered Sampling

Regular grid sampling can cause patterns. Adding random jitter breaks these up:

```cpp
// Without jitter: visible grid patterns
float subX = (sx + 0.5f) * subpixelSize;

// With jitter: smooth results
float jitterX = dist(rng);  // Random 0-1
float subX = (sx + jitterX) * subpixelSize;
```

## Anti-Aliasing Levels

| Level | Grid | Samples/Pixel | Quality | Performance |
|-------|------|---------------|---------|-------------|
| Off | 1×1 | 1 | Baseline | 1× |
| 2×2 | 2×2 | 4 | Good | ~4× slower |
| 4×4 | 4×4 | 16 | Excellent | ~16× slower |

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

### Setting Anti-Aliasing

```javascript
// 0 = Off, 1 = 2×2, 2 = 4×4
wasmModule.setAntiAliasing(1);
```

### Calling render()

```javascript
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
| 256×256 | 65,536 | 1× |
| 512×512 | 262,144 | 4× |
| 768×768 | 589,824 | 9× |
| 1024×1024 | 1,048,576 | 16× |

Time scales quadratically with resolution.

### Combined Impact (Resolution × AA)

| Resolution | No AA | 2×2 AA | 4×4 AA |
|------------|-------|--------|--------|
| 256 | 1× | 4× | 16× |
| 512 | 4× | 16× | 64× |
| 768 | 9× | 36× | 144× |

**Tip:** Use lower resolution with 4×4 AA for smooth results without excessive render time.

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

## API Reference

```typescript
// Set anti-aliasing level (0=Off, 1=2×2, 2=4×4)
setAntiAliasing(level: number): void

// Get current AA level
getAntiAliasing(): number

// Get samples per pixel for current AA level
getSamplesPerPixel(): number
```

## Complete Flow

```
JavaScript:                    C++ (WASM):
                              
render(512, 512) ─────────────► Renderer::render()
                                    │
                                    ├── for y in 0..height
                                    │   └── for x in 0..width
                                    │       ├── for sy in 0..gridSize (AA)
                                    │       │   └── for sx in 0..gridSize
                                    │       │       ├── getRay(u, v)
                                    │       │       └── traceRay() → accumulate
                                    │       ├── average samples
                                    │       └── buffer[index] = color
                                    │
pixelVector ◄─────────────────────── return buffer
    │
    ├── Copy to Uint8ClampedArray
    ├── pixelVector.delete()
    │
    └── ctx.putImageData()
```
