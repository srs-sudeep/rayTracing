import './SceneToolbar.css';

const SCENE_PRESETS = [
  { id: 0, name: 'Single', icon: '🔴' },
  { id: 1, name: 'Three', icon: '🎱' },
  { id: 2, name: 'Mirror', icon: '🪞' },
  { id: 3, name: 'Rainbow', icon: '🌈' },
  { id: 4, name: 'Glass', icon: '💎' },
];

function SceneToolbar({ activePreset, onPresetChange, sphereCount, disabled }) {
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
        <span className="info-chip">
          <span className="chip-icon">◉</span>
          {sphereCount} sphere{sphereCount > 1 ? 's' : ''}
        </span>
      </div>
    </div>
  );
}

export default SceneToolbar;

