import Toggle from '../ui/Toggle';
import Slider from '../ui/Slider';
import './ControlSection.css';

function ViewControls({ view, onChange, disabled }) {
  const handleChange = (prop, value) => {
    onChange({ ...view, [prop]: value });
  };

  return (
    <div className="control-section">
      <div className="control-group">
        <Toggle
          id="show-ground"
          label="Ground Plane"
          checked={view.showGroundPlane}
          onChange={(v) => handleChange('showGroundPlane', v)}
          disabled={disabled}
        />
        
        <Toggle
          id="show-grid"
          label="Grid Lines"
          checked={view.showGrid}
          onChange={(v) => handleChange('showGrid', v)}
          disabled={disabled || !view.showGroundPlane}
        />
        
        {view.showGrid && view.showGroundPlane && (
          <Slider
            id="grid-scale"
            label="Grid Scale"
            value={view.gridScale}
            min={0.5}
            max={4}
            step={0.5}
            color="#94a3b8"
            onChange={(v) => handleChange('gridScale', v)}
            disabled={disabled}
            formatValue={(v) => `${v.toFixed(1)}x`}
          />
        )}

        {view.showGroundPlane && (
          <Slider
            id="ground-reflectivity"
            label="Ground Reflect"
            value={view.groundReflectivity}
            min={0}
            max={0.8}
            step={0.05}
            color="#60a5fa"
            onChange={(v) => handleChange('groundReflectivity', v)}
            disabled={disabled}
          />
        )}
      </div>

      <div className="control-divider" />

      <div className="control-group">
        <Slider
          id="max-bounces"
          label="Max Bounces"
          value={view.maxBounces}
          min={1}
          max={8}
          step={1}
          color="#f472b6"
          onChange={(v) => handleChange('maxBounces', v)}
          disabled={disabled}
          formatValue={(v) => v.toFixed(0)}
        />
      </div>

      <div className="control-divider" />

      <div className="control-group">
        <div className="control-row">
          <span className="info-label">Resolution</span>
          <div className="button-group">
            {[256, 512, 1024].map((res) => (
              <button
                key={res}
                className={`size-btn ${view.resolution === res ? 'active' : ''}`}
                onClick={() => handleChange('resolution', res)}
                disabled={disabled}
              >
                {res}
              </button>
            ))}
          </div>
        </div>
      </div>

      <div className="control-info">
        <span className="info-label">Pixels</span>
        <code className="info-value">
          {(view.resolution * view.resolution).toLocaleString()}
        </code>
      </div>

      <p className="control-hint">
        🪞 Higher bounces = more reflections
      </p>
    </div>
  );
}

export default ViewControls;
