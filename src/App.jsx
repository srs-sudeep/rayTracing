import { useState, useEffect } from 'react';
import RaytracerCanvas from './components/RaytracerCanvas';
import Controls from './components/Controls';

function App() {
  const [wasmModule, setWasmModule] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [lightPosition, setLightPosition] = useState({ x: 2, y: 2, z: -1 });
  const [renderTime, setRenderTime] = useState(0);

  useEffect(() => {
    async function loadWasm() {
      try {
        // Dynamic import of the WASM module from src/wasm
        const createModule = (await import('./wasm/raytracer.js')).default;
        const module = await createModule();
        setWasmModule(module);
        setLoading(false);
      } catch (err) {
        console.error('Failed to load WASM module:', err);
        setError(err.message);
        setLoading(false);
      }
    }

    loadWasm();
  }, []);

  const handleLightChange = (axis, value) => {
    setLightPosition(prev => ({
      ...prev,
      [axis]: value
    }));
  };

  return (
    <div className="app">
      <header className="header">
        <div className="logo">
          <span className="logo-icon">◈</span>
          <h1>RayTracerStudio</h1>
        </div>
        <div className="header-stats">
          {renderTime > 0 && (
            <span className="render-time">
              <span className="stat-label">Render:</span>
              <span className="stat-value">{renderTime.toFixed(2)}ms</span>
            </span>
          )}
        </div>
      </header>

      <main className="main-content">
        <div className="canvas-container">
          {loading && (
            <div className="loading-overlay">
              <div className="loading-spinner"></div>
              <p>Loading WebAssembly Module...</p>
            </div>
          )}
          
          {error && (
            <div className="error-overlay">
              <p className="error-title">⚠ Failed to Load</p>
              <p className="error-message">{error}</p>
              <p className="error-hint">
                Make sure to run <code>npm run build:wasm</code> first.
              </p>
            </div>
          )}

          {!loading && !error && wasmModule && (
            <RaytracerCanvas
              wasmModule={wasmModule}
              lightPosition={lightPosition}
              onRenderTime={setRenderTime}
            />
          )}
        </div>

        <aside className="controls-panel">
          <Controls
            lightPosition={lightPosition}
            onLightChange={handleLightChange}
            disabled={loading || error}
          />
        </aside>
      </main>

      <footer className="footer">
        <span>CPU Ray Tracing • WebAssembly • React</span>
      </footer>
    </div>
  );
}

export default App;

