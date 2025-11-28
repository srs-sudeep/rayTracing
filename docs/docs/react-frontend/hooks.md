---
sidebar_position: 2
---

# Custom Hooks

## useWasm

The `useWasm` hook handles loading the WebAssembly module asynchronously.

### Location

```
src/hooks/useWasm.js
```

### Implementation

```jsx
import { useState, useEffect } from 'react';

export function useWasm() {
  const [wasmModule, setWasmModule] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    async function loadWasm() {
      try {
        // Dynamic import of the WASM module
        const createModule = (await import('../wasm/raytracer.js')).default;
        const module = await createModule();
        setWasmModule(module);
        setLoading(false);
      } catch (err) {
        console.error('Failed to load WASM module:', err);
        setError(err.message);
        setLoading(false);
      }
    }

    loadWasm();
  }, []);

  return { wasmModule, loading, error };
}

export default useWasm;
```

### Return Value

| Property | Type | Description |
|----------|------|-------------|
| `wasmModule` | Object \| null | The loaded WASM module with all exported functions |
| `loading` | boolean | True while module is loading |
| `error` | string \| null | Error message if loading failed |

### Usage

```jsx
import useWasm from './hooks/useWasm';

function App() {
  const { wasmModule, loading, error } = useWasm();

  if (loading) {
    return <div>Loading WebAssembly...</div>;
  }

  if (error) {
    return <div>Error: {error}</div>;
  }

  // wasmModule is ready to use
  return <RaytracerCanvas wasmModule={wasmModule} />;
}
```

### WASM Module API

Once loaded, `wasmModule` provides these functions:

```typescript
interface WasmModule {
  // Rendering
  render(width: number, height: number): VectorUint8;
  
  // Scene
  loadScenePreset(preset: number): void;
  getSphereCount(): number;
  
  // Light
  updateLight(x: number, y: number, z: number): void;
  
  // Material
  updateMaterial(specular: number, shininess: number, reflectivity: number): void;
  updateSphereColor(r: number, g: number, b: number): void;
  updateGroundReflectivity(reflectivity: number): void;
  
  // Camera
  updateCamera(x: number, y: number, z: number): void;
  orbitCamera(deltaX: number, deltaY: number): void;
  zoomCamera(delta: number): void;
  setCameraFov(fov: number): void;
  setCameraTarget(x: number, y: number, z: number): void;
  getCameraX(): number;
  getCameraY(): number;
  getCameraZ(): number;
  getCameraFov(): number;
  
  // View
  setShowGrid(show: boolean): void;
  setGridScale(scale: number): void;
  setShowGroundPlane(show: boolean): void;
  setMaxReflectionDepth(depth: number): void;
}
```

### Error Handling

The hook catches loading errors gracefully:

```jsx
try {
  const createModule = (await import('../wasm/raytracer.js')).default;
  const module = await createModule();
  setWasmModule(module);
} catch (err) {
  console.error('Failed to load WASM module:', err);
  setError(err.message);
}
```

Common errors:
- **Module not found**: WASM files not built (`npm run build:wasm`)
- **CORS error**: Incorrect server headers
- **Memory error**: Buffer too large

### Why Dynamic Import?

We use dynamic import (`import()`) because:

1. **Vite compatibility**: Static imports of WASM don't work with Vite's dev server
2. **Async loading**: WASM modules are large and should load asynchronously
3. **Error boundaries**: Easier to catch and handle loading errors

