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
│  │  │  • scene preset      • render time                      │   │  │
│  │  └───────────────────────────┬────────────────────────────┘   │  │
│  └──────────────────────────────┼────────────────────────────────┘  │
│                                 │                                   │
│                                 ▼                                   │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │                   WebAssembly Bridge                          │  │
│  │  ┌─────────────────────────────────────────────────────────┐  │  │
│  │  │  Emscripten Bindings (embind)                           │  │  │
│  │  │  • render()           • updateLight()                    │  │  │
│  │  │  • updateMaterial()   • updateCamera()                   │  │  │
│  │  │  • loadScenePreset()  • orbitCamera()                    │  │  │
│  │  └─────────────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────┬───────────────────────────────┘  │
│                                 │                                   │
│                                 ▼                                   │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │                    C++ Ray Tracing Engine                     │  │
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────────────┐  │  │
│  │  │  Vec3   │  │   Ray   │  │ Camera  │  │    Renderer     │  │  │
│  │  └─────────┘  └─────────┘  └─────────┘  └─────────────────┘  │  │
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────────────┐  │  │
│  │  │ Material│  │  Light  │  │ Sphere  │  │      Scene      │  │  │
│  │  └─────────┘  └─────────┘  └─────────┘  └─────────────────┘  │  │
│  │  ┌─────────────────────────────────────────────────────────┐  │  │
│  │  │                      Plane (Ground)                      │  │  │
│  │  └─────────────────────────────────────────────────────────┘  │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                    │
└────────────────────────────────────────────────────────────────────┘
```

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
  
  // 2. Render and get pixel buffer
  const pixelVector = wasmModule.render(width, height);
  
  // 3. Copy to JavaScript array
  const pixelData = new Uint8ClampedArray(pixelVector.size());
  for (let i = 0; i < pixelVector.size(); i++) {
    pixelData[i] = pixelVector.get(i);
  }
  
  // 4. Clean up C++ memory
  pixelVector.delete();
  
  // 5. Draw to canvas
  const imageData = new ImageData(pixelData, width, height);
  ctx.putImageData(imageData, 0, 0);
};
```

## Module Dependencies

### C++ Headers

```
core.cpp
    ├── Vec3.h         (no dependencies)
    ├── Ray.h          → Vec3.h
    ├── Material.h     → Vec3.h
    ├── Light.h        → Vec3.h
    ├── Camera.h       → Vec3.h, Ray.h
    ├── Sphere.h       → Vec3.h, Ray.h, Material.h
    ├── Plane.h        → Vec3.h, Ray.h, Material.h, Sphere.h
    ├── Scene.h        → All above
    └── Renderer.h     → Scene.h
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
            └── controls/ViewControls.jsx
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
    // ... ray tracing logic ...
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
| Reflections | O(bounces) | Limit to 3-5 bounces |
| Sphere Count | O(n) per ray | Use spatial acceleration for many objects |
| Shadow Rays | 2x rays | Consider soft shadows sampling |

