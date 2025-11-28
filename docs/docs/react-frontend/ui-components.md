---
sidebar_position: 5
---

# UI Components

Reusable UI components used throughout the application.

## Tabs

A horizontal tab bar for navigation between sections.

### Location

```
src/components/ui/Tabs.jsx
```

### Props

| Prop | Type | Description |
|------|------|-------------|
| `tabs` | Array | Tab definitions |
| `activeTab` | string | ID of active tab |
| `onChange` | Function | Tab change callback |

### Tab Definition

```typescript
interface Tab {
  id: string;
  label: string;
  icon: string;
}
```

### Usage

```jsx
const TABS = [
  { id: 'light', label: 'Light', icon: '☀' },
  { id: 'material', label: 'Material', icon: '◆' },
];

<Tabs 
  tabs={TABS} 
  activeTab={activeTab} 
  onChange={setActiveTab} 
/>
```

### Styling

```css
.tabs {
  display: flex;
  background: var(--bg-primary);
  border-bottom: 1px solid var(--border-subtle);
}

.tab {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: var(--space-sm);
  border-bottom: 2px solid transparent;
}

.tab.active {
  color: var(--text-primary);
  border-bottom-color: var(--accent-primary);
}
```

---

## Slider

A styled range input with label and value display.

### Location

```
src/components/ui/Slider.jsx
```

### Props

| Prop | Type | Default | Description |
|------|------|---------|-------------|
| `id` | string | required | HTML id |
| `label` | string | required | Display label |
| `value` | number | required | Current value |
| `min` | number | required | Minimum value |
| `max` | number | required | Maximum value |
| `step` | number | 0.1 | Step increment |
| `color` | string | '#a78bfa' | Thumb color |
| `onChange` | Function | required | Value change callback |
| `disabled` | boolean | false | Disable input |
| `formatValue` | Function | `v => v.toFixed(2)` | Value formatter |

### Usage

```jsx
<Slider
  id="specular"
  label="Specular"
  value={material.specular}
  min={0}
  max={1}
  step={0.05}
  color="#a78bfa"
  onChange={(v) => handleChange('specular', v)}
/>
```

### Implementation

```jsx
function Slider({ id, label, value, min, max, step, color, onChange, disabled, formatValue }) {
  return (
    <div className="slider-group">
      <div className="slider-header">
        <label htmlFor={id}>{label}</label>
        <span className="slider-value" style={{ color }}>
          {formatValue(value)}
        </span>
      </div>
      <input
        id={id}
        type="range"
        min={min}
        max={max}
        step={step}
        value={value}
        onChange={(e) => onChange(parseFloat(e.target.value))}
        disabled={disabled}
        className="slider"
        style={{ '--slider-color': color }}
      />
    </div>
  );
}
```

### Custom Styling

The slider uses CSS custom properties for theming:

```css
.slider::-webkit-slider-thumb {
  background: var(--slider-color, var(--accent-primary));
}
```

---

## Toggle

A boolean switch control.

### Location

```
src/components/ui/Toggle.jsx
```

### Props

| Prop | Type | Description |
|------|------|-------------|
| `id` | string | HTML id |
| `label` | string | Display label |
| `checked` | boolean | Toggle state |
| `onChange` | Function | State change callback |
| `disabled` | boolean | Disable toggle |

### Usage

```jsx
<Toggle
  id="show-grid"
  label="Show Grid"
  checked={view.showGrid}
  onChange={(v) => handleChange('showGrid', v)}
/>
```

### Implementation

```jsx
function Toggle({ id, label, checked, onChange, disabled }) {
  return (
    <div className="toggle-group">
      <label htmlFor={id}>{label}</label>
      <button
        id={id}
        role="switch"
        aria-checked={checked}
        className={`toggle ${checked ? 'active' : ''}`}
        onClick={() => onChange(!checked)}
        disabled={disabled}
      >
        <span className="toggle-thumb" />
      </button>
    </div>
  );
}
```

### Styling

```css
.toggle {
  width: 40px;
  height: 22px;
  background: var(--bg-primary);
  border-radius: 11px;
  position: relative;
}

.toggle.active {
  background: var(--accent-primary);
}

.toggle-thumb {
  position: absolute;
  width: 16px;
  height: 16px;
  background: white;
  border-radius: 50%;
  top: 3px;
  left: 3px;
  transition: left 0.2s;
}

.toggle.active .toggle-thumb {
  left: 21px;
}
```

---

## Design Tokens

All UI components use CSS custom properties defined in `:root`:

```css
:root {
  /* Colors */
  --bg-primary: #09090b;
  --bg-secondary: #0f0f12;
  --accent-primary: #e85d4c;
  --text-primary: #f4f4f5;
  --text-muted: #52525b;
  
  /* Spacing */
  --space-xs: 0.25rem;
  --space-sm: 0.5rem;
  --space-md: 1rem;
  
  /* Typography */
  --font-sans: 'Outfit', sans-serif;
  --font-mono: 'JetBrains Mono', monospace;
  
  /* Borders */
  --radius-sm: 4px;
  --radius-md: 8px;
  --border-subtle: #27272a;
  
  /* Transitions */
  --transition-fast: 120ms ease;
}
```

