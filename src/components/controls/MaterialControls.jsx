import Slider from '../ui/Slider';
import ColorPicker from '../ui/ColorPicker';
import './ControlSection.css';
import './MaterialControls.css';

const MATERIAL_PRESETS = [
  { 
    name: 'Matte', 
    icon: '🧱',
    desc: 'Non-reflective, diffuse surface',
    specular: 0.1, 
    shininess: 8, 
    reflectivity: 0, 
    transparency: 0, 
    refractiveIndex: 1.0 
  },
  { 
    name: 'Plastic', 
    icon: '🎨',
    desc: 'Slight shine and reflection',
    specular: 0.4, 
    shininess: 32, 
    reflectivity: 0.1, 
    transparency: 0, 
    refractiveIndex: 1.0 
  },
  { 
    name: 'Glossy', 
    icon: '✨',
    desc: 'Smooth with soft reflections',
    specular: 0.6, 
    shininess: 64, 
    reflectivity: 0.3, 
    transparency: 0, 
    refractiveIndex: 1.0 
  },
  { 
    name: 'Metal', 
    icon: '⚙️',
    desc: 'Highly reflective metallic',
    specular: 0.9, 
    shininess: 128, 
    reflectivity: 0.7, 
    transparency: 0, 
    refractiveIndex: 1.0 
  },
  { 
    name: 'Chrome', 
    icon: '🪞',
    desc: 'Perfect mirror surface',
    specular: 1.0, 
    shininess: 256, 
    reflectivity: 0.95, 
    transparency: 0, 
    refractiveIndex: 1.0 
  },
  { 
    name: 'Glass', 
    icon: '🫧',
    desc: 'Clear with refraction',
    specular: 1.0, 
    shininess: 256, 
    reflectivity: 0.1, 
    transparency: 0.95, 
    refractiveIndex: 1.5 
  },
  { 
    name: 'Diamond', 
    icon: '💎',
    desc: 'High dispersion crystal',
    specular: 1.0, 
    shininess: 512, 
    reflectivity: 0.2, 
    transparency: 0.95, 
    refractiveIndex: 2.4 
  },
  { 
    name: 'Water', 
    icon: '💧',
    desc: 'Liquid-like refraction',
    specular: 0.8, 
    shininess: 128, 
    reflectivity: 0.1, 
    transparency: 0.9, 
    refractiveIndex: 1.33 
  },
];

// IOR Reference values
const IOR_PRESETS = [
  { name: 'Air', value: 1.0 },
  { name: 'Water', value: 1.33 },
  { name: 'Glass', value: 1.5 },
  { name: 'Crystal', value: 2.0 },
  { name: 'Diamond', value: 2.4 },
];

function MaterialControls({ material, onChange, disabled }) {
  const handleChange = (prop, value) => {
    onChange({ ...material, [prop]: value });
  };

  const applyMaterialPreset = (preset) => {
    onChange({ 
      ...material, 
      specular: preset.specular,
      shininess: preset.shininess,
      reflectivity: preset.reflectivity,
      transparency: preset.transparency,
      refractiveIndex: preset.refractiveIndex
    });
  };

  const currentPreset = MATERIAL_PRESETS.find(preset => 
    material.specular === preset.specular &&
    material.shininess === preset.shininess &&
    material.reflectivity === preset.reflectivity &&
    material.transparency === preset.transparency &&
    Math.abs(material.refractiveIndex - preset.refractiveIndex) < 0.01
  );

  const isTransparent = material.transparency > 0.01;

  return (
    <div className="control-section material-editor">
      {/* Material Presets - Quick Selection */}
      <div className="control-group">
        <span className="group-label">Material Presets</span>
        <div className="material-preset-grid">
          {MATERIAL_PRESETS.map((preset) => {
            const isActive = currentPreset?.name === preset.name;
            return (
              <button
                key={preset.name}
                className={`material-preset-btn ${isActive ? 'active' : ''}`}
                onClick={() => applyMaterialPreset(preset)}
                disabled={disabled}
                title={preset.desc}
              >
                <span className="preset-icon">{preset.icon}</span>
                <span className="preset-name">{preset.name}</span>
              </button>
            );
          })}
        </div>
      </div>

      <div className="control-divider" />

      {/* Color Selection */}
      <div className="control-group">
        <ColorPicker
          color={material.color}
          onChange={(color) => handleChange('color', color)}
          disabled={disabled}
          label="Surface Color"
        />
      </div>

      <div className="control-divider" />

      {/* Surface Properties */}
      <div className="control-group">
        <div className="group-header">
          <span className="group-label">Surface Properties</span>
          <span className="group-badge">Lighting</span>
        </div>
        <Slider
          id="specular"
          label="Specular Intensity"
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
          max={512}
          step={2}
          color="#f472b6"
          onChange={(v) => handleChange('shininess', v)}
          disabled={disabled}
          formatValue={(v) => v.toFixed(0)}
        />
      </div>

      <div className="control-divider" />

      {/* Reflection */}
      <div className="control-group">
        <div className="group-header">
          <span className="group-label">Reflection</span>
          <span className={`group-badge ${material.reflectivity > 0.5 ? 'highlight' : ''}`}>
            {material.reflectivity > 0.8 ? 'Mirror' : material.reflectivity > 0.3 ? 'Shiny' : 'Subtle'}
          </span>
        </div>
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

      {/* Transparency & Refraction */}
      <div className="control-group">
        <div className="group-header">
          <span className="group-label">Transparency & Refraction</span>
          <span className={`group-badge ${isTransparent ? 'highlight' : ''}`}>
            {isTransparent ? 'Enabled' : 'Opaque'}
          </span>
        </div>
        <Slider
          id="transparency"
          label="Transparency"
          value={material.transparency}
          min={0}
          max={1}
          step={0.05}
          color="#34d399"
          onChange={(v) => handleChange('transparency', v)}
          disabled={disabled}
        />
        
        <div className={`ior-section ${!isTransparent ? 'disabled' : ''}`}>
          <Slider
            id="refractiveIndex"
            label="Index of Refraction (IOR)"
            value={material.refractiveIndex}
            min={1}
            max={3}
            step={0.01}
            color="#fbbf24"
            onChange={(v) => handleChange('refractiveIndex', v)}
            disabled={disabled || !isTransparent}
            formatValue={(v) => v.toFixed(2)}
          />
          
          {/* IOR Quick Presets */}
          <div className="ior-presets">
            {IOR_PRESETS.map((preset) => (
              <button
                key={preset.name}
                className={`ior-preset-btn ${
                  Math.abs(material.refractiveIndex - preset.value) < 0.02 ? 'active' : ''
                }`}
                onClick={() => handleChange('refractiveIndex', preset.value)}
                disabled={disabled || !isTransparent}
                title={`IOR: ${preset.value}`}
              >
                {preset.name}
              </button>
            ))}
          </div>
        </div>
      </div>

      {/* Material Summary */}
      <div className="material-summary">
        <div className="summary-item">
          <span className="summary-icon" style={{ background: `rgb(${material.color.r * 255}, ${material.color.g * 255}, ${material.color.b * 255})` }} />
          <span className="summary-label">{currentPreset?.name || 'Custom'}</span>
        </div>
        <div className="summary-stats">
          {material.reflectivity > 0.3 && <span className="stat-tag reflect">🪞</span>}
          {material.transparency > 0.3 && <span className="stat-tag transparent">💎</span>}
          {material.shininess > 100 && <span className="stat-tag shiny">✨</span>}
        </div>
      </div>
    </div>
  );
}

export default MaterialControls;
