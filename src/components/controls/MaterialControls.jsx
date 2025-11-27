import Slider from '../ui/Slider';
import './ControlSection.css';

const COLOR_PRESETS = [
  { name: 'Red', color: { r: 0.9, g: 0.2, b: 0.15 } },
  { name: 'Blue', color: { r: 0.2, g: 0.4, b: 0.9 } },
  { name: 'Green', color: { r: 0.2, g: 0.85, b: 0.3 } },
  { name: 'Gold', color: { r: 1.0, g: 0.84, b: 0.0 } },
  { name: 'Purple', color: { r: 0.6, g: 0.2, b: 0.9 } },
  { name: 'White', color: { r: 0.95, g: 0.95, b: 0.95 } },
];

const MATERIAL_PRESETS = [
  { name: 'Matte', specular: 0.1, shininess: 8, reflectivity: 0 },
  { name: 'Plastic', specular: 0.4, shininess: 32, reflectivity: 0.1 },
  { name: 'Glossy', specular: 0.6, shininess: 64, reflectivity: 0.3 },
  { name: 'Metal', specular: 0.9, shininess: 128, reflectivity: 0.7 },
  { name: 'Mirror', specular: 1.0, shininess: 256, reflectivity: 0.95 },
];

function MaterialControls({ material, onChange, disabled }) {
  const handleChange = (prop, value) => {
    onChange({ ...material, [prop]: value });
  };

  const applyColorPreset = (color) => {
    onChange({ ...material, color });
  };

  const applyMaterialPreset = (preset) => {
    onChange({ 
      ...material, 
      specular: preset.specular,
      shininess: preset.shininess,
      reflectivity: preset.reflectivity
    });
  };

  return (
    <div className="control-section">
      {/* Color Selection */}
      <div className="control-group">
        <span className="group-label">Color</span>
        <div className="color-grid">
          {COLOR_PRESETS.map((preset) => (
            <button
              key={preset.name}
              className={`color-btn ${
                material.color.r === preset.color.r && 
                material.color.g === preset.color.g && 
                material.color.b === preset.color.b ? 'active' : ''
              }`}
              style={{ 
                background: `rgb(${preset.color.r * 255}, ${preset.color.g * 255}, ${preset.color.b * 255})` 
              }}
              onClick={() => applyColorPreset(preset.color)}
              disabled={disabled}
              title={preset.name}
            />
          ))}
        </div>
      </div>

      <div className="control-divider" />

      {/* Material Properties */}
      <div className="control-group">
        <Slider
          id="specular"
          label="Specular"
          value={material.specular}
          min={0}
          max={1}
          step={0.05}
          color="#a78bfa"
          onChange={(v) => handleChange('specular', v)}
          disabled={disabled}
        />
        <Slider
          id="shininess"
          label="Shininess"
          value={material.shininess}
          min={2}
          max={256}
          step={2}
          color="#f472b6"
          onChange={(v) => handleChange('shininess', v)}
          disabled={disabled}
          formatValue={(v) => v.toFixed(0)}
        />
        <Slider
          id="reflectivity"
          label="Reflectivity"
          value={material.reflectivity}
          min={0}
          max={1}
          step={0.05}
          color="#60a5fa"
          onChange={(v) => handleChange('reflectivity', v)}
          disabled={disabled}
        />
      </div>

      <div className="control-divider" />

      {/* Material Presets */}
      <div className="control-group">
        <span className="group-label">Material Type</span>
        <div className="preset-chips">
          {MATERIAL_PRESETS.map((preset) => {
            const isActive = 
              material.specular === preset.specular &&
              material.shininess === preset.shininess &&
              material.reflectivity === preset.reflectivity;
            return (
              <button
                key={preset.name}
                className={`chip ${isActive ? 'chip-active' : ''}`}
                onClick={() => applyMaterialPreset(preset)}
                disabled={disabled}
              >
                {preset.name}
              </button>
            );
          })}
        </div>
      </div>
    </div>
  );
}

export default MaterialControls;
