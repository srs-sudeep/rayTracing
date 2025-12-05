---
sidebar_position: 6
---

# Anti-Aliasing

Anti-aliasing removes the jagged "stair-step" edges (aliasing) that appear on diagonal lines and curves. RayTracer Studio implements **supersampling** with **jittered sampling**.

## The Problem: Aliasing

Without anti-aliasing, each pixel is a single sample at its center. This causes visible "jaggies":

```
Actual curve:              Single sample per pixel:

       ╭────╮                    ████
      ╱      ╲                 ████
     ╱        ╲              ████       Stair-stepped!
    ╱          ╲           ████
   ╱            ╲        ████
```

## The Solution: Supersampling

Instead of one sample, we take multiple samples within each pixel and average them:

```
1 sample (no AA):          4 samples (2×2 AA):         16 samples (4×4 AA):

    ┌─────────┐              ┌────┬────┐                ┌─┬─┬─┬─┐
    │         │              │ ●  │  ● │                │●│●│●│●│
    │    ●    │              ├────┼────┤                ├─┼─┼─┼─┤
    │         │              │ ●  │  ● │                │●│●│●│●│
    └─────────┘              └────┴────┘                ├─┼─┼─┼─┤
                                                        │●│●│●│●│
                                                        ├─┼─┼─┼─┤
                                                        │●│●│●│●│
                                                        └─┴─┴─┴─┘
```

## How It Works

### Basic Supersampling

```cpp
Vec3 colorAccum(0, 0, 0);
int gridSize = 2;  // 2×2 = 4 samples

for (int sy = 0; sy < gridSize; ++sy) {
    for (int sx = 0; sx < gridSize; ++sx) {
        // Subpixel position (0.25 and 0.75 for 2×2)
        float subX = (sx + 0.5f) / gridSize;
        float subY = (sy + 0.5f) / gridSize;
        
        // Convert to NDC
        float u = (2.0f * (x + subX) / width - 1.0f);
        float v = (1.0f - 2.0f * (y + subY) / height);
        
        Ray ray = camera.getRay(u, v);
        colorAccum = colorAccum + scene.traceRay(ray, 0);
    }
}

// Average all samples
Vec3 finalColor = colorAccum * (1.0f / (gridSize * gridSize));
```

### Jittered Sampling

Regular grid sampling can create visible patterns. Adding randomness (jitter) breaks these up:

```cpp
// Without jitter (visible patterns on gradients)
float subX = (sx + 0.5f) / gridSize;

// With jitter (smooth results)
float jitter = randomFloat();  // 0 to 1
float subX = (sx + jitter) / gridSize;
```

```
Regular Grid:              Jittered Grid:

  ●   ●   ●   ●              ●    ●   ●    ●
                               ↘   ↙   ↘   ↙
  ●   ●   ●   ●              ●   ●    ●   ●
                               ↗   ↖   ↗   ↖
  ●   ●   ●   ●            ●   ●    ●    ●
                             ↙   ↗    ↖   ↘
  ●   ●   ●   ●              ●   ●   ●    ●

Can show patterns         No visible patterns
```

### Stratified Sampling

We combine the best of both: divide the pixel into a grid (stratified) but randomize within each cell (jittered):

```cpp
for (int sy = 0; sy < gridSize; ++sy) {
    for (int sx = 0; sx < gridSize; ++sx) {
        // Random offset within this cell
        float jitterX = randomFloat();  // 0 to 1
        float jitterY = randomFloat();
        
        // Position: cell + jitter, scaled to pixel
        float subX = (sx + jitterX) / gridSize;
        float subY = (sy + jitterY) / gridSize;
        
        // ... trace ray ...
    }
}
```

## AA Levels

RayTracer Studio offers three levels:

| Level | Grid | Samples | Quality | Render Time |
|-------|------|---------|---------|-------------|
| Off | 1×1 | 1 | Baseline | 1× |
| 2×2 | 2×2 | 4 | Good | ~4× |
| 4×4 | 4×4 | 16 | Excellent | ~16× |

## Visual Comparison

```
No AA (aliased):           2×2 AA:                  4×4 AA:

    ████████                  ▓▓██████                ░▓██████
    ████████                ▓▓████████              ░▓████████
  ████████                ▓▓████████              ░▓████████
  ████████              ▓▓████████              ░▓████████
████████              ▓▓████████              ░▓████████
████████            ▓▓████████              ░▓████████

Jagged edges           Smoother                 Very smooth
```

## Usage in RayTracer Studio

### UI Controls

In the **View** tab:
- **Anti-Aliasing** button group: Off, 2×2, 4×4
- Shows samples per pixel count
- Warning indicator for performance impact

### JavaScript API

```javascript
// Set AA level (0 = Off, 1 = 2×2, 2 = 4×4)
wasmModule.setAntiAliasing(1);

// Get current level
const level = wasmModule.getAntiAliasing();

// Get samples per pixel
const samples = wasmModule.getSamplesPerPixel();  // 1, 4, or 16
```

### C++ Implementation

```cpp title="cpp/include/Renderer.h"
enum class AALevel {
    NONE = 0,     // 1 sample per pixel
    AA_2X = 1,    // 2×2 = 4 samples
    AA_4X = 2     // 4×4 = 16 samples
};

class Renderer {
    AALevel antiAliasing = AALevel::NONE;
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist{0.0f, 1.0f};
    
    // ...
};
```

## Performance Tips

### Resolution vs AA Trade-off

Sometimes lower resolution + higher AA looks better than higher resolution + no AA:

| Setting | Total Samples | Quality |
|---------|---------------|---------|
| 512×512, no AA | 262,144 | Jagged |
| 256×256, 4×4 AA | 1,048,576 | Very smooth |
| 512×512, 2×2 AA | 1,048,576 | Good balance |

### Combining with Other Features

AA multiplies render time. When using expensive features:

```
Base render time: 100ms

+ 2×2 AA:         400ms (×4)
+ Soft Shadows:   900ms (×9 with 9 samples)
+ Both:           3600ms (×36)
```

**Recommendation:** Use 2×2 AA for interactive work, 4×4 for final renders.

## When to Use AA

| Scenario | Recommendation |
|----------|----------------|
| Interactive editing | Off or 2×2 |
| Final render | 4×4 |
| Mobile devices | 2×2 max |
| With soft shadows | 2×2 or Off |
| High resolution (1024+) | Off or 2×2 |

## Alternative: Temporal AA

A future enhancement could implement temporal anti-aliasing:
- Accumulate frames over time
- Jitter camera slightly each frame
- Running average converges to smooth result
- Allows interactive framerates with high quality

