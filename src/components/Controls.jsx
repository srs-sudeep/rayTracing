function Controls({ lightPosition, onLightChange, disabled }) {
  const sliders = [
    { axis: 'x', label: 'Light X', min: -5, max: 5, color: '#ff6b6b' },
    { axis: 'y', label: 'Light Y', min: -5, max: 5, color: '#4ecdc4' },
    { axis: 'z', label: 'Light Z', min: -5, max: 5, color: '#ffe66d' },
  ];

  return (
    <div className="controls">
      <div className="controls-header">
        <h2>Light Controls</h2>
        <span className="controls-icon">☀</span>
      </div>

      <div className="controls-body">
        {sliders.map(({ axis, label, min, max, color }) => (
          <div key={axis} className="slider-group">
            <div className="slider-header">
              <label htmlFor={`slider-${axis}`}>{label}</label>
              <span 
                className="slider-value"
                style={{ color }}
              >
                {lightPosition[axis].toFixed(2)}
              </span>
            </div>
            <input
              id={`slider-${axis}`}
              type="range"
              min={min}
              max={max}
              step={0.1}
              value={lightPosition[axis]}
              onChange={(e) => onLightChange(axis, parseFloat(e.target.value))}
              disabled={disabled}
              className="slider"
              style={{ '--slider-color': color }}
            />
          </div>
        ))}
      </div>

      <div className="controls-footer">
        <div className="position-display">
          <span className="position-label">Position</span>
          <code className="position-value">
            ({lightPosition.x.toFixed(1)}, {lightPosition.y.toFixed(1)}, {lightPosition.z.toFixed(1)})
          </code>
        </div>
      </div>

      <div className="controls-info">
        <h3>Scene Info</h3>
        <ul>
          <li><span>Object:</span> Red Sphere</li>
          <li><span>Radius:</span> 1.0</li>
          <li><span>Shading:</span> Diffuse (Lambertian)</li>
        </ul>
      </div>
    </div>
  );
}

export default Controls;

