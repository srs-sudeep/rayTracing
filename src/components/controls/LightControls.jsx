import Slider from '../ui/Slider';
import './ControlSection.css';

function LightControls({ light, onChange, disabled }) {
  const handleChange = (axis, value) => {
    onChange({ ...light, [axis]: value });
  };

  return (
    <div className="control-section">
      <div className="control-group">
        <Slider
          id="light-x"
          label="Position X"
          value={light.x}
          min={-5}
          max={5}
          step={0.1}
          color="#ff6b6b"
          onChange={(v) => handleChange('x', v)}
          disabled={disabled}
        />
        <Slider
          id="light-y"
          label="Position Y"
          value={light.y}
          min={-5}
          max={5}
          step={0.1}
          color="#4ecdc4"
          onChange={(v) => handleChange('y', v)}
          disabled={disabled}
        />
        <Slider
          id="light-z"
          label="Position Z"
          value={light.z}
          min={-5}
          max={5}
          step={0.1}
          color="#ffe66d"
          onChange={(v) => handleChange('z', v)}
          disabled={disabled}
        />
      </div>

      <div className="control-info">
        <span className="info-label">Position</span>
        <code className="info-value">
          ({light.x.toFixed(1)}, {light.y.toFixed(1)}, {light.z.toFixed(1)})
        </code>
      </div>
    </div>
  );
}

export default LightControls;

