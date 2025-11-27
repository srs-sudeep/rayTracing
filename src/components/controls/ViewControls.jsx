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
          id="show-grid"
          label="Show Grid"
          checked={view.showGrid}
          onChange={(v) => handleChange('showGrid', v)}
          disabled={disabled}
        />
        
        {view.showGrid && (
          <Slider
            id="grid-size"
            label="Grid Size"
            value={view.gridSize}
            min={16}
            max={128}
            step={16}
            color="#94a3b8"
            onChange={(v) => handleChange('gridSize', v)}
            disabled={disabled}
            formatValue={(v) => `${v}px`}
          />
        )}
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
    </div>
  );
}

export default ViewControls;

