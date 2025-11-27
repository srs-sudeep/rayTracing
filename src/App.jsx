import { useState, useEffect, useCallback } from 'react';
import useWasm from './hooks/useWasm';
import Header from './components/layout/Header';
import SceneToolbar from './components/canvas/SceneToolbar';
import RaytracerCanvas from './components/canvas/RaytracerCanvas';
import ControlPanel from './components/controls/ControlPanel';
import './App.css';

const DEFAULT_CAMERA = { x: 0, y: 0.5, z: -4 };

function App() {
  const { wasmModule, loading, error } = useWasm();
  
  // Scene state
  const [scenePreset, setScenePreset] = useState(0);
  const [sphereCount, setSphereCount] = useState(1);
  const [light, setLight] = useState({ x: 2, y: 3, z: -2 });
  const [material, setMaterial] = useState({ 
    color: { r: 0.9, g: 0.2, b: 0.15 },
    specular: 0.5, 
    shininess: 32, 
    reflectivity: 0.3 
  });
  const [camera, setCamera] = useState({ ...DEFAULT_CAMERA });
  const [view, setView] = useState({ 
    showGroundPlane: true,
    showGrid: true, 
    gridScale: 1.0,
    groundReflectivity: 0.15,
    maxBounces: 5,
    resolution: 512 
  });
  const [renderTime, setRenderTime] = useState(0);

  // Handle scene preset change
  const handlePresetChange = useCallback((presetId) => {
    setScenePreset(presetId);
    if (wasmModule) {
      wasmModule.loadScenePreset(presetId);
      setSphereCount(wasmModule.getSphereCount());
    }
  }, [wasmModule]);

  // Initialize when WASM loads
  useEffect(() => {
    if (wasmModule) {
      setSphereCount(wasmModule.getSphereCount());
    }
  }, [wasmModule]);

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
        <div className="viewport">
          <SceneToolbar
            activePreset={scenePreset}
            onPresetChange={handlePresetChange}
            sphereCount={sphereCount}
            disabled={isDisabled}
          />
          
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
                scenePreset={scenePreset}
                onCameraChange={setCamera}
                onRenderTime={setRenderTime}
              />
            )}
          </div>
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
            <h3>Render Info</h3>
            <ul>
              <li><span>Objects</span><span>{sphereCount} + Ground</span></li>
              <li><span>Lights</span><span>1 Point</span></li>
              <li><span>Bounces</span><span>{view.maxBounces}</span></li>
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
