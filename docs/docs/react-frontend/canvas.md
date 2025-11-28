---
sidebar_position: 3
---

# Canvas Components

The canvas components handle rendering the ray traced image and scene selection.

## RaytracerCanvas

The main rendering component that interfaces with the WASM module.

### Location

```
src/components/canvas/RaytracerCanvas.jsx
```

### Props

| Prop | Type | Description |
|------|------|-------------|
| `wasmModule` | Object | Loaded WASM module |
| `light` | Object | Light position `{x, y, z}` |
| `material` | Object | Material properties |
| `camera` | Object | Camera state |
| `view` | Object | View settings |
| `scenePreset` | number | Active scene preset ID |
| `onCameraChange` | Function | Camera update callback |
| `onRenderTime` | Function | Render time callback |

### Core Logic

#### Render Function

```jsx
const renderFrame = useCallback(() => {
  if (!wasmModule || !canvasRef.current) return;

  const canvas = canvasRef.current;
  const ctx = canvas.getContext('2d');
  const resolution = view.resolution;

  // Update WASM state
  wasmModule.updateLight(light.x, light.y, light.z);
  wasmModule.updateMaterial(material.specular, material.shininess, material.reflectivity);
  wasmModule.updateSphereColor(material.color.r, material.color.g, material.color.b);
  wasmModule.updateCamera(camera.position.x, camera.position.y, camera.position.z);
  wasmModule.setCameraTarget(camera.target.x, camera.target.y, camera.target.z);
  wasmModule.setCameraFov(camera.fov);
  // ... more updates

  // Render and measure time
  const startTime = performance.now();
  const pixelVector = wasmModule.render(resolution, resolution);
  const endTime = performance.now();
  onRenderTime(endTime - startTime);

  // Copy to JavaScript array
  const pixelData = new Uint8ClampedArray(pixelVector.size());
  for (let i = 0; i < pixelVector.size(); i++) {
    pixelData[i] = pixelVector.get(i);
  }
  pixelVector.delete();  // Free C++ memory!

  // Draw to canvas
  const imageData = new ImageData(pixelData, resolution, resolution);
  ctx.putImageData(imageData, 0, 0);
}, [wasmModule, light, material, camera, view, scenePreset, onRenderTime]);
```

#### Debounced Rendering

```jsx
useEffect(() => {
  if (renderRequestRef.current) {
    cancelAnimationFrame(renderRequestRef.current);
  }
  renderRequestRef.current = requestAnimationFrame(renderFrame);
  
  return () => {
    if (renderRequestRef.current) {
      cancelAnimationFrame(renderRequestRef.current);
    }
  };
}, [renderFrame]);
```

#### Mouse Orbit

```jsx
const handleMouseMove = (e) => {
  if (!isDragging || !wasmModule) return;

  const deltaX = e.clientX - lastMousePos.current.x;
  const deltaY = e.clientY - lastMousePos.current.y;
  lastMousePos.current = { x: e.clientX, y: e.clientY };

  wasmModule.orbitCamera(deltaX, deltaY);
  
  onCameraChange({
    ...camera,
    position: {
      x: wasmModule.getCameraX(),
      y: wasmModule.getCameraY(),
      z: wasmModule.getCameraZ()
    }
  });
};
```

#### Scroll Zoom

```jsx
const handleWheel = (e) => {
  if (!wasmModule) return;
  e.preventDefault();
  
  const delta = e.deltaY * 0.01;
  wasmModule.zoomCamera(delta);
  
  onCameraChange({
    ...camera,
    position: {
      x: wasmModule.getCameraX(),
      y: wasmModule.getCameraY(),
      z: wasmModule.getCameraZ()
    }
  });
};
```

### Responsive Sizing

The canvas dynamically sizes to fill its container:

```jsx
useEffect(() => {
  const updateSize = () => {
    if (containerRef.current) {
      const rect = containerRef.current.getBoundingClientRect();
      const maxSize = Math.min(rect.width - 20, rect.height - 20);
      const size = Math.max(300, maxSize);
      setDisplaySize({ width: size, height: size });
    }
  };

  updateSize();
  window.addEventListener('resize', updateSize);
  return () => window.removeEventListener('resize', updateSize);
}, []);
```

---

## SceneToolbar

The toolbar above the canvas for quick scene preset selection.

### Location

```
src/components/canvas/SceneToolbar.jsx
```

### Props

| Prop | Type | Description |
|------|------|-------------|
| `activePreset` | number | Currently selected preset |
| `onPresetChange` | Function | Callback when preset changes |
| `sphereCount` | number | Number of spheres in scene |
| `disabled` | boolean | Whether controls are disabled |

### Preset Definitions

```jsx
const SCENE_PRESETS = [
  { id: 0, name: 'Single', icon: '🔴' },
  { id: 1, name: 'Three', icon: '🎱' },
  { id: 2, name: 'Mirror', icon: '🪞' },
  { id: 3, name: 'Rainbow', icon: '🌈' },
];
```

### Rendering

```jsx
function SceneToolbar({ activePreset, onPresetChange, sphereCount, disabled }) {
  return (
    <div className="scene-toolbar">
      <div className="toolbar-section">
        <span className="toolbar-label">Scene</span>
        <div className="preset-buttons">
          {SCENE_PRESETS.map((preset) => (
            <button
              key={preset.id}
              className={`preset-btn ${activePreset === preset.id ? 'active' : ''}`}
              onClick={() => onPresetChange(preset.id)}
              disabled={disabled}
            >
              <span className="preset-icon">{preset.icon}</span>
              <span className="preset-name">{preset.name}</span>
            </button>
          ))}
        </div>
      </div>
      
      <div className="toolbar-info">
        <span className="info-chip">
          {sphereCount} sphere{sphereCount > 1 ? 's' : ''}
        </span>
      </div>
    </div>
  );
}
```

### Styling

```css
.scene-toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: var(--space-sm) var(--space-md);
  background: var(--bg-secondary);
  border: 1px solid var(--border-subtle);
  border-radius: var(--radius-lg);
}

.preset-btn.active {
  background: var(--accent-primary);
  color: white;
}
```

