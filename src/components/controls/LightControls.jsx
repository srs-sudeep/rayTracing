import { useState } from 'react';
import Slider from '../ui/Slider';
import './ControlSection.css';

// Preset light colors
const LIGHT_COLORS = [
  { name: 'White', color: { r: 1.0, g: 1.0, b: 1.0 } },
  { name: 'Warm', color: { r: 1.0, g: 0.9, b: 0.7 } },
  { name: 'Cool', color: { r: 0.8, g: 0.9, b: 1.0 } },
  { name: 'Red', color: { r: 1.0, g: 0.3, b: 0.3 } },
  { name: 'Green', color: { r: 0.3, g: 1.0, b: 0.4 } },
  { name: 'Blue', color: { r: 0.3, g: 0.5, b: 1.0 } },
];

function LightControls({ lights, onChange, onAddLight, onRemoveLight, disabled }) {
  const [selectedLight, setSelectedLight] = useState(0);

  const currentLight = lights[selectedLight] || lights[0];

  const handleChange = (property, value) => {
    const updated = [...lights];
    updated[selectedLight] = { ...updated[selectedLight], [property]: value };
    onChange(updated);
  };

  const handleColorChange = (colorObj) => {
    const updated = [...lights];
    updated[selectedLight] = { ...updated[selectedLight], color: colorObj };
    onChange(updated);
  };

  const handleAddLight = () => {
    if (lights.length < 4) {
      onAddLight();
      setSelectedLight(lights.length); // Select the new light
    }
  };

  const handleRemoveLight = () => {
    if (lights.length > 1) {
      onRemoveLight(selectedLight);
      setSelectedLight(Math.max(0, selectedLight - 1));
    }
  };

  if (!currentLight) return null;

  return (
    <div className="control-section">
      {/* Light selector */}
      <div className="light-selector">
        <div className="light-tabs">
          {lights.map((light, index) => (
            <button
              key={index}
              className={`light-tab ${selectedLight === index ? 'active' : ''}`}
              onClick={() => setSelectedLight(index)}
              disabled={disabled}
              style={{
                '--light-color': `rgb(${Math.round(light.color.r * 255)}, ${Math.round(light.color.g * 255)}, ${Math.round(light.color.b * 255)})`
              }}
            >
              <span className="light-indicator" />
              <span className="light-label">Light {index + 1}</span>
            </button>
          ))}
        </div>
        <div className="light-actions">
          <button 
            className="light-action-btn add"
            onClick={handleAddLight}
            disabled={disabled || lights.length >= 4}
            title="Add Light"
          >
            +
          </button>
          <button 
            className="light-action-btn remove"
            onClick={handleRemoveLight}
            disabled={disabled || lights.length <= 1}
            title="Remove Light"
          >
            −
          </button>
        </div>
      </div>

      {/* Color presets */}
      <div className="subsection">
        <label className="subsection-label">Light Color</label>
        <div className="light-color-grid">
          {LIGHT_COLORS.map((preset) => (
            <button
              key={preset.name}
              className={`light-color-btn ${
                Math.abs(currentLight.color.r - preset.color.r) < 0.01 &&
                Math.abs(currentLight.color.g - preset.color.g) < 0.01 &&
                Math.abs(currentLight.color.b - preset.color.b) < 0.01
                  ? 'active'
                  : ''
              }`}
              style={{
                background: `rgb(${Math.round(preset.color.r * 255)}, ${Math.round(preset.color.g * 255)}, ${Math.round(preset.color.b * 255)})`
              }}
              onClick={() => handleColorChange(preset.color)}
              disabled={disabled}
              title={preset.name}
            />
          ))}
        </div>
      </div>

      {/* Intensity */}
      <div className="subsection">
        <Slider
          id={`light-intensity-${selectedLight}`}
          label="Intensity"
          value={currentLight.intensity}
          min={0}
          max={2}
          step={0.05}
          color="#fbbf24"
          onChange={(v) => handleChange('intensity', v)}
          disabled={disabled}
          formatValue={(v) => `${(v * 100).toFixed(0)}%`}
        />
      </div>

      <div className="control-divider" />

      {/* Position */}
      <div className="control-group">
        <label className="subsection-label">Position</label>
        <Slider
          id={`light-x-${selectedLight}`}
          label="X"
          value={currentLight.x}
          min={-5}
          max={5}
          step={0.1}
          color="#ff6b6b"
          onChange={(v) => handleChange('x', v)}
          disabled={disabled}
        />
        <Slider
          id={`light-y-${selectedLight}`}
          label="Y"
          value={currentLight.y}
          min={-5}
          max={5}
          step={0.1}
          color="#4ecdc4"
          onChange={(v) => handleChange('y', v)}
          disabled={disabled}
        />
        <Slider
          id={`light-z-${selectedLight}`}
          label="Z"
          value={currentLight.z}
          min={-5}
          max={5}
          step={0.1}
          color="#ffe66d"
          onChange={(v) => handleChange('z', v)}
          disabled={disabled}
        />
      </div>

      <div className="control-info">
        <span className="info-label">Light {selectedLight + 1}</span>
        <code className="info-value">
          ({currentLight.x.toFixed(1)}, {currentLight.y.toFixed(1)}, {currentLight.z.toFixed(1)})
        </code>
      </div>

      {lights.length > 1 && (
        <div className="control-hint">
          {lights.length} lights active • Max 4
        </div>
      )}
    </div>
  );
}

export default LightControls;
