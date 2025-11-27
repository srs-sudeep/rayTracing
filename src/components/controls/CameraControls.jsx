import Slider from '../ui/Slider';
import './ControlSection.css';

function CameraControls({ camera, onChange, onReset, disabled }) {
  const handleChange = (axis, value) => {
    onChange({ ...camera, [axis]: value });
  };

  return (
    <div className="control-section">
      <div className="control-group">
        <Slider
          id="camera-x"
          label="Position X"
          value={camera.x}
          min={-10}
          max={10}
          step={0.1}
          color="#60a5fa"
          onChange={(v) => handleChange('x', v)}
          disabled={disabled}
        />
        <Slider
          id="camera-y"
          label="Position Y"
          value={camera.y}
          min={-10}
          max={10}
          step={0.1}
          color="#34d399"
          onChange={(v) => handleChange('y', v)}
          disabled={disabled}
        />
        <Slider
          id="camera-z"
          label="Position Z"
          value={camera.z}
          min={-10}
          max={-1}
          step={0.1}
          color="#fbbf24"
          onChange={(v) => handleChange('z', v)}
          disabled={disabled}
        />
      </div>

      <div className="control-info">
        <span className="info-label">Distance</span>
        <code className="info-value">
          {Math.sqrt(camera.x ** 2 + camera.y ** 2 + camera.z ** 2).toFixed(2)}
        </code>
      </div>

      <div className="control-actions">
        <button 
          className="action-btn" 
          onClick={onReset}
          disabled={disabled}
        >
          ↺ Reset Camera
        </button>
      </div>

      <p className="control-hint">
        💡 Drag on canvas to orbit camera
      </p>
    </div>
  );
}

export default CameraControls;

