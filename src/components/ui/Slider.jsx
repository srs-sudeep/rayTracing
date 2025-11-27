import './Slider.css';

function Slider({ 
  id, 
  label, 
  value, 
  min, 
  max, 
  step = 0.1, 
  color = '#a78bfa',
  onChange,
  disabled = false,
  formatValue = (v) => v.toFixed(2)
}) {
  return (
    <div className="slider-group">
      <div className="slider-header">
        <label htmlFor={id}>{label}</label>
        <span className="slider-value" style={{ color }}>
          {formatValue(value)}
        </span>
      </div>
      <input
        id={id}
        type="range"
        min={min}
        max={max}
        step={step}
        value={value}
        onChange={(e) => onChange(parseFloat(e.target.value))}
        disabled={disabled}
        className="slider"
        style={{ '--slider-color': color }}
      />
    </div>
  );
}

export default Slider;
