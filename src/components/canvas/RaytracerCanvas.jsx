import { useRef, useEffect, useCallback, useState } from 'react';
import './RaytracerCanvas.css';

function RaytracerCanvas({ 
  wasmModule, 
  lights, 
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
  const [displaySize, setDisplaySize] = useState({ width: 700, height: 700 });
  const lastMousePos = useRef({ x: 0, y: 0 });
  const renderRequestRef = useRef(null);
  const lastPresetRef = useRef(scenePreset);

  // Calculate display size to fill container while maintaining square aspect
  useEffect(() => {
    const updateSize = () => {
      if (containerRef.current) {
        const rect = containerRef.current.getBoundingClientRect();
        const maxSize = Math.min(rect.width - 20, rect.height - 20);
        const size = Math.max(300, maxSize);
        setDisplaySize({ width: size, height: size });
      }
    };

    updateSize();
    window.addEventListener('resize', updateSize);
    const timeout = setTimeout(updateSize, 100);
    
    return () => {
      window.removeEventListener('resize', updateSize);
      clearTimeout(timeout);
    };
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

    // Update all lights
    lights.forEach((light, index) => {
      wasmModule.setLightPosition(index, light.x, light.y, light.z);
      wasmModule.setLightColor(index, light.color.r, light.color.g, light.color.b);
      wasmModule.setLightIntensity(index, light.intensity);
    });

    // Material updates
    wasmModule.updateMaterial(material.specular, material.shininess, material.reflectivity);
    wasmModule.updateSphereColor(material.color.r, material.color.g, material.color.b);
    
    // Camera updates
    wasmModule.updateCamera(camera.position.x, camera.position.y, camera.position.z);
    wasmModule.setCameraTarget(camera.target.x, camera.target.y, camera.target.z);
    wasmModule.setCameraFov(camera.fov);
    
    // View settings
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
  }, [wasmModule, lights, material, camera, view, scenePreset, onRenderTime]);

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
    
    // Sync camera position back to React state
    onCameraChange({
      ...camera,
      position: {
        x: wasmModule.getCameraX(),
        y: wasmModule.getCameraY(),
        z: wasmModule.getCameraZ()
      }
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
      ...camera,
      position: {
        x: wasmModule.getCameraX(),
        y: wasmModule.getCameraY(),
        z: wasmModule.getCameraZ()
      }
    });
  };

  return (
    <div className="canvas-container" ref={containerRef}>
      <canvas
        ref={canvasRef}
        width={view.resolution}
        height={view.resolution}
        className={`raytracer-canvas ${isDragging ? 'dragging' : ''}`}
        style={{ 
          width: displaySize.width, 
          height: displaySize.height 
        }}
        onMouseDown={handleMouseDown}
        onMouseMove={handleMouseMove}
        onMouseUp={handleMouseUp}
        onMouseLeave={handleMouseUp}
        onWheel={handleWheel}
      />
      <div className="canvas-badge top-left">
        {view.resolution}×{view.resolution} • {lights.length} light{lights.length > 1 ? 's' : ''}
      </div>
      <div className="canvas-badge bottom-right">
        Drag to orbit • Scroll to zoom
      </div>
    </div>
  );
}

export default RaytracerCanvas;
