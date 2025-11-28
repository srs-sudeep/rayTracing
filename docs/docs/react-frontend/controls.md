---
sidebar_position: 4
---

# Control Components

The control panel provides UI for adjusting all ray tracer parameters.

## ControlPanel

Container component with tabs for different control categories.

### Location

```
src/components/controls/ControlPanel.jsx
```

### Structure

```jsx
const TABS = [
  { id: 'light', label: 'Light', icon: '☀' },
  { id: 'material', label: 'Material', icon: '◆' },
  { id: 'camera', label: 'Camera', icon: '📷' },
  { id: 'view', label: 'View', icon: '⊞' },
];

function ControlPanel({ light, onLightChange, material, ... }) {
  const [activeTab, setActiveTab] = useState('light');

  return (
    <div className="control-panel">
      <Tabs tabs={TABS} activeTab={activeTab} onChange={setActiveTab} />
      <div className="control-content">
        {activeTab === 'light' && <LightControls ... />}
        {activeTab === 'material' && <MaterialControls ... />}
        {activeTab === 'camera' && <CameraControls ... />}
        {activeTab === 'view' && <ViewControls ... />}
      </div>
    </div>
  );
}
```

---

## LightControls

Controls for light position.

### Props

| Prop | Type | Description |
|------|------|-------------|
| `light` | `{x, y, z}` | Light position |
| `onChange` | Function | Update callback |
| `disabled` | boolean | Disable controls |

### Sliders

| Slider | Range | Color |
|--------|-------|-------|
| X Position | -5 to 5 | Red |
| Y Position | -5 to 5 | Teal |
| Z Position | -5 to 5 | Yellow |

---

## MaterialControls

Controls for sphere material properties.

### Props

| Prop | Type | Description |
|------|------|-------------|
| `material` | Object | Material state |
| `onChange` | Function | Update callback |
| `disabled` | boolean | Disable controls |

### Material State

```typescript
interface Material {
  color: { r: number, g: number, b: number };
  specular: number;
  shininess: number;
  reflectivity: number;
}
```

### Color Selection

```jsx
const COLOR_PRESETS = [
  { name: 'Red', color: { r: 0.9, g: 0.2, b: 0.15 } },
  { name: 'Blue', color: { r: 0.2, g: 0.4, b: 0.9 } },
  { name: 'Green', color: { r: 0.2, g: 0.85, b: 0.3 } },
  { name: 'Gold', color: { r: 1.0, g: 0.84, b: 0.0 } },
  { name: 'Purple', color: { r: 0.6, g: 0.2, b: 0.9 } },
  { name: 'White', color: { r: 0.95, g: 0.95, b: 0.95 } },
];
```

### Material Presets

```jsx
const MATERIAL_PRESETS = [
  { name: 'Matte', specular: 0.1, shininess: 8, reflectivity: 0 },
  { name: 'Plastic', specular: 0.4, shininess: 32, reflectivity: 0.1 },
  { name: 'Glossy', specular: 0.6, shininess: 64, reflectivity: 0.3 },
  { name: 'Metal', specular: 0.9, shininess: 128, reflectivity: 0.7 },
  { name: 'Mirror', specular: 1.0, shininess: 256, reflectivity: 0.95 },
];
```

---

## CameraControls

Controls for camera position, target, and field of view.

### Props

| Prop | Type | Description |
|------|------|-------------|
| `camera` | Object | Camera state |
| `onChange` | Function | Update callback |
| `onReset` | Function | Reset camera callback |
| `disabled` | boolean | Disable controls |

### Camera State

```typescript
interface Camera {
  position: { x: number, y: number, z: number };
  target: { x: number, y: number, z: number };
  fov: number;
}
```

### Sections

1. **Position** - X, Y, Z sliders for camera location
2. **Look At Target** - X, Y, Z sliders for focus point
3. **Field of View** - FOV slider (20° - 120°)
4. **Actions** - Reset camera button

---

## ViewControls

Controls for rendering options.

### Props

| Prop | Type | Description |
|------|------|-------------|
| `view` | Object | View state |
| `onChange` | Function | Update callback |
| `disabled` | boolean | Disable controls |

### View State

```typescript
interface View {
  showGroundPlane: boolean;
  showGrid: boolean;
  gridScale: number;
  groundReflectivity: number;
  maxBounces: number;
  resolution: number;
}
```

### Controls

| Control | Type | Range |
|---------|------|-------|
| Ground Plane | Toggle | on/off |
| Grid Lines | Toggle | on/off |
| Grid Scale | Slider | 0.5x - 4x |
| Ground Reflect | Slider | 0 - 0.8 |
| Max Bounces | Slider | 1 - 8 |
| Resolution | Buttons | 256, 512, 768, 1024 |

---

## Shared Styles

All control components use shared styles from `ControlSection.css`:

```css
.control-section {
  padding: var(--space-md);
  display: flex;
  flex-direction: column;
  gap: var(--space-md);
}

.control-group {
  display: flex;
  flex-direction: column;
  gap: var(--space-md);
}

.control-divider {
  height: 1px;
  background: var(--border-subtle);
}

.control-info {
  display: flex;
  justify-content: space-between;
  padding: var(--space-sm);
  background: var(--bg-primary);
  border-radius: var(--radius-sm);
}
```

