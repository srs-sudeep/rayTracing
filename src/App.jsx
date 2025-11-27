import { useState } from 'react';
import useWasm from './hooks/useWasm';
import Header from './components/layout/Header';
import RaytracerCanvas from './components/canvas/RaytracerCanvas';
import ControlPanel from './components/controls/ControlPanel';
import './App.css';

const DEFAULT_CAMERA = { x: 0, y: 0, z: -3 };

function App() {
  const { wasmModule, loading, error } = useWasm();
  
  // Scene state
  const [light, setLight] = useState({ x: 2, y: 2, z: -1 });
  const [material, setMaterial] = useState({ specular: 0.5, shininess: 32 });
  const [camera, setCamera] = useState({ ...DEFAULT_CAMERA });
  const [view, setView] = useState({ 
    showGrid: true, 
    gridSize: 64,
    resolution: 512 
  });
  const [renderTime, setRenderTime] = useState(0);

  const handleCameraReset = () => {
    setCamera({ ...DEFAULT_CAMERA });
    if (wasmModule) {
      wasmModule.updateCamera(DEFAULT_CAMERA.x, DEFAULT_CAMERA.y, DEFAULT_CAMERA.z);
    }
  };

  const isDisabled = loading || !!error;

  return (
    <div className="app">
      <Header renderTime={renderTime} resolution={view.resolution} />

      <main className="main-content">
        <div className="canvas-area">
          {loading && (
            <div className="status-card">
              <div className="spinner" />
              <p className="status-title">Initializing WebAssembly</p>
              <p className="status-subtitle">Loading ray tracing engine...</p>
            </div>
          )}
          
          {error && (
            <div className="status-card error">
              <span className="status-icon">⚠</span>
              <p className="status-title">Failed to Load Module</p>
              <p className="status-subtitle">{error}</p>
              <code className="status-hint">Run: npm run build:wasm</code>
            </div>
          )}

          {!loading && !error && wasmModule && (
            <RaytracerCanvas
              wasmModule={wasmModule}
              light={light}
              material={material}
              camera={camera}
              view={view}
              onCameraChange={setCamera}
              onRenderTime={setRenderTime}
            />
          )}
        </div>

        <aside className="sidebar">
          <ControlPanel
            light={light}
            onLightChange={setLight}
            material={material}
            onMaterialChange={setMaterial}
            camera={camera}
            onCameraChange={setCamera}
            onCameraReset={handleCameraReset}
            view={view}
            onViewChange={setView}
            disabled={isDisabled}
          />

          <div className="scene-info">
            <h3>Scene</h3>
            <ul>
              <li><span>Objects</span><span>1 Sphere</span></li>
              <li><span>Lights</span><span>1 Point</span></li>
              <li><span>Shader</span><span>Blinn-Phong</span></li>
            </ul>
          </div>
        </aside>
      </main>

      <footer className="footer">
        <span>RayTracerStudio</span>
        <span className="footer-sep">•</span>
        <span>C++ / WebAssembly / React</span>
      </footer>
    </div>
  );
}

export default App;
