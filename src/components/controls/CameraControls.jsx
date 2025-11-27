import Slider from '../ui/Slider';
import './ControlSection.css';

function CameraControls({ camera, onChange, onReset, disabled }) {
  const handlePositionChange = (axis, value) => {
    onChange({ ...camera, position: { ...camera.position, [axis]: value } });
  };

  const handleTargetChange = (axis, value) => {
    onChange({ ...camera, target: { ...camera.target, [axis]: value } });
  };

  const handleFovChange = (value) => {
    onChange({ ...camera, fov: value });
  };

  const distance = Math.sqrt(
    Math.pow(camera.position.x - camera.target.x, 2) +
    Math.pow(camera.position.y - camera.target.y, 2) +
    Math.pow(camera.position.z - camera.target.z, 2)
  );

  return (
    <div className="control-section">
      {/* Camera Position */}
      <div className="control-group">
        <span className="group-label">Position</span>
        <Slider
          id="camera-x"
          label="X"
          value={camera.position.x}
          min={-10}
          max={10}
          step={0.1}
          color="#60a5fa"
          onChange={(v) => handlePositionChange('x', v)}
          disabled={disabled}
        />
        <Slider
          id="camera-y"
          label="Y"
          value={camera.position.y}
          min={-10}
          max={10}
          step={0.1}
          color="#34d399"
          onChange={(v) => handlePositionChange('y', v)}
          disabled={disabled}
        />
        <Slider
          id="camera-z"
          label="Z"
          value={camera.position.z}
          min={-15}
          max={-1}
          step={0.1}
          color="#fbbf24"
          onChange={(v) => handlePositionChange('z', v)}
          disabled={disabled}
        />
      </div>

      <div className="control-divider" />

      {/* Look-at Target */}
      <div className="control-group">
        <span className="group-label">Look At Target</span>
        <Slider
          id="target-x"
          label="X"
          value={camera.target.x}
          min={-5}
          max={5}
          step={0.1}
          color="#f472b6"
          onChange={(v) => handleTargetChange('x', v)}
          disabled={disabled}
        />
        <Slider
          id="target-y"
          label="Y"
          value={camera.target.y}
          min={-5}
          max={5}
          step={0.1}
          color="#a78bfa"
          onChange={(v) => handleTargetChange('y', v)}
          disabled={disabled}
        />
        <Slider
          id="target-z"
          label="Z"
          value={camera.target.z}
          min={-5}
          max={5}
          step={0.1}
          color="#fb923c"
          onChange={(v) => handleTargetChange('z', v)}
          disabled={disabled}
        />
      </div>

      <div className="control-divider" />

      {/* Field of View */}
      <div className="control-group">
        <Slider
          id="fov"
          label="Field of View"
          value={camera.fov}
          min={20}
          max={120}
          step={5}
          color="#4ecdc4"
          onChange={handleFovChange}
          disabled={disabled}
          formatValue={(v) => `${v.toFixed(0)}°`}
        />
      </div>

      <div className="control-info">
        <span className="info-label">Distance</span>
        <code className="info-value">{distance.toFixed(2)}</code>
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
        🖱️ Drag on canvas to orbit • Scroll to zoom
      </p>
    </div>
  );
}

export default CameraControls;
