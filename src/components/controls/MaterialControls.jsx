import Slider from '../ui/Slider';
import './ControlSection.css';

function MaterialControls({ material, onChange, disabled }) {
  const handleChange = (prop, value) => {
    onChange({ ...material, [prop]: value });
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
      </div>

      <div className="control-info">
        <span className="info-label">Preset</span>
        <span className="info-value info-tag">Custom</span>
      </div>

      <div className="preset-chips">
        <button 
          className="chip" 
          onClick={() => onChange({ specular: 0.1, shininess: 8 })}
          disabled={disabled}
        >
          Matte
        </button>
        <button 
          className="chip" 
          onClick={() => onChange({ specular: 0.4, shininess: 32 })}
          disabled={disabled}
        >
          Plastic
        </button>
        <button 
          className="chip" 
          onClick={() => onChange({ specular: 0.8, shininess: 64 })}
          disabled={disabled}
        >
          Glossy
        </button>
        <button 
          className="chip" 
          onClick={() => onChange({ specular: 1.0, shininess: 256 })}
          disabled={disabled}
        >
          Metal
        </button>
      </div>
    </div>
  );
}

export default MaterialControls;

