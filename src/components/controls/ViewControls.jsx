import Toggle from '../ui/Toggle';
import Slider from '../ui/Slider';
import './ControlSection.css';

const AA_OPTIONS = [
  { value: 0, label: 'Off', samples: 1 },
  { value: 1, label: '2×2', samples: 4 },
  { value: 2, label: '4×4', samples: 16 },
];

function ViewControls({ view, onChange, disabled }) {
  const handleChange = (prop, value) => {
    onChange({ ...view, [prop]: value });
  };

  const currentAA = AA_OPTIONS.find(opt => opt.value === view.antiAliasing) || AA_OPTIONS[0];

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
        <span className="group-label">Anti-Aliasing</span>
        <div className="aa-grid">
          {AA_OPTIONS.map((opt) => (
            <button
              key={opt.value}
              className={`aa-btn ${view.antiAliasing === opt.value ? 'active' : ''}`}
              onClick={() => handleChange('antiAliasing', opt.value)}
              disabled={disabled}
              title={`${opt.samples} sample${opt.samples > 1 ? 's' : ''} per pixel`}
            >
              {opt.label}
            </button>
          ))}
        </div>
        <p className="aa-info">
          <span className="aa-samples">{currentAA.samples} sample{currentAA.samples > 1 ? 's' : ''}/px</span>
          {view.antiAliasing > 0 && (
            <span className="aa-warning">• Slower render</span>
          )}
        </p>
      </div>

      <div className="control-divider" />

      <div className="control-group">
        <span className="group-label">Render Resolution</span>
        <div className="resolution-grid">
          {[256, 512, 768, 1024].map((res) => (
            <button
              key={res}
              className={`res-btn ${view.resolution === res ? 'active' : ''}`}
              onClick={() => handleChange('resolution', res)}
              disabled={disabled}
            >
              {res}
            </button>
          ))}
        </div>
        <p className="resolution-note">
          Higher = better quality, slower render
        </p>
      </div>
    </div>
  );
}

export default ViewControls;
