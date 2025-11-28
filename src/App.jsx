import { useState, useEffect, useCallback } from 'react';
import useWasm from './hooks/useWasm';
import Header from './components/layout/Header';
import SceneToolbar from './components/canvas/SceneToolbar';
import RaytracerCanvas from './components/canvas/RaytracerCanvas';
import ControlPanel from './components/controls/ControlPanel';
import './App.css';

const DEFAULT_CAMERA = { 
  position: { x: 0, y: 0.5, z: -4 },
  target: { x: 0, y: 0, z: 0 },
  fov: 60
};

const DEFAULT_LIGHT = {
  x: 2, y: 3, z: -2,
  color: { r: 1.0, g: 1.0, b: 1.0 },
  intensity: 1.0
};

function App() {
  const { wasmModule, loading, error } = useWasm();
  
  // Scene state
  const [scenePreset, setScenePreset] = useState(0);
  const [sphereCount, setSphereCount] = useState(1);
  const [lights, setLights] = useState([{ ...DEFAULT_LIGHT }]);
  const [material, setMaterial] = useState({ 
    color: { r: 0.9, g: 0.2, b: 0.15 },
    specular: 0.5, 
    shininess: 32, 
    reflectivity: 0.3 
  });
  const [camera, setCamera] = useState({ 
    position: { ...DEFAULT_CAMERA.position },
    target: { ...DEFAULT_CAMERA.target },
    fov: DEFAULT_CAMERA.fov
  });
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
    setCamera({ 
      position: { ...DEFAULT_CAMERA.position },
      target: { ...DEFAULT_CAMERA.target },
      fov: DEFAULT_CAMERA.fov
    });
    if (wasmModule) {
      wasmModule.updateCamera(DEFAULT_CAMERA.position.x, DEFAULT_CAMERA.position.y, DEFAULT_CAMERA.position.z);
      wasmModule.setCameraTarget(DEFAULT_CAMERA.target.x, DEFAULT_CAMERA.target.y, DEFAULT_CAMERA.target.z);
      wasmModule.setCameraFov(DEFAULT_CAMERA.fov);
    }
  };

  // Add a new light
  const handleAddLight = useCallback(() => {
    if (lights.length >= 4) return;
    
    // New lights at different positions
    const positions = [
      { x: 2, y: 3, z: -2 },
      { x: -3, y: 2, z: -1 },
      { x: 0, y: 4, z: 2 },
      { x: 3, y: 1, z: 1 }
    ];
    const colors = [
      { r: 1.0, g: 1.0, b: 1.0 },
      { r: 1.0, g: 0.8, b: 0.6 },
      { r: 0.6, g: 0.8, b: 1.0 },
      { r: 1.0, g: 0.9, b: 0.7 }
    ];
    
    const newLight = {
      ...positions[lights.length % positions.length],
      color: colors[lights.length % colors.length],
      intensity: 0.8
    };
    
    setLights(prev => [...prev, newLight]);
    
    if (wasmModule) {
      wasmModule.addLight(
        newLight.x, newLight.y, newLight.z,
        newLight.color.r, newLight.color.g, newLight.color.b,
        newLight.intensity
      );
    }
  }, [lights.length, wasmModule]);

  // Remove a light
  const handleRemoveLight = useCallback((index) => {
    if (lights.length <= 1) return;
    
    setLights(prev => prev.filter((_, i) => i !== index));
    
    if (wasmModule) {
      wasmModule.removeLight(index);
    }
  }, [lights.length, wasmModule]);

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
                lights={lights}
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
            lights={lights}
            onLightsChange={setLights}
            onAddLight={handleAddLight}
            onRemoveLight={handleRemoveLight}
            material={material}
            onMaterialChange={setMaterial}
            camera={camera}
            onCameraChange={setCamera}
            onCameraReset={handleCameraReset}
            view={view}
            onViewChange={setView}
            disabled={isDisabled}
            wasmModule={wasmModule}
          />

          <div className="scene-info">
            <h3>Render Info</h3>
            <ul>
              <li><span>Objects</span><span>{sphereCount} + Ground</span></li>
              <li><span>Lights</span><span>{lights.length} Point{lights.length > 1 ? 's' : ''}</span></li>
              <li><span>Bounces</span><span>{view.maxBounces}</span></li>
            </ul>
          </div>
        </aside>
      </main>

      <footer className="footer">
        <span>RayTracer Studio</span>
        <span className="footer-sep">•</span>
        <span>C++ / WebAssembly / React</span>
      </footer>
    </div>
  );
}

export default App;
