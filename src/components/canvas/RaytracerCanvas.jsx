import { useRef, useEffect, useCallback, useState } from 'react';
import './RaytracerCanvas.css';

function RaytracerCanvas({ 
  wasmModule, 
  light, 
  material, 
  camera,
  view,
  scenePreset,
  onCameraChange,
  onRenderTime 
}) {
  const canvasRef = useRef(null);
  const containerRef = useRef(null);
  const [isDragging, setIsDragging] = useState(false);
  const [canvasSize, setCanvasSize] = useState(600);
  const lastMousePos = useRef({ x: 0, y: 0 });
  const renderRequestRef = useRef(null);
  const lastPresetRef = useRef(scenePreset);

  // Calculate canvas size based on container
  useEffect(() => {
    const updateSize = () => {
      if (containerRef.current) {
        const rect = containerRef.current.getBoundingClientRect();
        // Use the smaller dimension, with some padding
        const maxSize = Math.min(rect.width - 40, rect.height - 80);
        const size = Math.max(400, Math.min(800, maxSize));
        setCanvasSize(size);
      }
    };

    updateSize();
    window.addEventListener('resize', updateSize);
    return () => window.removeEventListener('resize', updateSize);
  }, []);

  // Optimized render function
  const renderFrame = useCallback(() => {
    if (!wasmModule || !canvasRef.current) return;

    const canvas = canvasRef.current;
    const ctx = canvas.getContext('2d');
    const resolution = view.resolution;

    // Update scene preset if changed
    if (lastPresetRef.current !== scenePreset) {
      wasmModule.loadScenePreset(scenePreset);
      lastPresetRef.current = scenePreset;
    }

    // Update C++ state
    wasmModule.updateLight(light.x, light.y, light.z);
    wasmModule.updateMaterial(material.specular, material.shininess, material.reflectivity);
    wasmModule.updateCamera(camera.x, camera.y, camera.z);
    wasmModule.setShowGroundPlane(view.showGroundPlane);
    wasmModule.setShowGrid(view.showGrid);
    wasmModule.setGridScale(view.gridScale);
    wasmModule.updateGroundReflectivity(view.groundReflectivity);
    wasmModule.setMaxReflectionDepth(view.maxBounces);

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
    
    if (canvas.width !== resolution) {
      canvas.width = resolution;
      canvas.height = resolution;
    }
    
    ctx.putImageData(imageData, 0, 0);
  }, [wasmModule, light, material, camera, view, scenePreset, onRenderTime]);

  // Debounced render
  useEffect(() => {
    if (renderRequestRef.current) {
      cancelAnimationFrame(renderRequestRef.current);
    }
    renderRequestRef.current = requestAnimationFrame(renderFrame);
    
    return () => {
      if (renderRequestRef.current) {
        cancelAnimationFrame(renderRequestRef.current);
      }
    };
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

    wasmModule.orbitCamera(deltaX, deltaY);
    
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
    <div className="canvas-container" ref={containerRef}>
      <div className="canvas-wrapper" style={{ width: canvasSize, height: canvasSize }}>
        <canvas
          ref={canvasRef}
          width={view.resolution}
          height={view.resolution}
          className={`raytracer-canvas ${isDragging ? 'dragging' : ''}`}
          style={{ width: canvasSize, height: canvasSize }}
          onMouseDown={handleMouseDown}
          onMouseMove={handleMouseMove}
          onMouseUp={handleMouseUp}
          onMouseLeave={handleMouseUp}
          onWheel={handleWheel}
        />
        <div className="canvas-badge top-left">
          {view.resolution}×{view.resolution}
        </div>
        <div className="canvas-badge bottom-right">
          🖱️ Drag to orbit • ⚙️ Scroll to zoom
        </div>
      </div>
    </div>
  );
}

export default RaytracerCanvas;
