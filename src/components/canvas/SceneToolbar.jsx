import './SceneToolbar.css';

const SCENE_PRESETS = [
  { id: 0, name: 'Single', icon: '🔴' },
  { id: 1, name: 'Three', icon: '🎱' },
  { id: 2, name: 'Mirror', icon: '🪞' },
  { id: 3, name: 'Rainbow', icon: '🌈' },
  { id: 4, name: 'Glass', icon: '💎' },
  { id: 5, name: 'Shapes', icon: '🧊' },
];

function SceneToolbar({ activePreset, onPresetChange, objectCounts, disabled }) {
  const { spheres = 0, boxes = 0, cylinders = 0 } = objectCounts || {};
  const total = spheres + boxes + cylinders;

  return (
    <div className="scene-toolbar">
      <div className="toolbar-section">
        <span className="toolbar-label">Scene</span>
        <div className="preset-buttons">
          {SCENE_PRESETS.map((preset) => (
            <button
              key={preset.id}
              className={`preset-btn ${activePreset === preset.id ? 'active' : ''}`}
              onClick={() => onPresetChange(preset.id)}
              disabled={disabled}
              title={preset.name}
            >
              <span className="preset-icon">{preset.icon}</span>
              <span className="preset-name">{preset.name}</span>
            </button>
          ))}
        </div>
      </div>
      
      <div className="toolbar-info">
        {spheres > 0 && (
          <span className="info-chip" title="Spheres">
            <span className="chip-icon">⚪</span>
            {spheres}
          </span>
        )}
        {boxes > 0 && (
          <span className="info-chip" title="Boxes">
            <span className="chip-icon">▢</span>
            {boxes}
          </span>
        )}
        {cylinders > 0 && (
          <span className="info-chip" title="Cylinders">
            <span className="chip-icon">⬭</span>
            {cylinders}
          </span>
        )}
        <span className="info-chip total" title="Total objects">
          <span className="chip-icon">◉</span>
          {total} object{total !== 1 ? 's' : ''}
        </span>
      </div>
    </div>
  );
}

export default SceneToolbar;
