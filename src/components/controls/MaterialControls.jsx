import Slider from '../ui/Slider';
import './ControlSection.css';

function MaterialControls({ material, onChange, disabled }) {
  const handleChange = (prop, value) => {
    onChange({ ...material, [prop]: value });
  };

  const applyPreset = (preset) => {
    onChange({ ...material, ...preset });
  };

  return (
    <div className="control-section">
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

      <div className="preset-section">
        <span className="info-label">Presets</span>
        <div className="preset-chips">
          <button 
            className="chip" 
            onClick={() => applyPreset({ specular: 0.1, shininess: 8, reflectivity: 0 })}
            disabled={disabled}
          >
            Matte
          </button>
          <button 
            className="chip" 
            onClick={() => applyPreset({ specular: 0.4, shininess: 32, reflectivity: 0.1 })}
            disabled={disabled}
          >
            Plastic
          </button>
          <button 
            className="chip" 
            onClick={() => applyPreset({ specular: 0.6, shininess: 64, reflectivity: 0.3 })}
            disabled={disabled}
          >
            Glossy
          </button>
          <button 
            className="chip" 
            onClick={() => applyPreset({ specular: 0.9, shininess: 128, reflectivity: 0.7 })}
            disabled={disabled}
          >
            Metal
          </button>
          <button 
            className="chip chip-accent" 
            onClick={() => applyPreset({ specular: 1.0, shininess: 256, reflectivity: 0.95 })}
            disabled={disabled}
          >
            Mirror
          </button>
        </div>
      </div>
    </div>
  );
}

export default MaterialControls;
