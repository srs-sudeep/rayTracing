---
sidebar_position: 3
---

# Architecture Overview

RayTracer Studio follows a clean separation between the rendering engine (C++) and the user interface (React).

## System Architecture

```
┌────────────────────────────────────────────────────────────────────┐
│                          RayTracer Studio                          │
├────────────────────────────────────────────────────────────────────┤
│                                                                    │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │                     React Application                         │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐   │  │
│  │  │   Header    │  │  Controls   │  │   Canvas Component  │   │  │
│  │  │  (Stats)    │  │  (Panels)   │  │   (Render Target)   │   │  │
│  │  └─────────────┘  └──────┬──────┘  └──────────┬──────────┘   │  │
│  │                          │                     │              │  │
│  │                          ▼                     ▼              │  │
│  │  ┌────────────────────────────────────────────────────────┐   │  │
│  │  │                   App State (React)                     │   │  │
│  │  │  • light position    • material properties              │   │  │
│  │  │  • camera state      • view settings                    │   │  │
│  │  │  • scene preset      • antiAliasing, shadowSamples      │   │  │
│  │  └───────────────────────────┬────────────────────────────┘   │  │
│  └──────────────────────────────┼────────────────────────────────┘  │
│                                 │                                   │
│                                 ▼                                   │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │                   WebAssembly Bridge                          │  │
│  │  ┌─────────────────────────────────────────────────────────┐  │  │
│  │  │  Emscripten Bindings (embind)                           │  │  │
│  │  │  • render()             • setAntiAliasing()             │  │  │
│  │  │  • updateLight()        • setShadowSamples()            │  │  │
│  │  │  • updateMaterial()     • setLightRadius()              │  │  │
│  │  │  • updateCamera()       • orbitCamera()                 │  │  │
│  │  └─────────────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────┬───────────────────────────────┘  │
│                                 │                                   │
│                                 ▼                                   │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │                    C++ Ray Tracing Engine                     │  │
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────────────┐  │  │
│  │  │  Vec3   │  │   Ray   │  │ Camera  │  │    Renderer     │  │  │
│  │  │         │  │         │  │         │  │  (Anti-Alias)   │  │  │
│  │  └─────────┘  └─────────┘  └─────────┘  └─────────────────┘  │  │
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────────────┐  │  │
│  │  │ Material│  │  Light  │  │ Sphere  │  │      Scene      │  │  │
│  │  │         │  │ (Area)  │  │         │  │  (Soft Shadow)  │  │  │
│  │  └─────────┘  └─────────┘  └─────────┘  └─────────────────┘  │  │
│  │  ┌─────────────────────────────────────────────────────────┐  │  │
│  │  │                      Plane (Ground)                      │  │  │
│  │  └─────────────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                    │
└────────────────────────────────────────────────────────────────────┘
```

## Key Features

| Feature | Location | Description |
|---------|----------|-------------|
| **Anti-Aliasing** | `Renderer.h` | Supersampling with jittered sampling (1, 4, or 16 samples/pixel) |
| **Soft Shadows** | `Scene.h`, `Light.h` | Area lights with stratified shadow ray sampling |
| **Reflections** | `Scene.h` | Recursive ray tracing with Fresnel effect |
| **Blinn-Phong** | `Scene.h` | Local illumination with diffuse + specular |

## Data Flow

### Render Loop

1. **User Interaction** → React state updates
2. **State Change** → Triggers `useEffect` in Canvas component
3. **WASM Calls** → Update C++ engine state via bindings
4. **Render** → C++ engine traces rays and produces pixel buffer
5. **Display** → Pixel buffer copied to Canvas via `ImageData`

```jsx
// Simplified render flow
const renderFrame = () => {
  // 1. Update C++ state
  wasmModule.updateLight(light.x, light.y, light.z);
  wasmModule.updateMaterial(specular, shininess, reflectivity);
  wasmModule.updateCamera(camera.x, camera.y, camera.z);
  
  // 2. Set rendering options
  wasmModule.setAntiAliasing(view.antiAliasing);
  wasmModule.setShadowSamples(view.shadowSamples);
  
  // 3. Render and get pixel buffer
  const pixelVector = wasmModule.render(width, height);
  
  // 4. Copy to JavaScript array
  const pixelData = new Uint8ClampedArray(pixelVector.size());
  for (let i = 0; i < pixelVector.size(); i++) {
    pixelData[i] = pixelVector.get(i);
  }
  
  // 5. Clean up C++ memory
  pixelVector.delete();
  
  // 6. Draw to canvas
  const imageData = new ImageData(pixelData, width, height);
  ctx.putImageData(imageData, 0, 0);
};
```

## Rendering Pipeline

