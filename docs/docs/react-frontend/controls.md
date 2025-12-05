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

Controls for multiple colored lights.

### Props

| Prop | Type | Description |
|------|------|-------------|
| `lights` | `Array<Light>` | Array of light objects |
| `onChange` | Function | Update all lights callback |
| `onAddLight` | Function | Add new light callback |
| `onRemoveLight` | Function | Remove light callback |
| `disabled` | boolean | Disable controls |

### Light Object

```typescript
interface Light {
  x: number;
  y: number;
  z: number;
  color: { r: number, g: number, b: number };
  intensity: number;
}
```

### Features

- **Light Selector**: Tab buttons for each light (up to 4)
- **Add/Remove**: + and − buttons
- **Color Presets**: White, Warm, Cool, Red, Green, Blue
- **Intensity Slider**: 0% to 200%
- **Position Sliders**: X, Y, Z coordinates

### Color Presets

```jsx
const LIGHT_COLORS = [
  { name: 'White', color: { r: 1.0, g: 1.0, b: 1.0 } },
  { name: 'Warm', color: { r: 1.0, g: 0.9, b: 0.7 } },
  { name: 'Cool', color: { r: 0.8, g: 0.9, b: 1.0 } },
  { name: 'Red', color: { r: 1.0, g: 0.3, b: 0.3 } },
  { name: 'Green', color: { r: 0.3, g: 1.0, b: 0.4 } },
  { name: 'Blue', color: { r: 0.3, g: 0.5, b: 1.0 } },
];
```

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

Controls for rendering options including anti-aliasing and soft shadows.

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
  antiAliasing: number;      // 0=Off, 1=2×2, 2=4×4
  softShadows: boolean;      // Enable soft shadows
  shadowSamples: number;     // 4, 9, 16, or 25
  lightRadius: number;       // Area light size (0.1 - 1.5)
}
```

### Controls

| Control | Type | Range | Description |
|---------|------|-------|-------------|
| Ground Plane | Toggle | on/off | Show ground |
| Grid Lines | Toggle | on/off | Show grid |
| Grid Scale | Slider | 0.5x - 4x | Grid cell size |
| Ground Reflect | Slider | 0 - 0.8 | Ground reflectivity |
| Max Bounces | Slider | 1 - 8 | Reflection depth |
| **Soft Shadows** | Toggle | on/off | Enable area lights |
| **Light Size** | Slider | 0.1 - 1.5 | Shadow softness |
| **Shadow Samples** | Buttons | 4, 9, 16, 25 | Shadow quality |
| **Anti-Aliasing** | Buttons | Off, 2×2, 4×4 | Edge smoothing |
| Resolution | Buttons | 256, 512, 768, 1024 | Render size |

### Anti-Aliasing Options

```jsx
const AA_OPTIONS = [
  { value: 0, label: 'Off', samples: 1 },
  { value: 1, label: '2×2', samples: 4 },
  { value: 2, label: '4×4', samples: 16 },
];
```

### Shadow Sample Options

```jsx
const SHADOW_SAMPLE_OPTIONS = [
  { value: 4, label: '4' },
  { value: 9, label: '9' },
  { value: 16, label: '16' },
  { value: 25, label: '25' },
];
```

### Performance Notes

The View tab shows performance indicators:
- AA level displays samples per pixel
- Soft shadows show rays per light
- Warnings displayed when expensive options are enabled

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
```

### Button Grids

```css
/* Resolution and AA buttons */
.resolution-grid, .aa-grid {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 4px;
}

.shadow-samples-grid {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 4px;
}
```

### Active States

```css
/* AA buttons */
.aa-btn.active {
  background: linear-gradient(135deg, #10b981, #059669);
  border-color: #10b981;
  color: white;
}

/* Shadow sample buttons */
.shadow-sample-btn.active {
  background: linear-gradient(135deg, #f59e0b, #d97706);
  border-color: #f59e0b;
  color: white;
}
```

---

## Responsive Design

All controls are responsive and adapt to different screen sizes:

### Desktop (> 900px)
- Full sidebar with all controls visible
- Labels shown on all buttons

### Tablet (768px - 900px)
- Stacked layout
- Slightly smaller controls

### Mobile (< 640px)
- Compact controls
- Icons-only for tabs
- Touch-friendly slider thumbs
- Scrollable control panel
