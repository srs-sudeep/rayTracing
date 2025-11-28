---
sidebar_position: 1
---

# React Frontend Overview

The React frontend provides an interactive UI for controlling the ray tracer and displaying rendered images.

## Component Architecture

```
src/
├── components/
│   ├── canvas/
│   │   ├── RaytracerCanvas.jsx  # Main render canvas
│   │   ├── RaytracerCanvas.css
│   │   ├── SceneToolbar.jsx     # Scene preset selector
│   │   └── SceneToolbar.css
│   ├── controls/
│   │   ├── ControlPanel.jsx     # Tabbed control container
│   │   ├── ControlPanel.css
│   │   ├── LightControls.jsx    # Light position controls
│   │   ├── MaterialControls.jsx # Material property controls
│   │   ├── CameraControls.jsx   # Camera position/FOV controls
│   │   ├── ViewControls.jsx     # Grid and resolution controls
│   │   └── ControlSection.css   # Shared control styles
│   ├── layout/
│   │   ├── Header.jsx           # App header with stats
│   │   ├── Header.css
│   │   ├── InfoModal.jsx        # About dialog
│   │   └── InfoModal.css
│   └── ui/
│       ├── Tabs.jsx             # Reusable tab component
│       ├── Tabs.css
│       ├── Slider.jsx           # Reusable slider component
│       ├── Slider.css
│       ├── Toggle.jsx           # Reusable toggle component
│       └── Toggle.css
├── hooks/
│   └── useWasm.js               # WebAssembly loading hook
├── App.jsx                      # Main application
├── App.css                      # Global styles
└── main.jsx                     # Entry point
```

## State Management

The app uses React's `useState` for all state management:

```jsx
// App.jsx
const [scenePreset, setScenePreset] = useState(0);
const [sphereCount, setSphereCount] = useState(1);
const [light, setLight] = useState({ x: 2, y: 3, z: -2 });
const [material, setMaterial] = useState({ 
  color: { r: 0.9, g: 0.2, b: 0.15 },
  specular: 0.5, 
  shininess: 32, 
  reflectivity: 0.3 
});
const [camera, setCamera] = useState({ 
  position: { x: 0, y: 0.5, z: -4 },
  target: { x: 0, y: 0, z: 0 },
  fov: 60
});
const [view, setView] = useState({ 
  showGroundPlane: true,
  showGrid: true, 
  gridScale: 1.0,
  groundReflectivity: 0.15,
  maxBounces: 5,
  resolution: 512 
});
```

## Data Flow

```
┌─────────────────────────────────────────────────────────────┐
│                         App.jsx                              │
│  ┌─────────────────────────────────────────────────────┐    │
│  │  State: light, material, camera, view, scenePreset   │    │
│  └───────────────────────┬─────────────────────────────┘    │
│                          │                                   │
│          ┌───────────────┼───────────────┐                  │
│          ▼               ▼               ▼                  │
│  ┌───────────────┐ ┌───────────┐ ┌─────────────────┐       │
│  │ ControlPanel  │ │SceneToolbar│ │ RaytracerCanvas │       │
│  │ (onChange)    │ │(onPreset)  │ │ (renders image) │       │
│  └───────────────┘ └───────────┘ └────────┬────────┘       │
│                                           │                  │
│                                           ▼                  │
│                                    ┌──────────────┐         │
│                                    │  WASM Module │         │
│                                    │   (C++ API)  │         │
│                                    └──────────────┘         │
└─────────────────────────────────────────────────────────────┘
```

## Key Design Decisions

### 1. Prop Drilling vs Context

We use prop drilling for simplicity. The component tree is shallow enough that Context would add unnecessary complexity.

### 2. Controlled Components

All inputs are controlled - their values come from React state:

```jsx
<Slider
  value={light.x}
  onChange={(v) => setLight(prev => ({ ...prev, x: v }))}
/>
```

### 3. Debounced Rendering

Canvas renders are debounced with `requestAnimationFrame` to prevent excessive WASM calls:

```jsx
useEffect(() => {
  const frameId = requestAnimationFrame(renderFrame);
  return () => cancelAnimationFrame(frameId);
}, [renderFrame]);
```

### 4. Modular UI Components

Reusable components (`Slider`, `Toggle`, `Tabs`) are in `components/ui/` and can be used anywhere.

## Styling Approach

- **CSS Variables** for theming in `:root`
- **Component CSS files** alongside each component
- **BEM-like naming** for clarity
- **Dark theme** by default

```css
:root {
  --bg-primary: #09090b;
  --accent-primary: #e85d4c;
  --text-primary: #f4f4f5;
  /* ... */
}
```

## Next Steps

Explore each component category:

- [Hooks](./hooks) - Custom React hooks
- [Canvas](./canvas) - Render target and toolbar
- [Controls](./controls) - Control panels
- [UI Components](./ui-components) - Reusable elements