```
┌─────────────────────────────────────────────────────────────────────┐
│                        Rendering Pipeline                            │
├─────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  For each pixel (x, y):                                              │
│  ┌────────────────────────────────────────────────────────────────┐  │
│  │                    Anti-Aliasing Loop                          │  │
│  │  ┌──────────────────────────────────────────────────────────┐  │  │
│  │  │  For each sample (with jitter):                          │  │  │
│  │  │                                                          │  │  │
│  │  │    1. Generate Camera Ray                                │  │  │
│  │  │       └── getRay(u + jitter, v + jitter)                │  │  │
│  │  │                                                          │  │  │
│  │  │    2. Trace Ray (recursive)                              │  │  │
│  │  │       ├── Find intersection (spheres, plane)            │  │  │
│  │  │       ├── If hit:                                       │  │  │
│  │  │       │   ├── Calculate lighting                        │  │  │
│  │  │       │   │   └── For each light:                       │  │  │
│  │  │       │   │       ├── Shadow test (soft or hard)        │  │  │
│  │  │       │   │       └── Blinn-Phong shading               │  │  │
│  │  │       │   └── Trace reflection (if reflective)          │  │  │
│  │  │       └── Else: return background color                 │  │  │
│  │  │                                                          │  │  │
│  │  │    3. Accumulate sample color                            │  │  │
│  │  └──────────────────────────────────────────────────────────┘  │  │
│  │                                                                │  │
│  │  Average accumulated samples → Final pixel color               │  │
│  └────────────────────────────────────────────────────────────────┘  │
│                                                                      │
└─────────────────────────────────────────────────────────────────────┘
```

## Module Dependencies

### C++ Headers

```
core.cpp
    ├── Vec3.h         (no dependencies)
    ├── Ray.h          → Vec3.h
    ├── Material.h     → Vec3.h
    ├── Light.h        → Vec3.h (includes radius for area lights)
    ├── Camera.h       → Vec3.h, Ray.h
    ├── Sphere.h       → Vec3.h, Ray.h, Material.h
    ├── Plane.h        → Vec3.h, Ray.h, Material.h, Sphere.h
    ├── Scene.h        → All above (includes soft shadow logic)
    └── Renderer.h     → Scene.h (includes anti-aliasing logic)
```

### React Components

```
App.jsx
    ├── hooks/useWasm.js
    ├── layout/Header.jsx
    │       └── layout/InfoModal.jsx
    ├── canvas/SceneToolbar.jsx
    ├── canvas/RaytracerCanvas.jsx
    └── controls/ControlPanel.jsx
            ├── controls/LightControls.jsx
            ├── controls/MaterialControls.jsx
            ├── controls/CameraControls.jsx
            └── controls/ViewControls.jsx  (AA, soft shadows)
                    ├── ui/Tabs.jsx
                    ├── ui/Slider.jsx
                    └── ui/Toggle.jsx
```

## Memory Management

### C++ to JavaScript Data Transfer

The render function returns a `std::vector<uint8_t>` which Emscripten wraps as a JavaScript object:

```cpp
std::vector<uint8_t> render(int width, int height) {
    std::vector<uint8_t> buffer(width * height * 4);  // RGBA
    // ... ray tracing logic with AA ...
    return buffer;
}
```

:::warning Important
Always call `.delete()` on the returned vector to prevent memory leaks:

```javascript
const pixelVector = wasmModule.render(512, 512);
// ... use the data ...
pixelVector.delete();  // Free C++ memory!
```
:::

## Performance Considerations

| Factor | Impact | Optimization |
|--------|--------|--------------|
| Resolution | O(n²) | Use 256-512 for interaction |
| Anti-Aliasing | ×4 or ×16 | Use 2×2 for interaction, 4×4 for final |
| Soft Shadows | ×samples per light | 4-9 samples for preview, 16-25 for final |
| Reflections | O(bounces) | Limit to 3-5 bounces |
| Sphere Count | O(n) per ray | Use spatial acceleration for many objects |

### Combined Render Cost

```
Base cost = Resolution² × Reflections

With features:
Total = Base × AA_samples × Shadow_samples × Lights

Example (worst case):
512² × 5 bounces × 16 AA × 25 shadows × 2 lights
= 262,144 × 5 × 16 × 25 × 2
= ~1 billion ray operations
```

**Recommendation:** Enable features progressively. Use low settings during interaction, high settings for final render.

## Responsive Design

The UI adapts to different screen sizes:

| Breakpoint | Layout |
|------------|--------|
| > 900px | Side-by-side (canvas + sidebar) |
| ≤ 900px | Stacked (canvas above controls) |
| ≤ 640px | Compact tabs, icons only |
| Mobile | Touch support for camera orbit |
