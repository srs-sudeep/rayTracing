import './Toggle.css';

function Toggle({ id, label, checked, onChange, disabled = false }) {
  return (
    <div className="toggle-group">
      <label htmlFor={id} className="toggle-label">{label}</label>
      <button
        id={id}
        role="switch"
        aria-checked={checked}
        className={`toggle ${checked ? 'active' : ''}`}
        onClick={() => onChange(!checked)}
        disabled={disabled}
      >
        <span className="toggle-thumb" />
      </button>
    </div>
  );
}

export default Toggle;
