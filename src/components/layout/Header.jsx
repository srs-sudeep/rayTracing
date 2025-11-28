import { useState } from 'react';
import InfoModal from './InfoModal';
import './Header.css';

function Header({ renderTime, resolution }) {
  const [showInfo, setShowInfo] = useState(false);
  
  const pixelCount = resolution * resolution;
  const pixelsPerSecond = renderTime > 0 ? (pixelCount / (renderTime / 1000)).toFixed(0) : 0;

  // In dev mode (localhost:5173), link to docs dev server (localhost:3000)
  // In production, use relative /docs path
  const isDev = import.meta.env.DEV;
  const docsUrl = isDev ? 'http://localhost:3000/docs/' : '/docs';

  return (
    <>
      <header className="header">
        <div className="header-brand">
          <div className="logo">
            <div className="logo-icon">
              <svg viewBox="0 0 100 100" className="logo-svg">
                <defs>
                  <linearGradient id="sphereGradHeader" x1="30%" y1="30%" x2="70%" y2="70%">
                    <stop offset="0%" style={{stopColor:'#ff6b6b'}}/>
                    <stop offset="100%" style={{stopColor:'#e85d4c'}}/>
                  </linearGradient>
                </defs>
                <circle cx="50" cy="50" r="35" fill="url(#sphereGradHeader)"/>
                <ellipse cx="38" cy="38" rx="8" ry="5" fill="rgba(255,255,255,0.4)" transform="rotate(-45 38 38)"/>
              </svg>
            </div>
            <div className="logo-text">
              <h1>RayTracer <span className="logo-highlight">Studio</span></h1>
              <span className="logo-tagline">Real-time CPU Ray Tracing in the Browser</span>
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
            href={docsUrl}
            target={isDev ? "_blank" : "_self"}
            rel={isDev ? "noopener noreferrer" : undefined}
            className="header-btn docs-btn"
          >
            <span>📖</span>
            <span>Docs</span>
          </a>
          <button className="header-btn" onClick={() => setShowInfo(true)}>
            <span>ℹ️</span>
            <span>About</span>
          </button>
          <a 
            href="https://github.com/srs-sudeep/rayTracing" 
            target="_blank" 
            rel="noopener noreferrer"
            className="header-btn"
          >
            <span>⭐</span>
            <span>GitHub</span>
          </a>
        </div>
      </header>

      {showInfo && <InfoModal onClose={() => setShowInfo(false)} />}
    </>
  );
}

export default Header;
