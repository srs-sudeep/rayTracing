import './Header.css';

function Header({ renderTime, resolution }) {
  const pixelCount = resolution * resolution;
  const pixelsPerSecond = renderTime > 0 ? (pixelCount / (renderTime / 1000)).toFixed(0) : 0;

  return (
    <header className="header">
      <div className="header-brand">
        <div className="logo">
          <span className="logo-icon">◈</span>
          <div className="logo-text">
            <h1>RayTracerStudio</h1>
            <span className="logo-subtitle">WebAssembly Edition</span>
          </div>
        </div>
      </div>

      <div className="header-stats">
        <div className="stat">
          <span className="stat-value">{renderTime.toFixed(1)}</span>
          <span className="stat-label">ms</span>
        </div>
        <div className="stat-divider" />
        <div className="stat">
          <span className="stat-value">{(pixelsPerSecond / 1000000).toFixed(2)}</span>
          <span className="stat-label">MP/s</span>
        </div>
        <div className="stat-divider" />
        <div className="stat">
          <span className="stat-value">{Math.round(1000 / Math.max(renderTime, 1))}</span>
          <span className="stat-label">FPS</span>
        </div>
      </div>

      <div className="header-actions">
        <a 
          href="https://github.com" 
          target="_blank" 
          rel="noopener noreferrer"
          className="header-link"
        >
          GitHub
        </a>
      </div>
    </header>
  );
}

export default Header;

