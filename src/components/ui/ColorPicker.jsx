import { useRef } from 'react';
import './ColorPicker.css';

const COLOR_PRESETS = [
  { name: 'Red', color: { r: 0.9, g: 0.2, b: 0.15 } },
  { name: 'Orange', color: { r: 1.0, g: 0.5, b: 0.1 } },
  { name: 'Gold', color: { r: 1.0, g: 0.84, b: 0.0 } },
  { name: 'Green', color: { r: 0.2, g: 0.85, b: 0.3 } },
  { name: 'Cyan', color: { r: 0.2, g: 0.8, b: 0.9 } },
  { name: 'Blue', color: { r: 0.2, g: 0.4, b: 0.9 } },
  { name: 'Purple', color: { r: 0.6, g: 0.2, b: 0.9 } },
  { name: 'Pink', color: { r: 0.95, g: 0.4, b: 0.6 } },
  { name: 'White', color: { r: 0.95, g: 0.95, b: 0.95 } },
  { name: 'Gray', color: { r: 0.5, g: 0.5, b: 0.5 } },
];

function rgbToHex(r, g, b) {
  const toHex = (n) => {
    const hex = Math.round(n * 255).toString(16);
    return hex.length === 1 ? '0' + hex : hex;
  };
  return `#${toHex(r)}${toHex(g)}${toHex(b)}`;
}

function hexToRgb(hex) {
  const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result ? {
    r: parseInt(result[1], 16) / 255,
    g: parseInt(result[2], 16) / 255,
    b: parseInt(result[3], 16) / 255
  } : null;
}

function ColorPicker({ color, onChange, disabled, label = 'Color' }) {
  const inputRef = useRef(null);
  
  const currentHex = rgbToHex(color.r, color.g, color.b);
  
  const handleColorInputChange = (e) => {
    const rgb = hexToRgb(e.target.value);
    if (rgb) {
      onChange(rgb);
    }
  };
  
  const handlePresetClick = (presetColor) => {
    onChange(presetColor);
  };
  
  const isPresetActive = (preset) => {
    return Math.abs(color.r - preset.r) < 0.02 && 
           Math.abs(color.g - preset.g) < 0.02 && 
           Math.abs(color.b - preset.b) < 0.02;
  };

  return (
    <div className="color-picker">
      <div className="color-picker-header">
        <span className="color-picker-label">{label}</span>
        <span className="color-picker-value">{currentHex.toUpperCase()}</span>
      </div>
      
      <div className="color-picker-row">
        {/* Custom Color Input */}
        <button 
          className="color-picker-custom"
          onClick={() => inputRef.current?.click()}
          disabled={disabled}
          style={{ background: currentHex }}
          title="Pick custom color"
        >
          <span className="color-picker-icon">🎨</span>
          <input
            ref={inputRef}
            type="color"
            value={currentHex}
            onChange={handleColorInputChange}
            disabled={disabled}
            className="color-input-hidden"
          />
        </button>
        
        {/* Preset Colors */}
        <div className="color-preset-grid">
          {COLOR_PRESETS.map((preset) => (
            <button
              key={preset.name}
              className={`color-preset-btn ${isPresetActive(preset.color) ? 'active' : ''}`}
              style={{ 
                background: `rgb(${preset.color.r * 255}, ${preset.color.g * 255}, ${preset.color.b * 255})` 
              }}
              onClick={() => handlePresetClick(preset.color)}
              disabled={disabled}
              title={preset.name}
            />
          ))}
        </div>
      </div>
    </div>
  );
}

export default ColorPicker;

