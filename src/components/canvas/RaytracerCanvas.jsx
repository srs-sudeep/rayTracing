import { useRef, useEffect, useCallback, useState } from 'react';
import './RaytracerCanvas.css';

function RaytracerCanvas({ 
  wasmModule, 
  light, 
  material, 
  camera,
  view,
  onCameraChange,
  onRenderTime 
}) {
  const canvasRef = useRef(null);
  const [isDragging, setIsDragging] = useState(false);
  const lastMousePos = useRef({ x: 0, y: 0 });

  const renderFrame = useCallback(() => {
    if (!wasmModule || !canvasRef.current) return;

    const canvas = canvasRef.current;
    const ctx = canvas.getContext('2d');
    const resolution = view.resolution;

    // Update C++ state
    wasmModule.updateLight(light.x, light.y, light.z);
    wasmModule.updateMaterial(material.specular, material.shininess);
    wasmModule.updateCamera(camera.x, camera.y, camera.z);
    wasmModule.setShowGrid(view.showGrid);
    wasmModule.setGridSize(view.gridSize);

    // Time the render
    const startTime = performance.now();
    const pixelVector = wasmModule.render(resolution, resolution);
    const endTime = performance.now();
    onRenderTime(endTime - startTime);

    // Copy data from C++ vector
    const size = pixelVector.size();
    const pixelData = new Uint8ClampedArray(size);
    for (let i = 0; i < size; i++) {
      pixelData[i] = pixelVector.get(i);
    }
    pixelVector.delete();

    // Draw to canvas
    const imageData = new ImageData(pixelData, resolution, resolution);
    
    // If canvas size differs from resolution, scale it
    if (canvas.width !== resolution) {
      canvas.width = resolution;
      canvas.height = resolution;
    }
    
    ctx.putImageData(imageData, 0, 0);
  }, [wasmModule, light, material, camera, view, onRenderTime]);

  useEffect(() => {
    renderFrame();
  }, [renderFrame]);

  // Mouse handlers for camera orbit
  const handleMouseDown = (e) => {
    setIsDragging(true);
    lastMousePos.current = { x: e.clientX, y: e.clientY };
  };

  const handleMouseMove = (e) => {
    if (!isDragging || !wasmModule) return;

    const deltaX = e.clientX - lastMousePos.current.x;
    const deltaY = e.clientY - lastMousePos.current.y;
    lastMousePos.current = { x: e.clientX, y: e.clientY };

    // Orbit camera in C++
    wasmModule.orbitCamera(deltaX, deltaY);
    
    // Sync camera position back to React state
    onCameraChange({
      x: wasmModule.getCameraX(),
      y: wasmModule.getCameraY(),
      z: wasmModule.getCameraZ()
    });
  };

  const handleMouseUp = () => {
    setIsDragging(false);
  };

  const handleWheel = (e) => {
    if (!wasmModule) return;
    e.preventDefault();
    
    const delta = e.deltaY * 0.01;
    wasmModule.zoomCamera(delta);
    
    onCameraChange({
      x: wasmModule.getCameraX(),
      y: wasmModule.getCameraY(),
      z: wasmModule.getCameraZ()
    });
  };

  return (
    <div className="canvas-wrapper">
      <canvas
        ref={canvasRef}
        width={view.resolution}
        height={view.resolution}
        className={`raytracer-canvas ${isDragging ? 'dragging' : ''}`}
        onMouseDown={handleMouseDown}
        onMouseMove={handleMouseMove}
        onMouseUp={handleMouseUp}
        onMouseLeave={handleMouseUp}
        onWheel={handleWheel}
      />
      <div className="canvas-overlay">
        <span className="resolution-badge">{view.resolution}×{view.resolution}</span>
      </div>
    </div>
  );
}

export default RaytracerCanvas;

